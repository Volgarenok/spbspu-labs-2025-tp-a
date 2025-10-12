#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include <map>
#include <vector>
#include <string>
#include "polygon.hpp"

namespace puzikov
{
  struct AreaAccumulator
  {
    AreaAccumulator(const std::string &p);
    double operator()(double acc, const puzikov::Polygon &poly) const;

  private:
    const std::string &param;
    std::map< std::string, double (*)(const Polygon &poly) > commands;
    static double evenSubcommand(const Polygon &poly);
    static double oddSubcommand(const Polygon &poly);
    static double meanSubcommand(const Polygon &poly);
  };

  bool VerticesComparator(const Polygon &p1, const Polygon &p2);
  bool AreaComparator(const Polygon &p1, const Polygon &p2);

  struct ShapesAccumulator
  {
    ShapesAccumulator(const std::string &p);
    double operator()(double acc, const puzikov::Polygon &poly) const;

  private:
    const std::string &param;
    std::map< std::string, double (*)(const Polygon &poly) > commands;
    static double evenSubcommand(const Polygon &poly);
    static double oddSubcommand(const Polygon &poly);
  };

  struct RmEchoPredicate
  {
    RmEchoPredicate(const Polygon &poly);
    bool operator()(const Polygon &p1, const Polygon &p2);
    const Polygon &ref;
  };

  struct TranslatePoint
  {
    TranslatePoint(int dx, int dy);
    Point operator()(const Point &p) const;
    int dx_, dy_;
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

  using comparator = bool(*)(const Polygon &, const Polygon &);
  using constPolygonVecIt = std::vector< Polygon >::const_iterator;
  using AreaAlgo = constPolygonVecIt(*)(constPolygonVecIt, constPolygonVecIt, comparator);
  using VertAlgo = constPolygonVecIt(*)(constPolygonVecIt, constPolygonVecIt, comparator);

  constPolygonVecIt maxAreaElement(constPolygonVecIt first, constPolygonVecIt last, comparator comp);
  constPolygonVecIt minAreaElement(constPolygonVecIt first, constPolygonVecIt last, comparator comp);
  constPolygonVecIt maxVertElement(constPolygonVecIt first, constPolygonVecIt last, comparator comp);
  constPolygonVecIt minVertElement(constPolygonVecIt first, constPolygonVecIt last, comparator comp);
}

#endif
