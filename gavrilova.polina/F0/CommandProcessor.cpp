#include "CommandProcessor.hpp"
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <limits>
#include <vector>

namespace {

  struct StudentPrinter {
    std::ostream& out;
    void operator()(const std::shared_ptr< const gavrilova::student::Student >& s) const
    {
      out << "ID: " << s->id_ << ", ФИО: " << s->fullName_
          << ", Группа: " << s->group_ << ", Средний балл: " << std::fixed
          << std::setprecision(2) << s->averageGrade_ << '\n';
    }
  };

  struct GradePrinter {
    std::ostream& out;
    void operator()(const std::pair< const gavrilova::date::Date, int >& grade) const
    {
      out << "  " << gavrilova::date::to_string(grade.first) << ": " << grade.second << '\n';
    }
  };
}

gavrilova::CommandProcessor::CommandProcessor(StudentDatabase& db):
  db_(db),
  commandMap_()
{
  commandMap_["--help"] = std::bind(&CommandProcessor::handleHelp, this);
  commandMap_["addstudent"] = std::bind(&CommandProcessor::handleAddStudent, this);
  commandMap_["deletestudent"] = std::bind(&CommandProcessor::handleDeleteStudent, this);
  commandMap_["infostudent"] = std::bind(&CommandProcessor::handleInfoStudent, this);
  commandMap_["listgroup"] = std::bind(&CommandProcessor::handleListGroup, this);
  commandMap_["movegroup"] = std::bind(&CommandProcessor::handleMoveGroup, this);
  commandMap_["addgrade"] = std::bind(&CommandProcessor::handleAddGrade, this);
  commandMap_["changegrade"] = std::bind(&CommandProcessor::handleChangeGrade, this);
  commandMap_["removegrade"] = std::bind(&CommandProcessor::handleRemoveGrade, this);
  commandMap_["topstudentsall"] = std::bind(&CommandProcessor::handleTopStudentsAll, this);
  commandMap_["riskstudentsall"] = std::bind(&CommandProcessor::handleRiskStudentsAll, this);
  commandMap_["topstudentsgroup"] = std::bind(&CommandProcessor::handleTopStudentsGroup, this);
  commandMap_["riskstudentsgroup"] = std::bind(&CommandProcessor::handleRiskStudentsGroup, this);
  commandMap_["exportforgrades"] = std::bind(&CommandProcessor::handleExportForGrades, this);
  commandMap_["loadgrades"] = std::bind(&CommandProcessor::handleLoadGrades, this);
  commandMap_["groupstats"] = std::bind(&CommandProcessor::handleGroupStats, this);
  commandMap_["avgmarkbydate"] = std::bind(&CommandProcessor::handleAvgMarkByDate, this);
  commandMap_["clear"] = std::bind(&CommandProcessor::handleClear, this);
  commandMap_["create_group"] = std::bind(&CommandProcessor::handleCreateGroup, this);
  commandMap_["save"] = std::bind(&CommandProcessor::handleSave, this);
  commandMap_["loadbase"] = std::bind(&CommandProcessor::handleLoad, this);
}

