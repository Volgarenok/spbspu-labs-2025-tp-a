#include "Point.hpp"
#include <tuple>

namespace {

  constexpr char separator = ';';
  constexpr char lhs_divider = '(';
  constexpr char rhs_divider = ')';

}

namespace gavrilova {

  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  bool Point::operator<(const Point& other) const
  {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }

  std::ostream& operator<<(std::ostream& os, const Point& point)
  {
    std::ostream::sentry sentry(os);
    if (!sentry) {
      return os;
    }

    os << lhs_divider << point.x << separator << point.y << rhs_divider;
    return os;
  }

  std::istream& operator>>(std::istream& is, Point& point)
  {
    std::istream::sentry sentry(is);
    if (!sentry) {
      return is;
    }

    char separator_char = ' ';
    char l_div, r_div = ' ';
    int new_x = 0;
    int new_y = 0;

    if (!((is >> l_div) && (is >> new_x) && (is >> separator_char) && (is >> new_y) && (is >> r_div))) {
      is.setstate(std::ios::failbit);
      return is;
    }

    if (!(l_div == lhs_divider && separator_char == separator && r_div == rhs_divider)) {
      is.setstate(std::ios::failbit);
      return is;
    }

    point.x = new_x;
    point.y = new_y;

    return is;
  }
}
