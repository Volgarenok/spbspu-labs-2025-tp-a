#include "commands.hpp"
#include "inframe.hpp"
#include "lessarea.hpp"
#include "utils.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>
#include <functional>
#include <numeric>
#include <iterator>
namespace {
struct IsEvenPolygon { bool operator()(const guseynov::Polygon& poly) const { return poly.points.size() % 2 == 0; } };
struct IsOddPolygon { bool operator()(const guseynov::Polygon& poly) const { return poly.points.size() % 2 != 0; } };
struct HasVertexCount {
  explicit HasVertexCount(size_t targetCount) : target(targetCount) {}
  bool operator()(const guseynov::Polygon& poly) const { return poly.points.size() == target; }
  size_t target;
};
struct HasLessArea {
  explicit HasLessArea(double areaThreshold) : threshold(areaThreshold) {}
  bool operator()(const guseynov::Polygon& poly) const { return guseynov::utils::calculateArea(poly) < threshold; }
  double threshold;
};
struct AreaAccumulator {
  double operator()(double sum, const guseynov::Polygon& poly) const { return sum + guseynov::utils::calculateArea(poly); }
};
struct ConditionalAreaAccumulatorEven {
  double operator()(double sum, const guseynov::Polygon& poly) const { return IsEvenPolygon()(poly) ? sum + guseynov::utils::calculateArea(poly) : sum; }
};
struct ConditionalAreaAccumulatorOdd {
  double operator()(double sum, const guseynov::Polygon& poly) const { return IsOddPolygon()(poly) ? sum + guseynov::utils::calculateArea(poly) : sum; }
};
struct ConditionalAreaAccumulatorVertexCount {
  explicit ConditionalAreaAccumulatorVertexCount(size_t target) : target_(target) {}
  double operator()(double sum, const guseynov::Polygon& poly) const { return HasVertexCount(target_)(poly) ? sum + guseynov::utils::calculateArea(poly) : sum; }
  size_t target_;
};
struct MaxAreaFinder {
  double maxArea = 0.0;
  void operator()(const guseynov::Polygon& poly) {
    double area = guseynov::utils::calculateArea(poly);
    if (area > maxArea) maxArea = area;
  }
};
struct MinAreaFinder {
  double minArea = std::numeric_limits<double>::max();
  void operator()(const guseynov::Polygon& poly) {
    double area = guseynov::utils::calculateArea(poly);
    if (area < minArea) minArea = area;
  }
};
struct MaxVerticesFinder {
  size_t maxVertices = 0;
  void operator()(const guseynov::Polygon& poly) {
    size_t vertices = poly.points.size();
    if (vertices > maxVertices) maxVertices = vertices;
  }
};
struct MinVerticesFinder {
  size_t minVertices = std::numeric_limits<size_t>::max();
  void operator()(const guseynov::Polygon& poly) {
    size_t vertices = poly.points.size();
    if (vertices < minVertices) minVertices = vertices;
  }
};
struct FrameBoundsAccumulator {
  int minX = std::numeric_limits<int>::max();
  int maxX = std::numeric_limits<int>::min();
  int minY = std::numeric_limits<int>::max();
  int maxY = std::numeric_limits<int>::min();
  void operator()(const guseynov::Polygon& poly) {
    for (const auto& point : poly.points) {
      minX = std::min(minX, point.x);
      maxX = std::max(maxX, point.x);
      minY = std::min(minY, point.y);
      maxY = std::max(maxY, point.y);
    }
  }
};
struct PointOutsideFrame {
  int minX, maxX, minY, maxY;
  PointOutsideFrame(int minX_, int maxX_, int minY_, int maxY_) : minX(minX_), maxX(maxX_), minY(minY_), maxY(maxY_) {}
  bool operator()(const guseynov::Point& point) const {
    return point.x < minX || point.x > maxX || point.y < minY || point.y > maxY;
  }
};
}
void guseynov::commands::handleAreaCommand(const std::vector<Polygon>& polygons, const std::string& param) {
  if (param == "EVEN") {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, ConditionalAreaAccumulatorEven());
    std::cout << std::fixed << std::setprecision(1) << sum << '\n';
  } else if (param == "ODD") {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, ConditionalAreaAccumulatorOdd());
    std::cout << std::fixed << std::setprecision(1) << sum << '\n';
  } else if (param == "MEAN") {
    if (polygons.empty()) { std::cout << "<INVALID COMMAND>\n"; return; }
    double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaAccumulator());
    std::cout << std::fixed << std::setprecision(1) << total / polygons.size() << '\n';
  } else if (guseynov::utils::isNumber(param)) {
    size_t target = std::stoul(param);
    if (target < 3) { std::cout << "<INVALID COMMAND>\n"; return; }
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, ConditionalAreaAccumulatorVertexCount(target));
    std::cout << std::fixed << std::setprecision(1) << sum << '\n';
  } else { std::cout << "<INVALID COMMAND>\n"; }
}
void guseynov::commands::handleMaxCommand(const std::vector<Polygon>& polygons, const std::string& param) {
  if (polygons.empty()) { std::cout << "<INVALID COMMAND>\n"; return; }
  if (param == "AREA") {
    MaxAreaFinder finder;
    for (const auto& poly : polygons) finder(poly);
    std::cout << std::fixed << std::setprecision(1) << finder.maxArea << '\n';
  } else if (param == "VERTEXES") {
    MaxVerticesFinder finder;
    for (const auto& poly : polygons) finder(poly);
    std::cout << finder.maxVertices << '\n';
  } else { std::cout << "<INVALID COMMAND>\n"; }
}
void guseynov::commands::handleMinCommand(const std::vector<Polygon>& polygons, const std::string& param) {
  if (polygons.empty()) { std::cout << "<INVALID COMMAND>\n"; return; }
  if (param == "AREA") {
    MinAreaFinder finder;
    for (const auto& poly : polygons) finder(poly);
    std::cout << std::fixed << std::setprecision(1) << finder.minArea << '\n';
  } else if (param == "VERTEXES") {
    MinVerticesFinder finder;
    for (const auto& poly : polygons) finder(poly);
    std::cout << finder.minVertices << '\n';
  } else { std::cout << "<INVALID COMMAND>\n"; }
}
void guseynov::commands::handleCountCommand(const std::vector<Polygon>& polygons, const std::string& param) {
  if (param == "EVEN") {
    size_t count = std::count_if(polygons.begin(), polygons.end(), IsEvenPolygon());
    std::cout << count << '\n';
  } else if (param == "ODD") {
    size_t count = std::count_if(polygons.begin(), polygons.end(), IsOddPolygon());
    std::cout << count << '\n';
  } else if (guseynov::utils::isNumber(param)) {
    size_t target = std::stoul(param);
    if (target < 3) { std::cout << "<INVALID COMMAND>\n"; return; }
    size_t count = std::count_if(polygons.begin(), polygons.end(), HasVertexCount(target));
    std::cout << count << '\n';
  } else { std::cout << "<INVALID COMMAND>\n"; }
}
void guseynov::commands::handleInFrameCommand(const std::vector<Polygon>& polygons, const std::string& param) {
  if (polygons.empty()) { std::cout << "<FALSE>\n"; return; }
  Polygon targetPoly = guseynov::utils::parsePolygon(param);
  if (targetPoly.points.size() < 3) { std::cout << "<INVALID COMMAND>\n"; return; }
  FrameBoundsAccumulator bounds;
  for (const auto& poly : polygons) bounds(poly);
  bool allPointsInside = std::none_of(targetPoly.points.begin(), targetPoly.points.end(), 
                                    PointOutsideFrame(bounds.minX, bounds.maxX, bounds.minY, bounds.maxY));
  std::cout << (allPointsInside ? "<TRUE>" : "<FALSE>") << '\n';
}
void guseynov::commands::handleLessAreaCommand(const std::vector<Polygon>& polygons, const std::string& param) {
  Polygon targetPoly = guseynov::utils::parsePolygon(param);
  if (targetPoly.points.size() < 3) { std::cout << "<INVALID COMMAND>\n"; return; }
  double targetArea = guseynov::utils::calculateArea(targetPoly);
  size_t count = std::count_if(polygons.begin(), polygons.end(), HasLessArea(targetArea));
  std::cout << count << '\n';
}
