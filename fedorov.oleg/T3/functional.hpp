#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

#include "polygon.hpp"
#include "input_delimiter.hpp"

namespace fedorov
{
  struct LineValidator
  {
    bool operator()(const Line &line) const;
  };

  struct LineToPolygonConverter
  {
    Polygon operator()(const Line &line) const;
  };

  struct PolygonValidator
  {
    bool operator()(const Polygon &poly) const;
  };

  struct AreaComparator
  {
    bool operator()(const Polygon &p1, const Polygon &p2) const;
  };

  struct VerticesComparator
  {
    bool operator()(const Polygon &p1, const Polygon &p2) const;
  };

  struct AreaAccumulator
  {
    explicit AreaAccumulator(const std::string &param);
    double operator()(double acc, const Polygon &poly) const;
    const std::string &param;
  };

  struct CountAccumulator
  {
    explicit CountAccumulator(const std::string &param);
    size_t operator()(size_t cnt, const Polygon &poly) const;
    const std::string &param;
  };

  struct LessAreaPredicate
  {
    explicit LessAreaPredicate(double threshold);
    bool operator()(const Polygon &poly) const;
    double threshold;
  };

  struct EchoHandler
  {
    explicit EchoHandler(const Polygon &ref);
    void operator()(std::vector< Polygon > &result, const Polygon &poly) const;
    const Polygon &reference;
    mutable size_t count;
  };

  double calcPolygonArea(const Polygon &poly);
  void readPolygons(std::istream &in, std::vector< Polygon > &polygons);
  size_t countIf(const std::vector< Polygon > &polys, const LessAreaPredicate &pred);
  void processEcho(std::vector< Polygon > &polys, const Polygon &ref);
}

#endif
