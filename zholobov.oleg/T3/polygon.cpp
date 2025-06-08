#include "polygon.hpp"

#include <istream>

namespace zholobov {

  namespace io_helpers {

    struct CharIO {
      char c;
    };

    std::istream& operator>>(std::istream& input, CharIO&& value)
    {
      std::istream::sentry s(input);
      if (!s) {
        return input;
      }
      char c = 0;
      if ((input >> c) && (c != value.c)) {
        input.setstate(std::ios::failbit);
      }
      return input;
    }

  }

  std::istream& operator>>(std::istream& input, Point& point)
  {
    std::istream::sentry s(input);
    if (!s) {
      return input;
    }
    int x = 0;
    int y = 0;
    if ((input >> io_helpers::CharIO{'('} >> x >> io_helpers::CharIO{';'} >> y >> io_helpers::CharIO{')'})) {
      point.x = x;
      point.y = y;
    }
    return input;
  }

  std::istream& operator>>(std::istream& input, Polygon& polygon)
  {
    std::istream::sentry s(input);
    if (!s) {
      return input;
    }
    size_t count = 0;
    if (input >> count) {
      for (size_t i = 0; i < count; ++i) {
        Point p;
        if (!(input >> io_helpers::CharIO{' '} >> p)) {
          input.setstate(std::ios::failbit);
          return input;
        }
        polygon.points.push_back(p);
      }
    }
    return input;
  }

}
