#include "StudentsDataBase.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <numeric>
#include <stdexcept>

namespace {

  template < typename Pair >
  struct PairToSecond {
    const typename Pair::second_type& operator()(const Pair& p) const
    {
      return p.second;
    }
  };

  struct CompareStudentsByGrade {
    bool operator()(const std::shared_ptr< const gavrilova::student::Student >& a,
        const std::shared_ptr< const gavrilova::student::Student >& b) const
    {
      if (a->averageGrade_ == b->averageGrade_) {
        return a->id_ < b->id_;
      }
      return a->averageGrade_ > b->averageGrade_;
    }
  };

  struct CompareStudentsByGradeAsc {
    bool operator()(const std::shared_ptr< const gavrilova::student::Student >& a,
        const std::shared_ptr< const gavrilova::student::Student >& b) const
    {
      if (a->averageGrade_ == b->averageGrade_) {
        return a->id_ < b->id_;
      }
      return a->averageGrade_ < b->averageGrade_;
    }
  };

  struct IsGradeInPeriod {
    const gavrilova::DateRange& period;
    bool operator()(const std::pair< gavrilova::date::Date, int >& grade) const
    {
      return !(grade.first < period.start) && !(period.end < grade.first);
    }
  };

  struct GroupStatsAccumulator {
    const gavrilova::DateRange& period;
    std::vector< int >& grades;
    std::map< int, int >& distribution;

    void operator()(const std::shared_ptr< const gavrilova::student::Student >& student) const
    {
      struct GradeProcessor {
        const gavrilova::DateRange& p;
        std::vector< int >& g;
        std::map< int, int >& d;

        void operator()(const std::pair< gavrilova::date::Date, int >& grade) const
        {
          if (IsGradeInPeriod{p}(grade)) {
            g.push_back(grade.second);
            d[grade.second]++;
          }
        }
      };

      std::for_each(student->grades_.begin(), student->grades_.end(),
          GradeProcessor{period, grades, distribution});
    }
  };

  struct OtherGroupsGradesCollector {
    const gavrilova::DateRange& period;
    const std::string& currentGroup;
    std::vector< int >& grades;

    void operator()(const std::pair< std::string, gavrilova::StudentDatabase::Group >& group) const
    {
      if (group.first == currentGroup) {
        return;
      }

      struct StudentProcessor {
        const gavrilova::DateRange& p;
        std::vector< int >& g;

        void operator()(const std::pair< gavrilova::StudentID,
            std::shared_ptr< gavrilova::student::Student > >& student) const
        {
          struct GradeExtractor {
            const gavrilova::DateRange& period;
            std::vector< int >& grades;

            void operator()(const std::pair< gavrilova::date::Date, int >& grade) const
            {
              if (IsGradeInPeriod{period}(grade)) {
                grades.push_back(grade.second);
              }
            }
          };

          std::for_each(student.second->grades_.begin(), student.second->grades_.end(),
              GradeExtractor{p, g});
        }
      };

      std::for_each(group.second.begin(), group.second.end(),
          StudentProcessor{period, grades});
    }
  };

  struct IsRiskStudentPredicate {
    double threshold;
    bool operator()(const std::pair< gavrilova::StudentID,
        std::shared_ptr< gavrilova::student::Student > >& p) const
    {
      return !p.second->grades_.empty() && p.second->averageGrade_ < threshold;
    }
  };

  struct IsRiskStudentInGroupPredicate {
    double threshold;
    bool operator()(const std::shared_ptr< const gavrilova::student::Student >& s) const
    {
      return !s->grades_.empty() && s->averageGrade_ < threshold;
    }
  };

  struct GradePairAccumulator {
    double operator()(double sum, const std::pair< gavrilova::StudentID, int >& p) const
    {
      return sum + p.second;
    }
  };

  struct StudentAdder {
    gavrilova::StudentDatabase& db;

