#include "commands.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <string>
#include "polygon.hpp"

void getAreaEven(std::ostream &out, const std::vector< shak::Polygon > &polygons)
{
  std::vector< shak::Polygon > currentPolygons;
  std::copy_if(std::begin(polygons), std::end(polygons), std::back_inserter(currentPolygons), shak::isEven);
  std::vector< double > polygonAreas(currentPolygons.size());
  std::transform(std::begin(currentPolygons), std::end(currentPolygons), std::begin(polygonAreas), shak::getArea);
  out << std::fixed;
  out.precision(1);
  out << std::accumulate(polygonAreas.begin(), polygonAreas.end(), double{}) << "\n";
}

void getAreaOdd(std::ostream &out, const std::vector< shak::Polygon > &polygons)
{
  std::vector< shak::Polygon > currentPolygons;
  std::copy_if(std::begin(polygons), std::end(polygons), std::back_inserter(currentPolygons), shak::isOdd);
  std::vector< double > polygonAreas(currentPolygons.size());
  std::transform(std::begin(currentPolygons), std::end(currentPolygons), std::begin(polygonAreas), shak::getArea);
  out << std::fixed;
  out.precision(1);
  out << std::accumulate(polygonAreas.begin(), polygonAreas.end(), double{}) << "\n";
}

void getAreaMean(std::ostream &out, const std::vector< shak::Polygon > &polygons)
{
  if (polygons.size() == 0)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  std::vector< double > polygonAreas(polygons.size());
  std::transform(std::begin(polygons), std::end(polygons), std::begin(polygonAreas), shak::getArea);
  out << std::fixed;
  out.precision(1);
  out << std::accumulate(polygonAreas.begin(), polygonAreas.end(), double{}) / polygons.size() << "\n";
}

void getAreaVertexes(std::ostream &out, const std::vector< shak::Polygon > &polygons, size_t vertexCount)
{
  std::vector< shak::Polygon > currentPolygons;
  auto areEqualVertexCount = std::bind(shak::areEqualVertexes, vertexCount, std::placeholders::_1);
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(currentPolygons), areEqualVertexCount);
  std::vector< double > polygonAreas(currentPolygons.size());
  std::transform(std::begin(currentPolygons), std::end(currentPolygons), std::begin(polygonAreas), shak::getArea);
  out << std::fixed;
  out.precision(1);
  out << std::accumulate(polygonAreas.begin(), polygonAreas.end(), double{}) << "\n";
}

void getMaxArea(std::ostream &out, const std::vector< shak::Polygon > &polygons)
{
  if (polygons.empty())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  std::vector< double > polygonAreas(polygons.size());
  std::transform(std::begin(polygons), std::end(polygons), polygonAreas.begin(), shak::getArea);
  auto maxIter = std::max_element(polygonAreas.begin(), polygonAreas.end());
  out << std::fixed << std::setprecision(1) << *maxIter << "\n";
}

void getMaxVertexes(std::ostream &out, const std::vector< shak::Polygon > &polygons)
{
  if (polygons.empty())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  std::vector< size_t > currentPolygons(polygons.size());
  std::transform(std::begin(polygons), std::end(polygons), currentPolygons.begin(), shak::getVertexes);
  auto maxIter = std::max_element(currentPolygons.begin(), currentPolygons.end());
  out << *maxIter << "\n";
}

void getMinArea(std::ostream &out, const std::vector< shak::Polygon > &polygons)
{
  if (polygons.empty())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  std::vector< double > polygonAreas(polygons.size());
  std::transform(std::begin(polygons), std::end(polygons), polygonAreas.begin(), shak::getArea);
  auto minIter = std::min_element(polygonAreas.begin(), polygonAreas.end());
  out << std::fixed << std::setprecision(1) << *minIter << "\n";
}

void getMinVertexes(std::ostream &out, const std::vector< shak::Polygon > &polygons)
{
  if (polygons.empty())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  std::vector< size_t > polygonVertexes(polygons.size());
  std::transform(std::begin(polygons), std::end(polygons), polygonVertexes.begin(), shak::getVertexes);
  auto minIter = std::min_element(polygonVertexes.begin(), polygonVertexes.end());
  out << *minIter << "\n";
}

void countEven(std::ostream &out, const std::vector< shak::Polygon > &polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), shak::isEven) << "\n";
}

void countOdd(std::ostream &out, const std::vector< shak::Polygon > &polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), shak::isOdd) << "\n";
}

void getCountVertexes(std::ostream &out, const std::vector< shak::Polygon > &polygons, size_t vertexCount)
{
  out << std::count_if(polygons.begin(), polygons.end(), std::bind(shak::areEqualVertexes, vertexCount, std::placeholders::_1)) << "\n";
}

void shak::createCommandHandler(std::map< std::string, std::function< void() > > &cmds, std::vector< Polygon > &polygons)
{
  cmds["AREA"] = std::bind(cmdArea, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MAX"] = std::bind(cmdMax, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MIN"] = std::bind(cmdMin, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["COUNT"] = std::bind(cmdCount, std::cref(polygons),std::ref(std::cin), std::ref(std::cout));
  cmds["MAXSEQ"] = std::bind(cmdMaxSeq, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["RECTS"] = std::bind(cmdRects, std::cref(polygons), std::ref(std::cout));
}

void shak::cmdArea(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out)
{
  std::string subcmd;
  in >> subcmd;
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getAreaEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(getAreaOdd, std::ref(out), std::cref(polygons));
  subcmds["MEAN"] = std::bind(getAreaMean, std::ref(out), std::cref(polygons));
  try
  {
    subcmds.at(subcmd)();
  }
  catch (const std::exception &e)
  {
    size_t vertexCount = std::stoull(subcmd);
    if (vertexCount < 3)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    getAreaVertexes(out, polygons, vertexCount);
  }
}

void shak::cmdMax(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out)
{
  std::string subcmd;
  in >> subcmd;
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMaxArea, std::ref(out), std::cref(polygons));
  subcmds["VERTEXES"] = std::bind(getMaxVertexes, std::ref(out), std::cref(polygons));
  try
  {
    subcmds.at(subcmd)();
  }
  catch (const std::exception &e)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
}

void shak::cmdMin(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out)
{
  std::string subcmd;
  in >> subcmd;
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMinArea, std::ref(out), std::cref(polygons));
  subcmds["VERTEXES"] = std::bind(getMinVertexes, std::ref(out), std::cref(polygons));
  try
  {
    subcmds.at(subcmd)();
  }
  catch (const std::exception &e)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
}

void shak::cmdCount(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out)
{
  std::string subcmd;
  in >> subcmd;
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(countEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(countOdd, std::ref(out), std::cref(polygons));
  try
  {
    subcmds.at(subcmd)();
  }
  catch (const std::exception &e)
  {
    size_t vertexCount = std::stoull(subcmd);
    if (vertexCount < 3)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    getCountVertexes(out, polygons, vertexCount);
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
