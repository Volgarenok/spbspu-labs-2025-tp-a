#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP

#include <ios>
#include "geometry.hpp"

namespace aleksandrov
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream&, DelimiterIO&&);
  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Polygon&);
  std::ostream& operator<<(std::ostream&, const Point&);
  std::ostream& operator<<(std::ostream&, const Polygon&);
}

#endif

