#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP
#include "GeometricalTypes.hpp"

namespace shapkov
{
  struct isSize
  {
    size_t size;
    bool operator()(const Polygon& p) const;
  };
  bool compareDouble(double d1, double d2, double epsilon = 1e-9);
  struct isSame
  {
    Polygon& p;
    bool operator()(const Polygon& src) const;
  };
  struct pointDeltaSum
  {
    double operator()(const Point& a, const Point& b) const;
  };
}

#endif
