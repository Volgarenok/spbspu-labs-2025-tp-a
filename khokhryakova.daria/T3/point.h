#ifndef POINT_H
#define POINT_H

#include <iostream>

namespace khokhryakova
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::ios& stream);
    ~StreamGuard();
  private:
    std::ios& stream_;
    std::ios::fmtflags flags_;
  };

  struct Point
  {
    int x;
    int y;
    bool operator==(const Point& other) const;
  };

  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, Point& p);
  std::ostream& operator<<(std::ostream& out, const Point& p);
}

#endif
