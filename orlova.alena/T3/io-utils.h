#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <iostream>
#include "geom.h"

namespace orlova
{
  struct DelimiterIO
  {
    char exp;
  };

  class IoGuard
  {
  public:
    explicit IoGuard(std::basic_ios< char >& s);
    ~IoGuard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  std::istream& operator>>(std::istream&, DelimiterIO&&);
  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Polygon&);
  std::ostream& operator<<(std::ostream&, const Point&);
  std::ostream& operator<<(std::ostream&, const Polygon&);
  bool operator==(const Point&, const Point&);
  bool operator==(const Polygon&, const Polygon&);
}

#endif