    void operator()(const gavrilova::student::Student& stud) const
    {
      if (!db.groupExists(stud.group_)) {
        db.createGroup(stud.group_);
      }
      auto pair = db.addStudent(stud.fullName_, stud.group_);
      bool ok = pair.first;
      gavrilova::StudentID id = pair.second;
      if (ok) {
        auto sp = db.findStudentById(id);
        if (sp) {
          sp->grades_ = stud.grades_;
          db.updateStudentAverageGrade(sp);
        }
      }
    }
  };

  struct StudentToFileWriter {
    std::ostream& out;
    void operator()(const std::pair< gavrilova::StudentID,
        std::shared_ptr< gavrilova::student::Student > >& p) const
    {
      out << *p.second << '\n';
    }
  };

  struct GradeFileLineProcessor {
    gavrilova::StudentDatabase& db;
    const std::string& groupName;
    const gavrilova::date::Date& date;
    mutable gavrilova::StudentID currentId = 0;

    void operator()(const std::string& line) const
    {
      if (line.find("ID:") == 0) {
        currentId = std::stoul(line.substr(3));
      } else if (line.find("Оценка:") == 0 && currentId) {
        std::string value = line.substr(13);
        value.erase(0, value.find_first_not_of(' '));
        if (!value.empty() && std::any_of(value.begin(), value.end(), ::isdigit)) {
          int grade = std::stoi(value);
          db.addGrade(currentId, grade, date);
        }
      }
    }
  };

  struct GroupStudentExporter {
    std::ostream& out;

    void operator()(const std::pair< gavrilova::StudentID,
        std::shared_ptr< gavrilova::student::Student > >& p) const
    {
      const auto& student = p.second;
      out << "ID:" << student->id_ << '\n';
      out << "ФИО:" << student->fullName_ << '\n';
      out << "Оценка:\n";
    }
  };
}

gavrilova::StudentDatabase::StudentDatabase(int id_digits)
{
  nextId = std::pow(10, id_digits - 1) + 1;
}

bool gavrilova::StudentDatabase::saveToFile(const std::string& filename) const
{
  std::ofstream out(filename);
  if (!out) {
    return false;
  }

  std::for_each(students.begin(), students.end(), StudentToFileWriter{out});
  return true;
}

bool gavrilova::StudentDatabase::loadFromFile(const std::string& filename)
{
  std::ifstream in(filename);
  if (!in) {
    return false;
  }

  std::vector< student::Student > loadedStudents(std::istream_iterator< student::Student >{in},
      std::istream_iterator< student::Student >{});

  std::for_each(loadedStudents.begin(), loadedStudents.end(), StudentAdder{*this});

  return true;
}

void gavrilova::StudentDatabase::clear()
{
  students.clear();
  groups.clear();
  nameToStudentIndex.clear();
  dateToGradesIndex.clear();
}

bool gavrilova::StudentDatabase::createGroup(const std::string& groupName)
{
  return groups.emplace(groupName, Group{}).second;
}

bool gavrilova::StudentDatabase::groupExists(const std::string& groupName) const
{
  return groups.count(groupName) > 0;
}

std::pair< bool, gavrilova::StudentID > gavrilova::StudentDatabase::addStudent
(const std::string& fullName, const std::string& groupName)
{
  if (!groupExists(groupName)) {
    return {false, 0};
  }

  struct StudentInGroupPredicate {
    const std::string& group;
    const std::map< StudentID, std::shared_ptr< student::Student > >& students;

    bool operator()(StudentID id) const
    {
      return students.at(id)->group_ == group;
    }
  };

  auto it_name = nameToStudentIndex.find(fullName);
  if (it_name != nameToStudentIndex.end()) {
    const auto& ids = it_name->second;
    if (std::any_of(ids.begin(), ids.end(), StudentInGroupPredicate{groupName, students})) {
      return {false, 0};
    }
  }

  auto student = std::make_shared< student::Student >(nextId, fullName, groupName);
  students[nextId] = student;
  groups[groupName][nextId] = student;
  nameToStudentIndex[fullName].insert(nextId);

  return {true, nextId++};
}

