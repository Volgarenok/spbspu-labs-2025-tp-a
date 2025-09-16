#include "Point.hpp"
#include <istream>
#include <ostream>
#include <tuple>
#include <IOStreamGuard.hpp>

namespace gavrilova {
  bool operator==(const Point& a, const Point& b)
  {
    return a.x == b.x && a.y == b.y;
  }

  bool operator<(const Point& a, const Point& b)
  {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
  }

  std::istream& operator>>(std::istream& is, Point& point)
  {
    std::istream::sentry sentry(is);
    if (!sentry) {
      return is;
    }
    char open_bracket = 0, separator = 0, close_bracket = 0;
    int x = 0, y = 0;
    is >> open_bracket >> x >> separator >> y >> close_bracket;
    if (is && open_bracket == '(' && separator == ';' && close_bracket == ')') {
      point = {x, y};
    } else {
      is.setstate(std::ios::failbit);
    }
    return is;
  }

  std::ostream& operator<<(std::ostream& os, const Point& point)
  {
    std::ostream::sentry sentry(os);
    if (!sentry) {
      return os;
    }
    IOStreamGuard guard(os);
    os << "(" << point.x << ";" << point.y << ")";
    return os;
  }
}
