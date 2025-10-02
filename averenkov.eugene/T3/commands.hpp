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
  double acum(const std::vector< Polygon >& polygons, std::function< double(double accumulator, const Polygon& polygon) > func);

  struct Prod
  {
    double operator()(const averenkov::Point& a, const averenkov::Point& b) const;
  };

  struct VertexCount
  {
    VertexCount(size_t num);
    bool operator()(const Polygon& poly) const;

  private:
    size_t num_;
  };

  struct AreaComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  struct VertexCountComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  struct AreaSumCalculator
  {
    double operator()(double sum, const Polygon& poly) const;
    double operator()(const std::vector< Polygon >& polygons) const;
  };

  struct EvenSumCalculator
  {
    double operator()(double sum, const Polygon& poly) const;
    double operator()(const std::vector< Polygon >& polygons) const;
  };

  struct OddSumCalculator
  {
    double operator()(double sum, const Polygon& poly) const;
    double operator()(const std::vector< Polygon >& polygons) const;
  };

  struct IsEven
  {
    bool operator()(const Polygon& poly) const;
  };

  struct IsOdd
  {
    bool operator()(const Polygon& poly) const;
  };

  struct NumVertexCalculator
  {
    size_t num;
    double operator()(double sum, const Polygon& poly) const;
    double operator()(const std::vector< Polygon >& polygons) const;
 };

  struct MeanAreaCalculator
  {
    double operator()(const std::vector< Polygon >& polygons) const;
  };

  void printMaxArea(const std::vector< Polygon >& polygons, std::ostream& out);

  void printMaxVertexCount(const std::vector< Polygon >& polygons, std::ostream& out);

  void printMinArea(const std::vector< Polygon >& polygons, std::ostream& out);

  void printMinVertexCount(const std::vector< Polygon >& polygons, std::ostream& out);

  struct MaxAreaFinder
  {
  public:
    explicit MaxAreaFinder(const std::vector< Polygon >& polygons);
    friend std::ostream& operator<<(std::ostream& out, const MaxAreaFinder& finder);
  private:
    const std::vector< Polygon >& polygons_;
  };

  std::ostream& operator<<(std::ostream& out, const MaxAreaFinder& finder);

  struct MaxVertexCountFinder
  {
  public:
    explicit MaxVertexCountFinder(const std::vector< Polygon >& polygons);
    friend std::ostream& operator<<(std::ostream& out, const MaxVertexCountFinder& finder);
  private:
    const std::vector< Polygon >& polygons_;
  };

  std::ostream& operator<<(std::ostream& out, const MaxVertexCountFinder& finder);

  struct MinAreaFinder
  {
  public:
    explicit MinAreaFinder(const std::vector< Polygon >& polygons);
    friend std::ostream& operator<<(std::ostream& out, const MinAreaFinder& finder);
  private:
    const std::vector< Polygon >& polygons_;
  };

  std::ostream& operator<<(std::ostream& out, const MinAreaFinder& finder);

  struct MinVertexCountFinder
  {
  public:
    explicit MinVertexCountFinder(const std::vector< Polygon >& polygons);
    friend std::ostream& operator<<(std::ostream& out, const MinVertexCountFinder& finder);
  private:
    const std::vector< Polygon >& polygons_;
  };

  std::ostream& operator<<(std::ostream& out, const MinVertexCountFinder& finder);

  struct EvenCounter
  {
    size_t operator()(const std::vector< Polygon >& polygons) const;
  };

  struct OddCounter
  {
    size_t operator()(const std::vector< Polygon >& polygons) const;
  };

  struct NumVertexCounter
  {
    size_t num;
    size_t operator()(const std::vector< Polygon >& polygons) const;
  };

  struct AngleCheckHelper
  {
    const std::vector< Point >& pts;
    size_t idx;
    AngleCheckHelper(const std::vector< Point >& points);
    bool operator()(const Point&);
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
