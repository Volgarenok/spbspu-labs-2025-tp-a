#include "commands.hpp"

using namespace std::placeholders;

void filonova::area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
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
      out << "<INVALID COMMAND>";
      return;
    }

    double totalArea = computeTotalArea(polygons);
    out << std::fixed << std::setprecision(1) << totalArea / polygons.size();
    return;
  }
  else if (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
  {
    size_t vertexCount = std::stoul(subcmd);
    if (vertexCount < MIN_VERTEX_COUNT)
    {
      out << "<INVALID COMMAND>";
      return;
    }

    auto pred = std::bind(HasVertexCount(vertexCount), _1);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), pred);
  }
  else
  {
    out << "<INVALID COMMAND>";
    return;
  }

  double totalArea = computeTotalArea(filteredPolygons);
  out << std::fixed << std::setprecision(1) << totalArea;
}

void filonova::max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string subcmd;
  in >> subcmd;

  out << std::fixed << std::setprecision(1);

  if (polygons.empty())
  {
    out << "<INVALID COMMAND>";
    return;
  }

  if (subcmd == "AREA")
  {
    auto maxIt = std::max_element(polygons.begin(), polygons.end(), CompareByArea());
    out << getArea(*maxIt);
  }
  else if (subcmd == "VERTEXES")
  {
    auto maxIt = std::max_element(polygons.begin(), polygons.end(), CompareByVertexes());
    out << maxIt->points.size();
  }
  else
  {
    out << "<INVALID COMMAND>";
  }
}

void filonova::min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string subcmd;
  in >> subcmd;

  out << std::fixed << std::setprecision(1);

  if (polygons.empty())
  {
    out << "<INVALID COMMAND>";
    return;
  }

  if (subcmd == "AREA")
  {
    auto minIt = std::min_element(polygons.begin(), polygons.end(), CompareByArea());
    out << getArea(*minIt);
  }
  else if (subcmd == "VERTEXES")
  {
    auto minIt = std::min_element(polygons.begin(), polygons.end(), CompareByVertexes());
    out << minIt->points.size();
  }
  else
  {
    out << "<INVALID COMMAND>";
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
      out << "<INVALID COMMAND>";
      return;
    }

    auto pred = std::bind(HasVertexCount(vertexCount), _1);
    count = std::count_if(polygons.begin(), polygons.end(), pred);
  }
  else
  {
    out << "<INVALID COMMAND>";
    return;
  }

  out << count;
}

void filonova::intersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string line;
  std::getline(in, line);

  if (line.empty())
  {
    out << "<INVALID COMMAND>";
    return;
  }

  std::istringstream lineStream(line);
  Polygon inputPolygon;
  if (!(lineStream >> inputPolygon))
  {
    out << "<INVALID COMMAND>";
    return;
  }

  std::string extra;
  if (lineStream >> extra)
  {
    out << "<INVALID COMMAND>";
    return;
  }

  if (inputPolygon.points.size() < MIN_VERTEX_COUNT)
  {
    out << "<INVALID COMMAND>";
    return;
  }

  IntersectsWith intersects(inputPolygon);
  auto pred = std::bind(intersects, _1);
  size_t count = std::count_if(polygons.begin(), polygons.end(), pred);
  out << count;
}

void filonova::rects(std::ostream &out, const std::vector< Polygon > &polygons)
{
  size_t count = std::count_if(polygons.begin(), polygons.end(), IsRectangle{});
  out << count;
}
