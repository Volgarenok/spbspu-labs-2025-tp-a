#include "Student.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <string>
#include "Date.hpp"

namespace {
  struct DateGradeReader {
    std::istream& is;
    std::map< gavrilova::date::Date, int >& grades;

    DateGradeReader(std::istream& is_, std::map< gavrilova::date::Date, int >& g):
      is(is_),
      grades(g)
    {}

    void operator()(int)
    {
      gavrilova::date::Date d;
      int grade;
      if (is >> d >> grade) {
        grades[d] = grade;
      } else {
        is.setstate(std::ios::failbit);
      }
    }
  };

  struct DateGradeInserter {
    std::istream& is;
    std::map< gavrilova::date::Date, int >& grades;

    DateGradeInserter(std::istream& is_, std::map< gavrilova::date::Date, int >& g):
      is(is_),
      grades(g)
    {}

    void operator()(const gavrilova::date::Date& d)
    {
      int grade;
      is >> grade;
      grades[d] = grade;
    }
  };

  struct DateGradePrinter {
    std::ostream& os;
    DateGradePrinter(std::ostream& os_):
      os(os_)
    {}

    void operator()(const std::pair< const gavrilova::date::Date, int >& p) const
    {
      os << p.first << " " << p.second << " ";
    }
  };

  struct GradeValueExtractor {
    double operator()(double sum, const std::pair< gavrilova::date::Date, int >& p) const
    {
      return sum + p.second;
    }
  };

}

double gavrilova::student::calcAverage(const gavrilova::student::Student& s)
{
  const auto& grades = s.grades_;
  if (grades.empty()) {
    return 0.0;
  }
  double total = std::accumulate(grades.begin(), grades.end(), 0.0, GradeValueExtractor{});
  return total / grades.size();
}

std::istream& gavrilova::student::operator>>(std::istream& is, Student& s)
{
  std::string fullName;
  std::string group;

  std::istream::sentry sentry(is);
  if (!sentry) {
    return is;
  }

  is >> std::quoted(fullName) >> group;
  s.fullName_ = std::move(fullName);
  s.group_ = group;

  std::string tmp;
  std::getline(is, tmp);

  is.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

  size_t n = 0;
  if (!(is >> tmp >> n)) {
    return is;
  }
  is.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

  std::map< date::Date, int > grades;
  std::vector< int > indices(n);
  std::iota(indices.begin(), indices.end(), 0);
  std::for_each(indices.begin(), indices.end(), DateGradeReader(is, grades));

  if (grades.size() == n) {
    is.clear();
    is.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  s.grades_ = std::move(grades);

  return is;
}

std::ostream& gavrilova::student::operator<<(std::ostream& os, const Student& s)
{
  std::ostream::sentry sentry(os);
  if (!sentry) {
    return os;
  }
  os << std::quoted(s.fullName_) << " " << s.group_ << "\n";
  os << "Средний балл: " << calcAverage(s) << "\n";
  os << "Оценки " << s.grades_.size() << "\n";
  std::for_each(s.grades_.begin(), s.grades_.end(), DateGradePrinter(os));
  os << "\n";

  return os;
}
