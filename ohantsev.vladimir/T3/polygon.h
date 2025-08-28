#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include <iosfwd>

namespace ohantsev
{
  struct Point
  {
    int x, y;
    bool operator<(const Point& rhs) const noexcept;
    bool operator==(const Point& rhs) const noexcept;
  };

  struct Polygon
  {
    std::vector< Point > points;
    std::size_t size() const noexcept;
    bool operator==(const Polygon& rhs) const noexcept;
  };

  double getArea(const Polygon& polygon);
  int areaGaussPairSum(const Point& lhs, const Point& rhs);
  std::istream& operator>>(std::istream& in, Point& rhs);
  std::istream& operator>>(std::istream& in, Polygon& rhs);
}
#endif
