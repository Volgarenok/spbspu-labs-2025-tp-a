#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include "polygon.hpp"
#include <functional>
#include <vector>
#include <string>

namespace puzikov
{
  struct AreaAccumulator
  {
    AreaAccumulator(const std::string &p);
    double operator()(double acc, const puzikov::Polygon &poly) const;
    const std::string &param;
  };

  bool VerticesComparator(const Polygon &p1, const Polygon &p2);
  bool AreaComparator(const Polygon &p1, const Polygon &p2);

  struct ShapesAccumulator
  {
    ShapesAccumulator(const std::string &p);
    double operator()(double acc, const puzikov::Polygon &poly) const;
    const std::string &param;
  };

  struct RmEchoPredicate
  {
    RmEchoPredicate(const Polygon &poly);
    bool operator()(const Polygon &p1, const Polygon &p2);
    const Polygon &ref;
  };

  struct TranslatePoint
  {
    TranslatePoint(int dx_, int dy_);
    Point operator()(const Point &p) const;
    int dx, dy;
  };

  struct AnyOfShift
  {
    using PointVec = std::vector< Point >;
    AnyOfShift(const PointVec &candidate_, const PointVec &reference_);
    bool operator()(std::size_t shift) const;
    const PointVec &candidate;
    const PointVec &reference;
  };

  struct IsTranslationCongruent
  {
    IsTranslationCongruent(const Polygon &reference_);
    bool operator()(const Polygon &poly) const;
    const Polygon &reference;
  };

  Point PointGenerator(std::istream &);

  using AreaComp = bool (*)(const Polygon &, const Polygon &);
  using VertComp = bool (*)(const Polygon &, const Polygon &);
  using AreaIt = std::vector< Polygon >::const_iterator;
  using AreaAlgo = AreaIt (*)(AreaIt, AreaIt, AreaComp);
  using VertIt = std::vector< Polygon >::const_iterator;
  using VertAlgo = VertIt (*)(VertIt, VertIt, VertComp);

  AreaIt maxAreaElement(AreaIt first, AreaIt last, AreaComp comp);
  AreaIt minAreaElement(AreaIt first, AreaIt last, AreaComp comp);
  VertIt maxVertElement(VertIt first, VertIt last, VertComp comp);
  VertIt minVertElement(VertIt first, VertIt last, VertComp comp);
}

#endif