bool gavrilova::StudentDatabase::deleteStudent(StudentID id)
{
  auto it_student = students.find(id);
  if (it_student == students.end()) {
    return false;
  }
  const auto& student = it_student->second;

  struct GradeRemoverFromDateIndex {
    struct IsStudentGrade {
      StudentID studentId;
      bool operator()(const std::pair< StudentID, int >& p) const
      {
        return p.first == studentId;
      }
    };
    StudentID studentIdToRemove;
    std::map< date::Date, std::vector< std::pair< StudentID, int > > >* dateIndex;

    void operator()(const std::pair< const date::Date, int >& gradeEntry)
    {
      auto it_date = dateIndex->find(gradeEntry.first);
      if (it_date != dateIndex->end()) {
        auto& gradesOnDate = it_date->second;
        auto newEnd = std::remove_if(gradesOnDate.begin(), gradesOnDate.end(), IsStudentGrade{studentIdToRemove});
        gradesOnDate.erase(newEnd, gradesOnDate.end());

        if (gradesOnDate.empty()) {
          dateIndex->erase(it_date);
        }
      }
    }
  };

  std::for_each(student->grades_.begin(), student->grades_.end(),
      GradeRemoverFromDateIndex{id, &dateToGradesIndex});

  groups.at(student->group_).erase(id);

  auto& name_set = nameToStudentIndex.at(student->fullName_);
  name_set.erase(id);
  if (name_set.empty()) {
    nameToStudentIndex.erase(student->fullName_);
  }
  students.erase(it_student);
  return true;
}

bool gavrilova::StudentDatabase::moveStudentToGroup(StudentID id, const std::string& newGroupName)
{
  auto student_ptr = findStudentById(id);
  if (!student_ptr || !groupExists(newGroupName) || student_ptr->group_ == newGroupName) {
    return false;
  }

  groups.at(student_ptr->group_).erase(id);
  groups[newGroupName][id] = student_ptr;
  student_ptr->group_ = newGroupName;

  return true;
}

std::shared_ptr< const gavrilova::student::Student > gavrilova::StudentDatabase::findStudentById(StudentID id) const
{
  auto it = students.find(id);
  return (it != students.end()) ? it->second : nullptr;
}

std::shared_ptr< gavrilova::student::Student > gavrilova::StudentDatabase::findStudentById(StudentID id)
{
  auto it = students.find(id);
  return (it != students.end()) ? it->second : nullptr;
}

std::vector< gavrilova::StudentID > gavrilova::StudentDatabase::findStudentsByName(const std::string& fullName) const
{
  auto it = nameToStudentIndex.find(fullName);
  if (it == nameToStudentIndex.end()) {
    return {};
  }

  std::vector< StudentID > result;
  std::copy(it->second.begin(), it->second.end(), std::back_inserter(result));
  return result;
}

bool gavrilova::StudentDatabase::addGrade(StudentID id, int grade, const date::Date& date)
{
  auto student_ptr = findStudentById(id);

  if (!student_ptr || student_ptr->grades_.count(date)) {
    return false;
  }

  student_ptr->grades_[date] = grade;
  dateToGradesIndex[date].push_back({id, grade});
  updateStudentAverageGrade(student_ptr);

  return true;
}

bool gavrilova::StudentDatabase::changeGrade(StudentID id, int newGrade, const date::Date& date)
{
  auto student_ptr = findStudentById(id);

  if (!student_ptr || !student_ptr->grades_.count(date)) {
    return false;
  }
  student_ptr->grades_[date] = newGrade;

  struct GradeChanger {
    int newGrade;
    void operator()(std::pair< StudentID, int >& p) const
    {
      p.second = newGrade;
    }
  };

  struct FindGradePredicate {
    StudentID studentId;
    bool operator()(const std::pair< StudentID, int >& p) const
    {
      return p.first == studentId;
    }
  };

  auto& grades_on_date = dateToGradesIndex.at(date);
  auto it_grade = std::find_if(grades_on_date.begin(), grades_on_date.end(), FindGradePredicate{id});
  if (it_grade != grades_on_date.end()) {
    GradeChanger{newGrade}(*it_grade);
  }

  updateStudentAverageGrade(student_ptr);
  return true;
}

