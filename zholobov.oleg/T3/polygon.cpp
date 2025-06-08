#include "polygon.hpp"

#include <algorithm>
#include <istream>
#include <iterator>

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
      std::vector< Point > points;
      std::copy_n(std::istream_iterator< Point >(input), count, std::back_inserter(points));

      if (!input) {
        input.setstate(std::ios::failbit);
      } else {
        polygon.points = std::move(points);
      }
    }
    return input;
  }

}
