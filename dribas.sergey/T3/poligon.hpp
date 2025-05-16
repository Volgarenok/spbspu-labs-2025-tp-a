#ifndef POLIGON_HPP
#define POLIGON_HPP

#include <vector>
#include <istream>

namespace dribas
{
  struct Point
  {
    int x;
    int y;
  };

  struct Poligon
  {
    std::vector< Point > points;
  };

  bool operator==(const Point&, const Point&);
  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Poligon&);
  double getPoligonArea(const Poligon&);
}

#endif
