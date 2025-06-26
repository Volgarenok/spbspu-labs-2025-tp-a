#include <iostream>
#include <functional>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <map>
#include <dataStruct.hpp>
#include "polygon.hpp"

namespace averenkov
{

  struct VertexCount
  {
    VertexCount(size_t num);
    bool operator()(const Polygon& poly) const;

  private:
    size_t num_;
  };

  struct AreaSumCalculator
  {
    double operator()(const std::vector<Polygon>& polygons) const;
  };

  struct EvenAreaSumCalculator
  {
    double operator()(const std::vector<Polygon>& polygons) const;
  };

  struct OddAreaSumCalculator
  {
    double operator()(const std::vector<Polygon>& polygons) const;
  };

  struct NumVertexAreaSumCalculator
  {
    size_t num;
    double operator()(const std::vector<Polygon>& polygons) const;
  };

  struct MeanAreaCalculator
  {
    double operator()(const std::vector<Polygon>& polygons) const;
  };

  struct MaxAreaFinder
  {
    void operator()(const std::vector<Polygon>& polygons, std::ostream& out) const;
  };

  struct MaxVertexCountFinder
  {
    void operator()(const std::vector<Polygon>& polygons, std::ostream& out) const;
  };

  struct MinAreaFinder
  {
    void operator()(const std::vector<Polygon>& polygons, std::ostream& out) const;
  };

  struct MinVertexCountFinder
  {
    void operator()(const std::vector<Polygon>& polygons, std::ostream& out) const;
  };

  struct EvenCounter
  {
    size_t operator()(const std::vector<Polygon>& polygons) const;
  };

  struct OddCounter
  {
    size_t operator()(const std::vector<Polygon>& polygons) const;
  };

  struct NumVertexCounter
  {
    size_t num;
    size_t operator()(const std::vector<Polygon>& polygons) const;
  };

  struct RightAngleChecker
  {
    bool operator()(const Polygon& poly) const;
  };

  struct PolygonEqual
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  double calculateArea(const Polygon& poly);
  void printAreaSum(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printMaxValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printMinValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printCountOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printPermsCnt(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printRightsCnt(const std::vector< Polygon >& polygons, std::ostream& out);
  void printRmEcho(std::istream& in, std::vector< Polygon >& polygons, std::ostream& out);
}
