#ifndef IO_POLYGON_HPP
#define IO_POLYGON_HPP

#include "geometric_shapes.hpp"
#include <iostream>

namespace petrov
{
  struct DelimeterIO
  {
    char exp;
  };

  struct StreamGuard
  {
    explicit StreamGuard(std::basic_ios< char > & s);
    ~StreamGuard();
  private:
    std::basic_ios< char > & s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  std::istream & operator>>(std::istream & in, DelimeterIO && dest);
  std::istream & operator>>(std::istream & in, Polygon & polygon);
  std::ostream & operator<<(std::ostream & in, Polygon & polygon);
}

#endif
