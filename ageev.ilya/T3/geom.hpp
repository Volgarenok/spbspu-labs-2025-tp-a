#ifndef GEOM_HPP
#define GEOM_HPP

#include <istream>
#include <ostream>
#include <vector>

namespace ageev
{
  struct Point
  {
    int x;
    int y;
    bool operator==(const Point& other) const;
  };

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& other) const;
  };

  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  std::ostream& operator<<(std::ostream& out, const Point& point);
  std::ostream& operator<<(std::ostream& out, const Polygon& polygon);
  
  double get_area(const Polygon& polygon);
  bool has_right_angle(const Polygon& polygon);
}

#endif