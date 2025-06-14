#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <limits>
#include <string>
#include "polygon.hpp"

void shak::cmdArea(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out)
{
  std::vector< Polygon > currentPolygons;
  std::vector< double > polygonAreas;
  size_t vertexCount = 0;
  size_t counter = 0;
  std::string subcmd;
  in >> subcmd;
  if (subcmd == "EVEN")
  {
    counter = count_if(std::begin(polygons), std::end(polygons), isEven);
    currentPolygons.reserve(counter);
    polygonAreas.reserve(counter);
    std::copy_if(std::begin(polygons), std::end(polygons), std::back_inserter(currentPolygons), isEven);
  }
  else if (subcmd == "ODD")
  {
    counter = count_if(std::begin(polygons), std::end(polygons), isOdd);
    currentPolygons.reserve(counter);
    polygonAreas.reserve(counter);
    std::copy_if(std::begin(polygons), std::end(polygons), std::back_inserter(currentPolygons), isOdd);
  }
  else if (subcmd == "MEAN")
  {
    if (polygons.size() == 0)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::transform(std::begin(polygons), std::end(polygons), std::back_inserter(polygonAreas), getArea);
    out << std::fixed;
    out.precision(1);
    out << (std::accumulate(polygonAreas.begin(), polygonAreas.end(), 0.0, std::plus< double >{}) / polygons.size()) << "\n";
    return;
  }
  else
  {
    vertexCount = std::stoull(subcmd);
    if (vertexCount < 3)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    else
    {
      using namespace std::placeholders;
      counter = count_if(std::begin(polygons), std::end(polygons), std::bind(areEqualVertexes, vertexCount, _1));
      currentPolygons.reserve(counter);
      polygonAreas.reserve(counter);
      auto areEqualVertexCount = std::bind(areEqualVertexes, vertexCount, _1);
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(currentPolygons), areEqualVertexCount);
    }
  }
  std::transform(std::begin(currentPolygons), std::end(currentPolygons), std::back_inserter(polygonAreas), getArea);
  out << std::fixed;
  out.precision(1);
  out << std::accumulate(polygonAreas.begin(), polygonAreas.end(), 0.0, std::plus< double >{}) << "\n";
}

void shak::cmdMax(const std::vector< Polygon > &polygon, std::istream &in, std::ostream &out)
{
  if (polygon.size() == 0)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  else
  {
    std::string subcmd;
    in >> subcmd;
    if (subcmd == "AREA")
    {
      std::vector< double > currentPolygons(polygon.size());
      std::transform(std::begin(polygon), std::end(polygon), currentPolygons.begin(), getArea);
      auto maxIter = std::max_element(currentPolygons.begin(), currentPolygons.end());
      out << std::fixed << std::setprecision(1) << *maxIter << "\n";
    }
    else if (subcmd == "VERTEXES")
    {
      std::vector< size_t > currentPolygons(polygon.size());
      std::transform(std::begin(polygon), std::end(polygon), currentPolygons.begin(), getVertexes);
      auto maxIter = std::max_element(currentPolygons.begin(), currentPolygons.end());
      out << *maxIter << "\n";
    }
    else
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
  }
}

void shak::cmdMin(const std::vector< Polygon > &polygon, std::istream &in, std::ostream &out)
{
  if (polygon.size() == 0)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  else
  {
    std::string subcmd;
    in >> subcmd;

    if (subcmd == "AREA")
    {
      std::vector< double > currentPolygons(polygon.size());
      std::transform(std::begin(polygon), std::end(polygon), currentPolygons.begin(), getArea);
      auto minIter = std::min_element(currentPolygons.begin(), currentPolygons.end());
      out << std::fixed << std::setprecision(1) << *minIter << "\n";
    }
    else if (subcmd == "VERTEXES")
    {
      std::vector< size_t > currentPolygons(polygon.size());
      std::transform(std::begin(polygon), std::end(polygon), currentPolygons.begin(), getVertexes);
      auto minIter = std::min_element(currentPolygons.begin(), currentPolygons.end());
      out << *minIter << "\n";
}
    else
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
  }
}

void shak::cmdCount(const std::vector< Polygon > &polygon, std::istream &in, std::ostream &out)
{
  size_t vertexCount = 0;
  std::string subcmd;
  in >> subcmd;
  if (subcmd == "EVEN")
  {
    out << std::count_if(polygon.begin(), polygon.end(), isEven) << "\n";
  }
  else if (subcmd == "ODD")
  {
    out << std::count_if(polygon.begin(), polygon.end(), isOdd) << "\n";
  }
  else
  {
    vertexCount = std::stoull(subcmd);
    if (vertexCount < 3)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    else
    {
      using namespace std::placeholders;
      out << std::count_if(polygon.begin(), polygon.end(), std::bind(areEqualVertexes, vertexCount, _1)) << "\n";
    }
  }
}

void shak::cmdMaxSeq(const std::vector< Polygon > &polygon, std::istream &in, std::ostream &out)
{
  size_t counter = 0;
  size_t vertexCount = 0;
  std::vector< Point > srcPoints;
  std::vector< size_t > matchCount;
  using inIter = std::istream_iterator< Point >;
  in >> vertexCount;
  if (vertexCount < 3)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  srcPoints.reserve(vertexCount);
  std::copy_n(inIter{in}, vertexCount, std::back_inserter(srcPoints));
  if (srcPoints.empty() || in.peek() != '\n')
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  matchCount.reserve(polygon.size());
  using namespace std::placeholders;
  auto functor = std::bind(equalCounter, _1, srcPoints, counter);
  std::transform(std::begin(polygon), std::end(polygon), std::back_inserter(matchCount), functor);
  auto maxMatchIter = std::max_element(matchCount.begin(), matchCount.end());
  out << *maxMatchIter << "\n";
}

void shak::cmdRects(const std::vector< Polygon > &polygons, std::ostream &out)
{
  out << std::count_if(polygons.begin(), polygons.end(), checkRectangle) << "\n";
}
