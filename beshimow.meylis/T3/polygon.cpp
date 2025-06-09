#include "polygon.hpp"

#include <sstream>
#include <cmath>

bool Point::operator==(const Point& other) const {
  return x == other.x && y == other.y;
}

bool Polygon::operator==(const Polygon& other) const {
  return points == other.points;
}

bool parsePolygon(const std::string& line, Polygon& poly) {
  std::istringstream iss(line);
  int n;
  if (!(iss >> n) || n < 3) return false;

  std::vector<Point> pts;
  std::string token;

  while (std::getline(iss >> std::ws, token, ' ')) {
    if (token.size() < 5 || token.front() != '(' || token.back() != ')') return false;
    std::string inner = token.substr(1, token.size() - 2);
    std::size_t semi = inner.find(';');
    if (semi == std::string::npos) return false;

    try {
      int x = std::stoi(inner.substr(0, semi));
      int y = std::stoi(inner.substr(semi + 1));
      pts.push_back({x, y});
    } catch (...) {
      return false;
    }
  }

  if (pts.size() != static_cast<std::size_t>(n)) return false;

  poly.points = std::move(pts);
  return true;
}

double computeArea(const Polygon& poly) {
  const auto& pts = poly.points;
  std::size_t n = pts.size();
  double area = 0.0;

  for (std::size_t i = 0; i < n; ++i) {
    std::size_t j = (i + 1) % n;
    area += pts[i].x * pts[j].y - pts[j].x * pts[i].y;
  }

  return std::abs(area) / 2.0;
}
