#include "commands.hpp"

#include <iomanip>
#include "format_guard.hpp"

void filonova::area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  filonova::FormatGuard guard(out);

  std::string subcmd;
  in >> subcmd;

  std::vector< Polygon > filteredPolygons;

  if (subcmd == "EVEN")
  {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), isEven);
  }
  else if (subcmd == "ODD")
  {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), isOdd);
  }
  else if (subcmd == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }

    double totalArea = getTotalArea(polygons);
    out << std::fixed << std::setprecision(1) << totalArea / polygons.size();
    return;
  }
  else if (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
  {
    size_t vertexCount = std::stoul(subcmd);
    if (vertexCount < MIN_VERTEX_COUNT)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    auto pred = std::bind(filonova::hasVertexCount, std::placeholders::_1, vertexCount);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), pred);
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  double totalArea = getTotalArea(filteredPolygons);
  out << std::fixed << std::setprecision(1) << totalArea;
}

bool filonova::hasVertexCount(const filonova::Polygon &p, size_t count)
{
  return p.points.size() == count;
}

void filonova::max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  filonova::FormatGuard guard(out);

  std::string subcmd;
  in >> subcmd;

  out << std::fixed << std::setprecision(1);

  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  if (subcmd == "AREA")
  {
    auto maxIt = std::max_element(polygons.begin(), polygons.end(), compareByArea);
    out << getPolygonArea(*maxIt);
  }
  else if (subcmd == "VERTEXES")
  {
    auto maxIt = std::max_element(polygons.begin(), polygons.end(), compareByVertexes);
    out << maxIt->points.size();
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void filonova::min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  filonova::FormatGuard guard(out);

  std::string subcmd;
  in >> subcmd;

  out << std::fixed << std::setprecision(1);

  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  if (subcmd == "AREA")
  {
    auto minIt = std::min_element(polygons.begin(), polygons.end(), compareByArea);
    out << getPolygonArea(*minIt);
  }
  else if (subcmd == "VERTEXES")
  {
    auto minIt = std::min_element(polygons.begin(), polygons.end(), compareByVertexes);
    out << minIt->points.size();
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void filonova::count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string subcmd;
  in >> subcmd;

  size_t count = 0;

  if (subcmd == "EVEN")
  {
    count = std::count_if(polygons.begin(), polygons.end(), isEven);
  }
  else if (subcmd == "ODD")
  {
    count = std::count_if(polygons.begin(), polygons.end(), isOdd);
  }
  else if (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
  {
    size_t vertexCount = std::stoul(subcmd);
    if (vertexCount < MIN_VERTEX_COUNT)
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

bool intersectsWith(const filonova::Polygon &a, const filonova::Polygon &b)
{
  int p1_minX = std::min_element(a.points.cbegin(), a.points.cend(), filonova::comparePointByX)->x;
  int p1_maxX = std::max_element(a.points.cbegin(), a.points.cend(), filonova::comparePointByX)->x;
  int p1_minY = std::min_element(a.points.cbegin(), a.points.cend(), filonova::comparePointByY)->y;
  int p1_maxY = std::max_element(a.points.cbegin(), a.points.cend(), filonova::comparePointByY)->y;

  int p2_minX = std::min_element(b.points.cbegin(), b.points.cend(), filonova::comparePointByX)->x;
  int p2_maxX = std::max_element(b.points.cbegin(), b.points.cend(), filonova::comparePointByX)->x;
  int p2_minY = std::min_element(b.points.cbegin(), b.points.cend(), filonova::comparePointByY)->y;
  int p2_maxY = std::max_element(b.points.cbegin(), b.points.cend(), filonova::comparePointByY)->y;

  bool x_overlap = (p1_minX <= p2_maxX) && (p2_minX <= p1_maxX);
  bool y_overlap = (p1_minY <= p2_maxY) && (p2_minY <= p1_maxY);

  return x_overlap && y_overlap;
}

void filonova::intersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  Polygon polygon;
  in >> polygon;

  if ((polygon.points.empty()) || (!in) || (in.peek() != '\n'))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  out << std::count_if(polygons.cbegin(), polygons.cend(), std::bind(intersectsWith, polygon, std::placeholders::_1));
}

void filonova::rects(std::ostream &out, const std::vector< Polygon > &polygons)
{
  size_t count = std::count_if(polygons.begin(), polygons.end(), isRectangle);
  out << count;
}

double dot(const filonova::Point &a, const filonova::Point &b, const filonova::Point &c)
{
  double abx = b.x - a.x;
  double aby = b.y - a.y;
  double bcx = c.x - b.x;
  double bcy = c.y - b.y;
  return abx * bcx + aby * bcy;
}

double getDistanceSquared(const filonova::Point &a, const filonova::Point &b)
{
  double dx = b.x - a.x;
  double dy = b.y - a.y;
  return dx * dx + dy * dy;
}

bool isRightAngle(const filonova::Point &prev, const filonova::Point &current, const filonova::Point &next)
{
  return std::abs(dot(prev, current, next)) <= EPS;
}

bool filonova::isRectangle(const filonova::Polygon &polygon)
{
  const auto &points = polygon.points;

  if (points.size() != RECTANGLE_SIDES)
  {
    return false;
  }

  double diag1 = getDistanceSquared(points[0], points[2]);
  double diag2 = getDistanceSquared(points[1], points[3]);

  if (std::abs(diag1 - diag2) > EPS)
  {
    return false;
  }

  return isRightAngle(points[3], points[0], points[1]) &&
    isRightAngle(points[0], points[1], points[2]) &&
    isRightAngle(points[1], points[2], points[3]) &&
    isRightAngle(points[2], points[3], points[0]);
}
