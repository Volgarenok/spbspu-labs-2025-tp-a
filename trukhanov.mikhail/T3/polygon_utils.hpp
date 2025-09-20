#ifndef POLYGON_UTILS_HPP
#define POLYGON_UTILS_HPP

#include <cstddef>
#include <algorithm>
#include "polygon.hpp"

namespace trukhanov
{
  struct isSize
  {
    size_t size;
    isSize(size_t s);
    bool operator()(const Polygon& p) const;
  };

  bool CompareByArea(const Polygon& lhs, const Polygon& rhs);

  struct LessArea
  {
    double area;
    bool operator()(const Polygon& p) const;
  };

  bool compareByVertexes(const Polygon& lhs, const Polygon& rhs);

  bool HasDuplicates(const Polygon& p);

  bool PolygonHasMinSize(const trukhanov::Polygon& p);

  double VectorProduct(const Point& a, const Point& b, const Point& c);

  struct MakeAngles
  {
    const std::vector< Point >& pts;
    Angle operator()(std::size_t i) const;
  };

  struct HasRightAngle
  {
    explicit HasRightAngle(const std::vector< Point >& pts):
      pts_(pts)
    {}
    bool operator()(const Angle& ang) const;
    const std::vector< Point >& pts_;
  };

  struct CommonVertex
  {
    const std::vector< Polygon >& polygons;
    size_t i;

    CommonVertex(const std::vector< Polygon >& polys, size_t idx):
      polygons(polys), i(idx)
    {}
    bool operator()(size_t j) const;
  };

  struct CheckAllPairs
  {
    const std::vector< Polygon >& polygons;

    CheckAllPairs(const std::vector< Polygon >& polys):
      polygons(polys)
    {}
    bool operator()(size_t n) const;
  };

  bool hasCommonPoint(const std::vector< Polygon >& polygons);

  std::pair< Point, Point > normalizeSide(const std::pair< Point, Point >& side);

  std::pair< Point, Point > makeSidePair(const Point& a, const Point& b);

  struct CollectSides
  {
    const std::vector< Point >& pts;
    std::vector< std::pair< Point, Point > > operator()() const;
  };

  struct CommonSide
  {
    const std::vector< Polygon >& polygons;
    size_t i;
    CommonSide(const std::vector< Polygon >& polygons_, std::size_t i_):
      polygons(polygons_), i(i_)
    {}
    bool operator()(size_t j) const;
  };

  struct CheckAllSides
  {
    const std::vector< Polygon >& polygons;
    CheckAllSides(const std::vector< Polygon >& ps) : polygons(ps) {}
    bool operator()(size_t n) const;
  };
}

#endif
