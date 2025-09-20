#ifndef BASE_TYPES_HPP
#define BASE_TYPES_HPP
#include <iostream>

namespace savintsev
{
  struct point_t
  {
    double x;
    double y;
  };

  std::istream & operator>>(std::istream & in, point_t & dest);
  std::ostream & operator<<(std::ostream & out, const point_t & dest);

  struct rectangle_t
  {
    double width;
    double height;
    point_t pos;
  };
}
#endif
