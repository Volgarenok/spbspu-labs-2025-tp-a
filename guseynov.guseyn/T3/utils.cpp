#include "utils.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <numeric>
#include <functional>
#include <iterator>
namespace {
struct IsSpace {
  bool operator()(char c) const { return std::isspace(static_cast<unsigned char>(c)); }
};
struct IsDigit {
  bool operator()(char c) const { return std::isdigit(static_cast<unsigned char>(c)); }
};
struct IsInvalidChar {
  bool operator()(char c) const { return !std::isdigit(static_cast<unsigned char>(c)) && c != '-'; }
};
struct PointAreaAccumulator {
  double operator()(double area, const std::pair<const guseynov::Point*, const guseynov::Point*>& points) const {
    return area + points.first->x * points.second->y - points.second->x * points.first->y;
  }
};
}
bool guseynov::utils::isNumber(const std::string& s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), IsDigit());
}
double guseynov::utils::calculateArea(const Polygon& poly) {
  if (poly.points.size() < 3) return 0.0;
  std::vector<std::pair<const Point*, const Point*>> pointPairs;
  pointPairs.reserve(poly.points.size());
  for (size_t i = 0; i < poly.points.size(); ++i) {
    size_t j = (i + 1) % poly.points.size();
    pointPairs.emplace_back(&poly.points[i], &poly.points[j]);
  }
  double area = std::accumulate(pointPairs.begin(), pointPairs.end(), 0.0, PointAreaAccumulator());
  return std::abs(area) / 2.0;
}
guseynov::Polygon guseynov::utils::parsePolygon(const std::string& line) {
  Polygon poly;
  std::string remaining = line;
  size_t start = std::find_if_not(remaining.begin(), remaining.end(), IsSpace()) - remaining.begin();
  size_t end = std::find_if(remaining.begin() + start, remaining.end(), IsSpace()) - remaining.begin();
  std::string numVerticesStr(remaining.begin() + start, remaining.begin() + end);
  if (numVerticesStr.empty()) { poly.points.clear(); return poly; }
  size_t numVertices = 0;
  try { numVertices = std::stoul(numVerticesStr); }
  catch (const std::exception&) { poly.points.clear(); return poly; }
  if (numVertices < 3) { poly.points.clear(); return poly; }
  remaining = std::string(remaining.begin() + end, remaining.end());
  for (size_t i = 0; i < numVertices; ++i) {
    start = std::find_if_not(remaining.begin(), remaining.end(), IsSpace()) - remaining.begin();
    if (start >= remaining.size()) { poly.points.clear(); return poly; }
    end = std::find_if(remaining.begin() + start, remaining.end(), IsSpace()) - remaining.begin();
    std::string pointStr(remaining.begin() + start, remaining.begin() + end);
    if (pointStr.size() < 5 || pointStr[0] != '(' || pointStr.back() != ')') { poly.points.clear(); return poly; }
    size_t semicolon = pointStr.find(';');
    if (semicolon == std::string::npos || semicolon < 2 || semicolon > pointStr.size() - 2) { poly.points.clear(); return poly; }
    std::string xStr(pointStr.begin() + 1, pointStr.begin() + semicolon);
    std::string yStr(pointStr.begin() + semicolon + 1, pointStr.end() - 1);
    if (xStr.empty() || yStr.empty() ||
        std::any_of(xStr.begin(), xStr.end(), IsInvalidChar()) ||
        std::any_of(yStr.begin(), yStr.end(), IsInvalidChar())) { poly.points.clear(); return poly; }
    try {
      int x = std::stoi(xStr);
      int y = std::stoi(yStr);
      poly.points.push_back({x, y});
    }
    catch (const std::exception&) { poly.points.clear(); return poly; }
    remaining = std::string(remaining.begin() + end, remaining.end());
  }
  if (std::find_if_not(remaining.begin(), remaining.end(), IsSpace()) != remaining.end()) { poly.points.clear(); return poly; }
  return poly;
}
std::vector<guseynov::Polygon> guseynov::utils::readPolygonsFromFile(const std::string& filename) {
  std::vector<Polygon> polygons;
  std::ifstream file(filename);
  if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);
  std::string line;
  while (std::getline(file, line)) {
    auto start = std::find_if_not(line.begin(), line.end(), IsSpace());
    auto end = std::find_if_not(line.rbegin(), line.rend(), IsSpace()).base();
    if (start >= end) continue;
    std::string trimmed(start, end);
    Polygon poly = parsePolygon(trimmed);
    if (poly.points.size() >= 3) polygons.push_back(poly);
  }
  return polygons;
}
