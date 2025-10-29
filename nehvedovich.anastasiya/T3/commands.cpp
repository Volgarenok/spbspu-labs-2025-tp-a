#include "commands.hpp"
#include <functional>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include "format_guard.hpp"

void nehvedovich::areaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  nehvedovich::FormatGuard guard(out);

  std::string param;
  in >> param;

  std::vector< Polygon > filteredPolygons;

  if (param == "EVEN")
  {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), nehvedovich::isEven);
  }
  else if (param == "ODD")
  {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), nehvedovich::isOdd);
  }
  else if (param == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::vector< double > polygonAreas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), polygonAreas.begin(), nehvedovich::getArea);

    out << std::fixed << std::setprecision(1) << std::accumulate(polygonAreas.begin(), polygonAreas.end(), 0.0) / polygons.size();
    return;
  }
  else if (std::all_of(param.begin(), param.end(), ::isdigit))
  {
    size_t vertexCount = std::stoul(param);
    if (vertexCount < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    auto areEqualVertexCount = std::bind(nehvedovich::hasVertexCount, std::placeholders::_1, vertexCount);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), areEqualVertexCount);
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  std::vector< double > polygonAreas(filteredPolygons.size());
  std::transform(filteredPolygons.begin(), filteredPolygons.end(), polygonAreas.begin(), nehvedovich::getArea);

  out << std::fixed << std::setprecision(1) << std::accumulate(polygonAreas.begin(), polygonAreas.end(), 0.0);
}

void nehvedovich::maxCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  nehvedovich::FormatGuard guard(out);

  std::string param;
  in >> param;

  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  if (param == "AREA")
  {
    auto maxIt = std::max_element(polygons.begin(), polygons.end(), compareByArea);
    out << std::fixed << std::setprecision(1) << getArea(*maxIt);
  }
  else if (param == "VERTEXES")
  {
    auto maxIt = std::max_element(polygons.begin(), polygons.end(), compareByVertices);
    out << std::fixed << std::setprecision(1) << maxIt->points.size();
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void nehvedovich::minCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  nehvedovich::FormatGuard guard(out);

  std::string param;
  in >> param;

  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  if (param == "AREA")
  {
    auto minIt = std::min_element(polygons.begin(), polygons.end(), compareByArea);
    out << std::fixed << std::setprecision(1) << getArea(*minIt);
  }
  else if (param == "VERTEXES")
  {
    auto minIt = std::min_element(polygons.begin(), polygons.end(), compareByVertices);
    out << std::fixed << std::setprecision(1) << minIt->points.size();
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void nehvedovich::countCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string param;
  in >> param;

  size_t count = 0;

  if (param == "EVEN")
  {
    count = std::count_if(polygons.begin(), polygons.end(), isEven);
  }
  else if (param == "ODD")
  {
    count = std::count_if(polygons.begin(), polygons.end(), isOdd);
  }
  else if (std::all_of(param.begin(), param.end(), ::isdigit))
  {
    size_t vertexCount = std::stoul(param);
    if (vertexCount < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    auto pred = std::bind(hasVertexCount, std::placeholders::_1, vertexCount);
    count = std::count_if(polygons.begin(), polygons.end(), pred);
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  out << count;
}
struct ContainsPoint
{
  int minX = 0, maxX = 0, minY = 0, maxY = 0;

  ContainsPoint(int minX, int maxX, int minY, int maxY):
    minX(minX),
    maxX(maxX),
    minY(minY),
    maxY(maxY)
  {}

  bool operator()(const nehvedovich::Point& p) const
  {
    return p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY;
  }
};

struct AccumulatePoints
{
  std::vector< nehvedovich::Point > operator()(std::vector< nehvedovich::Point > acc, const nehvedovich::Polygon& poly) const
  {
    acc.insert(acc.end(), poly.points.begin(), poly.points.end());
    return acc;
  }
};

struct BoundingBox
{
  int minX = 0, maxX = 0, minY = 0, maxY = 0;

  BoundingBox(const std::vector< nehvedovich::Polygon >& polygons)
  {
    std::vector< nehvedovich::Point > points = std::accumulate(
      polygons.begin(), polygons.end(), std::vector< nehvedovich::Point >{}, AccumulatePoints());

    if (points.empty())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    auto minmax_x = std::minmax_element(points.begin(), points.end(), nehvedovich::comparePointByX);
    auto minmax_y = std::minmax_element(points.begin(), points.end(), nehvedovich::comparePointByY);

    minX = minmax_x.first->x;
    maxX = minmax_x.second->x;
    minY = minmax_y.first->y;
    maxY = minmax_y.second->y;
  }

  bool contains(const nehvedovich::Polygon& poly) const
  {
    return std::all_of(poly.points.begin(), poly.points.end(), ContainsPoint(minX, maxX, minY, maxY));
  }
};

void nehvedovich::inFrameCommand(std::istream& in, std::ostream& out, const std::vector< nehvedovich::Polygon >& polygons)
{
  Polygon polygon;
  in >> polygon;

  if ((polygon.points.empty()) || (!in) || (in.peek() != '\n'))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  BoundingBox box(polygons);
  out << (box.contains(polygon) ? "<TRUE>" : "<FALSE>");
}

struct LessThanArea
{
  double area = 0.0;

  explicit LessThanArea(double a):
    area(a)
  {}

  bool operator()(const nehvedovich::Polygon& p) const
  {
    return nehvedovich::getArea(p) < area;
  }
};

void nehvedovich::lessAreaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon polygon;
  in >> polygon;

  if ((polygon.points.empty()) || (!in) || (in.peek() != '\n'))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  size_t count = std::count_if(polygons.begin(), polygons.end(), LessThanArea(getArea(polygon)));
  out << count;
}
