#ifndef DATE_HPP
#define DATE_HPP

#include <iostream>
#include <string>

namespace gavrilova {
  namespace date {
    struct Date {
      int year;
      int month;
      int day;
    };

    bool operator<(const Date& a, const Date& b);
    bool operator==(const Date& a, const Date& b);
    std::istream& operator>>(std::istream& is, Date& d);
    std::ostream& operator<<(std::ostream& os, const Date& d);
    std::string to_string(const Date& d);
  }
}

#endif
