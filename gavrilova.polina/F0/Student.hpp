#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Date.hpp"

namespace gavrilova {
  using StudentID = unsigned long;

  namespace student {
    struct Student {
      StudentID id_;
      std::string fullName_;
      std::string group_;
      std::map< gavrilova::date::Date, int > grades_;
      double averageGrade_;

      Student():
        id_(0),
        fullName_(""),
        group_(""),
        averageGrade_(0.0)
      {}

      Student(StudentID id, const std::string& fullName, const std::string& group):
        id_(id),
        fullName_(fullName),
        group_(group),
        grades_(),
        averageGrade_(0.0)
      {}
    };

    double calcAverage(const Student& s);

    std::ostream& operator<<(std::ostream& os, const Student& student);
    std::istream& operator>>(std::istream& is, Student& student);
  }
}

#endif
