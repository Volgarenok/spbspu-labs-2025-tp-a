#include "commands.hpp"
#include "functors.hpp"

void filonova::area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string subcmd;
  in >> subcmd;

  double area = 0.0;

  if (subcmd == "EVEN")
  {
    std::vector< Polygon > filteredPolygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), IsEven{});

    std::vector< double > areas;
    std::transform(filteredPolygons.begin(), filteredPolygons.end(), std::back_inserter(areas), GetPolygonArea{});
    area = std::accumulate(areas.begin(), areas.end(), 0.0);
  }
  else if (subcmd == "ODD")
  {
    std::vector< Polygon > filteredPolygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), IsOdd{});

    std::vector< double > areas;
    std::transform(filteredPolygons.begin(), filteredPolygons.end(), std::back_inserter(areas), GetPolygonArea{});
    area = std::accumulate(areas.begin(), areas.end(), 0.0);
  }
  else if (subcmd == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }

    std::vector< double > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), GetPolygonArea{});
    area = std::accumulate(areas.begin(), areas.end(), 0.0) / areas.size();
  }
  else
  {
    size_t vertexes = std::stoul(subcmd);
    if (vertexes < MIN_VERTEX_COUNT)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }

    std::vector< Polygon > filteredPolygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), HasVertexCount{vertexes});

    std::vector< double > areas;
    std::transform(filteredPolygons.begin(), filteredPolygons.end(), std::back_inserter(areas), GetPolygonArea{});
    area = std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  out << std::fixed << std::setprecision(1) << area;
}

void filonova::max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string subcmd;
  in >> subcmd;

  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  if (subcmd == "AREA")
  {
    std::vector< double > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), GetPolygonArea{});

    double maxArea = *std::max_element(areas.begin(), areas.end());
    out << std::fixed << std::setprecision(1) << maxArea;
  }
  else if (subcmd == "VERTEXES")
  {
    std::vector< size_t > vertexCounts;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(vertexCounts), GetPolygonVertexCount{});

    size_t maxVertexes = *std::max_element(vertexCounts.begin(), vertexCounts.end());
    out << maxVertexes;
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void filonova::min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string subcmd;
  in >> subcmd;

  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  if (subcmd == "AREA")
  {
    std::vector< double > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), GetPolygonArea{});

    double minArea = *std::min_element(areas.begin(), areas.end());
    out << std::fixed << std::setprecision(1) << minArea;
  }
  else if (subcmd == "VERTEXES")
  {
    std::vector< size_t > vertexCounts;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(vertexCounts), GetPolygonVertexCount{});

    size_t minVertexes = *std::min_element(vertexCounts.begin(), vertexCounts.end());
    out << minVertexes;
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

  size_t result = 0;

  if (subcmd == "EVEN")
  {
    result = std::count_if(polygons.begin(), polygons.end(), IsEven{});
  }
  else if (subcmd == "ODD")
  {
    result = std::count_if(polygons.begin(), polygons.end(), IsOdd{});
  }
  else
  {
    size_t vertexes = std::stoul(subcmd);
    if (vertexes < MIN_VERTEX_COUNT)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }

    result = std::count_if(polygons.begin(), polygons.end(), HasVertexCount{vertexes});
  }

  out << result;
}

void filonova::intersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  Polygon inputPolygon;
  in >> inputPolygon;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  size_t count = std::count_if(polygons.begin(), polygons.end(), IntersectsWith{inputPolygon});
  out << count;
}

void filonova::rects(std::ostream &out, const std::vector< Polygon > &polygons)
{
  size_t count = std::count_if(polygons.begin(), polygons.end(), filonova::IsRectangle{});
  out << count;
}
