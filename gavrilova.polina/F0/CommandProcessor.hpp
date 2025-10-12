#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include "StudentsDataBase.hpp"

namespace gavrilova {

  class CommandProcessor {
  public:
    explicit CommandProcessor(StudentDatabase& db);
    void run();

  private:
    StudentDatabase& db_;
    std::map< std::string, std::function< void() > > commandMap_;

    void handleHelp();
    void handleAddStudent();
    void handleDeleteStudent();
    void handleInfoStudent();
    void handleListGroup();
    void handleListGrades();
    void handleMoveGroup();
    void handleAddGrade();
    void handleChangeGrade();
    void handleRemoveGrade();
    void handleTopStudentsAll();
    void handleRiskStudentsAll();
    void handleTopStudentsGroup();
    void handleRiskStudentsGroup();
    void handleExportForGrades();
    void handleLoadGrades();
    void handleGroupStats();
    void handleAvgMarkByDate();
    void handleClear();
    void handleCreateGroup();
    void handleSave();
    void handleLoad();
  };

}

#endif