void gavrilova::CommandProcessor::run()
{
  std::string command;
  while (std::cout << "> ", std::cin >> command) {
    auto it = commandMap_.find(command);
    if (it != commandMap_.end()) {
      it->second();
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }

    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}

void gavrilova::CommandProcessor::handleHelp()
{
  std::cout << "Список доступных команд:\n"
               "--help\n"
               "addstudent <\"ФИО\"> <группа>\n"
               "deletestudent <ID>\n"
               "infostudent <ID | \"ФИО\">\n"
               "listgroup <группа>\n"
               "listgrades <ID | \"ФИО\">\n"
               "movegroup <ID> <новая_группа>\n"
               "addgrade <ID | \"ФИО\"> <оценка> <ДД-ММ-ГГГГ>\n"
               "changegrade <ID | \"ФИО\"> <оценка> <ДД-ММ-ГГГГ>\n"
               "removegrade <ID | \"ФИО\"> <ДД-ММ-ГГГГ>\n"
               "topstudentsall <N>\n"
               "riskstudentsall <порог>\n"
               "topstudentsgroup <группа> <N>\n"
               "riskstudentsgroup <группа> <порог>\n"
               "exportforgrades <группа> <файл>\n"
               "loadgrades <группа> <ДД-ММ-ГГГГ> <файл>\n"
               "groupstats <группа> <старт_ДД-ММ-ГГГГ> <конец_ДД-ММ-ГГГГ>\n"
               "avgmarkbydate <ДД-ММ-ГГГГ>\n"
               "create_group <группа>\n"
               "save <файл>\n"
               "loadbase <файл>\n"
               "clear\n";
}

void gavrilova::CommandProcessor::handleAddStudent()
{
  std::string fullName, groupName;
  std::cin >> std::quoted(fullName) >> groupName;
  auto result = db_.addStudent(fullName, groupName);
  bool success = result.first;
  auto id = result.second;
  if (success) {
    std::cout << "Студент добавлен с ID: " << id << '\n';
  } else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void gavrilova::CommandProcessor::handleDeleteStudent()
{
  StudentID id;
  std::cin >> id;
  if (db_.deleteStudent(id)) {
    std::cout << "Студент с ID " << id << " удален.\n";
  } else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void gavrilova::CommandProcessor::handleInfoStudent()
{
  std::string token;
  std::cin >> token;
  StudentID id = 0;
  try {
    id = std::stoul(token);
    auto student = db_.findStudentById(id);
    if (student) {
      StudentPrinter{std::cout}(student);
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
  } catch (const std::invalid_argument&) {
    auto ids = db_.findStudentsByName(token);
    if (ids.empty()) {
      std::cout << "<INVALID COMMAND>\n";
    } else if (ids.size() == 1) {
      StudentPrinter{std::cout}(db_.findStudentById(ids.front()));
    } else {
      std::cout << "Найдено несколько студентов. Введите ID одного из них:\n";
      std::copy(ids.begin(), ids.end(), std::ostream_iterator< StudentID >(std::cout, " "));
    }
  }
}

void gavrilova::CommandProcessor::handleListGroup()
{
  std::string groupName;
  std::cin >> groupName;
  auto students = db_.getStudentsInGroup(groupName);
  if (students.empty() && !db_.groupExists(groupName)) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::cout << "Студенты группы " << groupName << ":\n";
  std::for_each(students.begin(), students.end(), StudentPrinter{std::cout});
}

void gavrilova::CommandProcessor::handleMoveGroup()
{
  StudentID id;
  std::string newGroup;
  std::cin >> id >> newGroup;
  if (db_.moveStudentToGroup(id, newGroup)) {
    std::cout << "Студент с ID " << id << " переведен в группу " << newGroup << ".\n";
  } else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void gavrilova::CommandProcessor::handleAddGrade()
{
  // не успела реализовать!!
}

void gavrilova::CommandProcessor::handleChangeGrade()
{
  // не успела реализовать!!
}

void gavrilova::CommandProcessor::handleRemoveGrade()
{
  // не успела реализовать!!
}

void gavrilova::CommandProcessor::handleTopStudentsAll()
{
  size_t n;
  std::cin >> n;
  auto students = db_.getTopStudents(n);
  std::cout << "Топ-" << n << " студентов по среднему баллу:\n";
  std::for_each(students.begin(), students.end(), StudentPrinter{std::cout});
}

void gavrilova::CommandProcessor::handleRiskStudentsAll()
{
  double threshold;
  std::cin >> threshold;
  if (threshold <= 0.0 || threshold > 5.0) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  auto students = db_.getRiskStudents(threshold);
  std::cout << "Студенты со средним баллом ниже " << threshold << ":\n";
  std::for_each(students.begin(), students.end(), StudentPrinter{std::cout});
}

void gavrilova::CommandProcessor::handleTopStudentsGroup()
{
  std::string groupName;
  size_t n;
  std::cin >> groupName >> n;
  if (!db_.groupExists(groupName)) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  auto students = db_.getTopStudentsInGroup(groupName, n);
  std::cout << "Топ-" << n << " студентов группы " << groupName << ":\n";
  std::for_each(students.begin(), students.end(), StudentPrinter{std::cout});
}

void gavrilova::CommandProcessor::handleRiskStudentsGroup()
{
  std::string groupName;
  double threshold;
  std::cin >> groupName >> threshold;
  if (threshold <= 0.0 || threshold > 5.0 || !db_.groupExists(groupName)) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  auto students = db_.getRiskStudentsInGroup(groupName, threshold);
  std::cout << "Студенты группы " << groupName << " со средним баллом ниже " << threshold << ":\n";
  std::for_each(students.begin(), students.end(), StudentPrinter{std::cout});
}

void gavrilova::CommandProcessor::handleExportForGrades()
{
  std::string groupName, filename;
  std::cin >> groupName >> filename;
  if (db_.exportGroupForGrading(groupName, filename)) {
    std::cout << "Шаблон для группы " << groupName << " экспортирован в " << filename << '\n';
  } else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void gavrilova::CommandProcessor::handleLoadGrades()
{
  std::string groupName, filename;
  date::Date date;
  std::cin >> groupName >> date >> filename;
  if (std::cin.fail()) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }
  if (db_.loadGradesFromFile(groupName, date, filename)) {
    std::cout << "Оценки для группы " << groupName << " из файла " << filename << " загружены.\n";
  } else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void gavrilova::CommandProcessor::handleGroupStats()
{
  std::string groupName;
  DateRange period;
  std::cin >> groupName >> period.start >> period.end;
  if (std::cin.fail()) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }

  auto result = db_.getGroupStatistics(groupName, period);
  bool success = result.first;
  auto stats = result.second;
  if (!success) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  std::cout << "Статистика по группе " << groupName << " за период с "
            << date::to_string(period.start) << " по " << date::to_string(period.end) << ":\n";
  std::cout << " - Средний балл по группе: " << std::fixed << std::setprecision(2) << stats.groupAverage << '\n';
  std::cout << " - Средний балл по остальным группам: " << stats.allOtherGroupsAverage << '\n';

  std::cout << " - Распределение оценок:\n";
  std::for_each(stats.gradeDistribution.begin(), stats.gradeDistribution.end(),
      [](const auto& p) { std::cout << "    Оценка " << p.first << ": " << p.second << " шт.\n"; });

  std::cout << " - Лучшие студенты:\n";
  std::for_each(stats.topStudents.begin(), stats.topStudents.end(), StudentPrinter{std::cout});

  std::cout << " - Худшие студенты:\n";
  std::for_each(stats.bottomStudents.begin(), stats.bottomStudents.end(), StudentPrinter{std::cout});
}

void gavrilova::CommandProcessor::handleAvgMarkByDate()
{
  date::Date date;
  std::cin >> date;
  if (std::cin.fail()) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }
  auto result = db_.getAverageGradeByDate(date);
  bool success = result.first;
  auto avg = result.second;
  if (success) {
    std::cout << "Средний балл за " << date::to_string(date) << ": " << std::fixed << std::setprecision(2) << avg << '\n';
  } else {
    std::cout << "Нет данных за указанную дату.\n";
  }
}

void gavrilova::CommandProcessor::handleClear()
{
  char response;
  std::cout << "Вы уверены, что хотите очистить всю базу данных? (y/n): ";
  std::cin >> response;
  if (response == 'y' || response == 'Y') {
    db_.clear();
    std::cout << "База данных очищена.\n";
  } else {
    std::cout << "Отмена.\n";
  }
}

void gavrilova::CommandProcessor::handleCreateGroup()
{
  std::string groupName;
  std::cin >> groupName;
  if (db_.createGroup(groupName)) {
    std::cout << "Группа " << groupName << " создана.\n";
  } else {
    std::cout << "Группа " << groupName << " уже существует.\n";
  }
}

void gavrilova::CommandProcessor::handleSave()
{
  std::string filename;
  std::cin >> filename;
  if (db_.saveToFile(filename)) {
    std::cout << "База данных сохранена в файл " << filename << '\n';
  } else {
    std::cout << "Ошибка сохранения файла.\n";
  }
}

void gavrilova::CommandProcessor::handleLoad()
{
  std::string filename;
  std::cin >> filename;
  if (db_.loadFromFile(filename)) {
    std::cout << "База данных загружена из файла " << filename << '\n';
  } else {
    std::cout << "Ошибка загрузки файла.\n";
  }
}