bool gavrilova::StudentDatabase::removeGradesByDate(StudentID id, const date::Date& date)
{
  auto student_ptr = findStudentById(id);
  if (!student_ptr || !student_ptr->grades_.count(date)) {
    return false;
  }
  student_ptr->grades_.erase(date);

  struct IsStudentGrade {
    StudentID studentId;
    bool operator()(const std::pair< StudentID, int >& p) const
    {
      return p.first == studentId;
    }
  };

  auto it_date = dateToGradesIndex.find(date);
  if (it_date != dateToGradesIndex.end()) {
    auto& grades_on_date = it_date->second;
    auto new_end = std::remove_if(grades_on_date.begin(), grades_on_date.end(), IsStudentGrade{id});
    grades_on_date.erase(new_end, grades_on_date.end());
    if (grades_on_date.empty()) {
      dateToGradesIndex.erase(it_date);
    }
  }

  updateStudentAverageGrade(student_ptr);
  return true;
}

void gavrilova::StudentDatabase::updateStudentAverageGrade(std::shared_ptr< student::Student >& student)
{
  if (student->grades_.empty()) {
    student->averageGrade_ = 0.0;
    return;
  }

  double sum = 0.0;
  for (const auto& grade: student->grades_) {
    sum += grade.second;
  }
  student->averageGrade_ = sum / student->grades_.size();
}

std::vector< std::shared_ptr< const gavrilova::student::Student > >
gavrilova::StudentDatabase::getStudentsInGroup(const std::string& groupName) const
{
  if (!groupExists(groupName)) {
    return {};
  }
  const auto& group_map = groups.at(groupName);
  std::vector< std::shared_ptr< const student::Student > > result;
  result.reserve(group_map.size());
  std::transform(group_map.begin(), group_map.end(), std::back_inserter(result),
      PairToSecond< Group::value_type >());
  return result;
}

bool gavrilova::StudentDatabase::exportGroupForGrading
    (const std::string& groupName, const std::string& filename) const
{
  auto it = groups.find(groupName);
  if (it == groups.end()) {
    std::cout << "1\n";
    return false;
  }
  std::ofstream out(filename);
  if (!out) {
    std::cout << "2\n";
    return false;
  }

  out << "#GROUP:" << groupName << '\n';

  std::for_each(it->second.begin(), it->second.end(), GroupStudentExporter{out});

  return true;
}

bool gavrilova::StudentDatabase::loadGradesFromFile(const std::string& groupName,
    const date::Date& date, const std::string& filename)
{
  if (!groupExists(groupName)) {
    return false;
  }

  std::ifstream in(filename);
  if (!in) {
    return false;
  }
  std::vector< std::string > lines((std::istream_iterator< std::string >(in)),
      std::istream_iterator< std::string >());

  std::for_each(lines.begin(), lines.end(), GradeFileLineProcessor{*this, groupName, date});

  return true;
}

