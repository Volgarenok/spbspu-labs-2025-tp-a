#ifndef PREDICATES_HPP
#define PREDICATES_HPP

#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>
#include "shapes.hpp"

namespace sharifullina
{
  namespace detail
  {
    double calcPoints(const Point & p1, const Point & p2);

    bool cmpPoint(const Point & p1, const Point & p2);

    struct IsPermutationPredicate
    {
      const Polygon & target;
      explicit IsPermutationPredicate(const Polygon & t);
      bool operator()(const Polygon & poly) const;
    };

    struct LessAreaPredicate
    {
      double targetArea;
      explicit LessAreaPredicate(double area);
      bool operator()(const Polygon & poly) const;
    };
  }
}

#endif
