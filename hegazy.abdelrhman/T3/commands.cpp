#include "commands.hpp"

#include "stream_guard.hpp"
#include "geometry.hpp"
#include <iomanip>
#include <algorithm>
#include <functional>
#include <cctype>

bool hasVertexCount(const bob::Polygon &p, size_t count)
{
  return p.points.size() == count;
}

void bob::printAreaSum(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  bob::Iofmtguard guard(out);

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
    if (vertexCount < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    auto pred = std::bind(hasVertexCount, std::placeholders::_1, vertexCount);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), pred);
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  double totalArea = getTotalArea(filteredPolygons);
  out << std::fixed << std::setprecision(1) << totalArea;
}

void bob::printMaxValueOf(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  bob::Iofmtguard guard(out);

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

void bob::printMinValueOf(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  bob::Iofmtguard guard(out);

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

void bob::printCountOf(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
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

void bob::printLessArea(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  Polygon polygon;
  in >> polygon;

  if ((polygon.points.empty()) || (!in) || (in.peek() != '\n'))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  out << std::count_if(polygons.cbegin(), polygons.cend(), std::bind(compareByArea, std::placeholders::_1, polygon));
}

bool intersectsWith(const bob::Polygon &a, const bob::Polygon &b)
{
  int p1_minX = std::min_element(a.points.cbegin(), a.points.cend(), bob::comparePointByX)->x;
  int p1_maxX = std::max_element(a.points.cbegin(), a.points.cend(), bob::comparePointByX)->x;
  int p1_minY = std::min_element(a.points.cbegin(), a.points.cend(), bob::comparePointByY)->y;
  int p1_maxY = std::max_element(a.points.cbegin(), a.points.cend(), bob::comparePointByY)->y;

  int p2_minX = std::min_element(b.points.cbegin(), b.points.cend(), bob::comparePointByX)->x;
  int p2_maxX = std::max_element(b.points.cbegin(), b.points.cend(), bob::comparePointByX)->x;
  int p2_minY = std::min_element(b.points.cbegin(), b.points.cend(), bob::comparePointByY)->y;
  int p2_maxY = std::max_element(b.points.cbegin(), b.points.cend(), bob::comparePointByY)->y;

  bool x_overlap = (p1_minX <= p2_maxX) && (p2_minX <= p1_maxX);
  bool y_overlap = (p1_minY <= p2_maxY) && (p2_minY <= p1_maxY);

  return x_overlap && y_overlap;
}

void bob::printIntersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  Polygon polygon;
  in >> polygon;

  if ((polygon.points.empty()) || (!in) || (in.peek() != '\n'))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  out << std::count_if(polygons.cbegin(), polygons.cend(), std::bind(intersectsWith, polygon, std::placeholders::_1));
}
