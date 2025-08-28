#ifndef STUDENT_DATABASE_HPP
#define STUDENT_DATABASE_HPP

#include <cmath>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>
#include "Date.hpp"
#include "Student.hpp"

namespace gavrilova {

  struct DateRange {
    date::Date start;
    date::Date end;
  };

  struct GroupStatistics {
    std::map< int, int > gradeDistribution;
    std::vector< std::shared_ptr< const student::Student > > topStudents;
    std::vector< std::shared_ptr< const student::Student > > bottomStudents;
    double groupAverage = 0.0;
    double allOtherGroupsAverage = 0.0;
  };

  class StudentDatabase {
  public:
    using Group = std::map< StudentID, std::shared_ptr< student::Student > >;

    explicit StudentDatabase(int id_digits = 4);

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    void clear();

    bool createGroup(const std::string& groupName);
    bool groupExists(const std::string& groupName) const;

    std::pair< bool, StudentID > addStudent(const std::string& fullName, const std::string& groupName);
    bool deleteStudent(StudentID id);
    bool moveStudentToGroup(StudentID id, const std::string& newGroupName);

    std::shared_ptr< const student::Student > findStudentById(StudentID id) const;
    std::vector< StudentID > findStudentsByName(const std::string& fullName) const;

    bool addGrade(StudentID id, int grade, const date::Date& date);
    bool changeGrade(StudentID id, int newGrade, const date::Date& date);
    bool removeGradesByDate(StudentID id, const date::Date& date);

    std::vector< std::shared_ptr< const student::Student > > getStudentsInGroup(const std::string& groupName) const;
    std::vector< std::shared_ptr< const student::Student > > getTopStudents(size_t n) const;
    std::vector< std::shared_ptr< const student::Student > > getRiskStudents(double threshold) const;
    std::vector< std::shared_ptr< const student::Student > > getTopStudentsInGroup(const std::string& groupName, size_t n) const;
    std::vector< std::shared_ptr< const student::Student > > getRiskStudentsInGroup(const std::string& groupName, double threshold) const;
    std::pair< bool, double > getAverageGradeByDate(const date::Date& date) const;

    bool loadGradesFromFile(const std::string& groupName, const date::Date& date, const std::string& filename);
    bool exportGroupForGrading(const std::string& groupName, const std::string& filename) const;
    std::pair< bool, GroupStatistics > getGroupStatistics(const std::string& groupName, const DateRange& period) const;

    std::shared_ptr< student::Student > findStudentById(StudentID id);
    void updateStudentAverageGrade(std::shared_ptr< student::Student >& student);

  private:
    std::map< StudentID, std::shared_ptr< student::Student > > students;
    std::map< std::string, Group > groups;
    std::map< std::string, std::set< StudentID > > nameToStudentIndex;
    std::map< date::Date, std::vector< std::pair< StudentID, int > > > dateToGradesIndex;
    StudentID nextId;
  };
}

#endif