std::pair< bool, gavrilova::GroupStatistics > gavrilova::StudentDatabase::getGroupStatistics
    (const std::string& groupName, const DateRange& period) const
{
  auto it = groups.find(groupName);
  if (it == groups.end()) return {false, {}};

  GroupStatistics stats;
  std::vector< int > groupGrades;
  std::vector< std::shared_ptr< const student::Student > > groupStudents;

  std::transform(it->second.begin(), it->second.end(),
      std::back_inserter(groupStudents),
      PairToSecond< Group::value_type >());

  std::for_each(groupStudents.begin(), groupStudents.end(),
      GroupStatsAccumulator{period, groupGrades, stats.gradeDistribution});

  if (!groupGrades.empty()) {
    stats.groupAverage = std::accumulate(groupGrades.begin(), groupGrades.end(), 0.0) / groupGrades.size();
  }

  size_t top_count = std::min(static_cast< size_t >(3), groupStudents.size());
  stats.topStudents.resize(top_count);
  std::partial_sort_copy(groupStudents.begin(), groupStudents.end(),
      stats.topStudents.begin(), stats.topStudents.end(),
      CompareStudentsByGrade{});

  stats.bottomStudents.resize(top_count);
  std::partial_sort_copy(groupStudents.begin(), groupStudents.end(),
      stats.bottomStudents.begin(), stats.bottomStudents.end(),
      CompareStudentsByGradeAsc{});

  std::vector< int > otherGrades;
  std::for_each(groups.begin(), groups.end(),
      OtherGroupsGradesCollector{period, groupName, otherGrades});

  if (!otherGrades.empty()) {
    stats.allOtherGroupsAverage = std::accumulate(otherGrades.begin(), otherGrades.end(), 0.0) / otherGrades.size();
  }

  return {true, stats};
}

std::vector< std::shared_ptr< const gavrilova::student::Student > >
gavrilova::StudentDatabase::getTopStudents(size_t n) const
{
  std::vector< std::shared_ptr< const student::Student > > allStudents;
  std::transform(students.begin(), students.end(),
      std::back_inserter(allStudents),
      PairToSecond< std::map< StudentID, std::shared_ptr< student::Student > >::value_type >());

  std::vector< std::shared_ptr< const student::Student > > result;
  result.resize(std::min(n, allStudents.size()));

  std::partial_sort_copy(allStudents.begin(), allStudents.end(),
      result.begin(), result.end(),
      CompareStudentsByGrade{});

  return result;
}

std::vector< std::shared_ptr< const gavrilova::student::Student > >
gavrilova::StudentDatabase::getRiskStudents(double threshold) const
{
  std::vector< std::pair< StudentID, std::shared_ptr< student::Student > > > temp;
  std::copy_if(students.begin(), students.end(),
      std::back_inserter(temp),
      IsRiskStudentPredicate{threshold});

  std::vector< std::shared_ptr< const student::Student > > result;
  std::transform(temp.begin(), temp.end(),
      std::back_inserter(result),
      PairToSecond< std::pair< StudentID, std::shared_ptr< student::Student > > >());

  return result;
}

std::vector< std::shared_ptr< const gavrilova::student::Student > >
gavrilova::StudentDatabase::getTopStudentsInGroup(const std::string& groupName, size_t n) const
{
  auto students = getStudentsInGroup(groupName);
  std::vector< std::shared_ptr< const student::Student > > result;
  result.resize(std::min(n, students.size()));

  std::partial_sort_copy(students.begin(), students.end(),
      result.begin(), result.end(),
      CompareStudentsByGrade{});

  return result;
}

std::vector< std::shared_ptr< const gavrilova::student::Student > >
gavrilova::StudentDatabase::getRiskStudentsInGroup(const std::string& groupName, double threshold) const
{
  auto students = getStudentsInGroup(groupName);
  std::vector< std::shared_ptr< const student::Student > > result;
  std::copy_if(students.begin(), students.end(),
      std::back_inserter(result),
      IsRiskStudentInGroupPredicate{threshold});

  return result;
}

std::pair< bool, double > gavrilova::StudentDatabase::getAverageGradeByDate(const date::Date& date) const
{
  auto it = dateToGradesIndex.find(date);
  if (it == dateToGradesIndex.end()) return {false, 0.0};

  const auto& grades = it->second;
  double sum = std::accumulate(grades.begin(), grades.end(), 0.0,
      GradePairAccumulator{});

  return {true, sum / grades.size()};
}
