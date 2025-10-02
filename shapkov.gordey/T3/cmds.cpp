#include "cmds.hpp"
#include <string>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <iomanip>
#include <unordered_map>
#include <scopeGuard.hpp>
#include "polygonfunctors.hpp"

using namespace std::placeholders;

void shapkov::areaEven(std::ostream& out, const VecOfPolygons& src)
{
  VecOfPolygons evenPolygons;
  std::copy_if(src.begin(), src.end(), std::back_inserter(evenPolygons), isEven);
  std::vector< double > areas(evenPolygons.size());
  std::transform(evenPolygons.begin(), evenPolygons.end(), areas.begin(), getArea);
  double area = std::accumulate(areas.begin(), areas.end(), 0.0);
  out << std::fixed << std::setprecision(1) << area << '\n';
}
void shapkov::areaOdd(std::ostream& out, const VecOfPolygons& src)
{
  VecOfPolygons oddPolygons;
  std::copy_if(src.begin(), src.end(), std::back_inserter(oddPolygons), isOdd);
  std::vector< double > areas(oddPolygons.size());
  std::transform(oddPolygons.begin(), oddPolygons.end(), areas.begin(), getArea);
  double area = std::accumulate(areas.begin(), areas.end(), 0.0);
  out << std::fixed << std::setprecision(1) << area << '\n';
}
void shapkov::areaMean(std::ostream& out, const VecOfPolygons& src)
{
  if (src.empty())
  {
    throw std::logic_error("no polygons");
  }
  std::vector< double > areas(src.size());
  std::transform(src.begin(), src.end(), areas.begin(), getArea);
  double area = std::accumulate(areas.begin(), areas.end(), 0.0) / src.size();
  out << std::fixed << std::setprecision(1) << area << '\n';
}
void shapkov::areaVertexes(std::ostream& out, const VecOfPolygons& src, size_t vertexes)
{
  if (vertexes < 3)
  {
    throw std::logic_error("wrong number of vertexes");
  }
  VecOfPolygons polygons;
  std::copy_if(src.begin(), src.end(), std::back_inserter(polygons), std::bind(isSize, _1, vertexes));
  std::vector< double > areas(polygons.size());
  std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
  double area = std::accumulate(areas.begin(), areas.end(), 0.0);
  out << std::fixed << std::setprecision(1) << area << '\n';
}
void shapkov::area(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{
  std::unordered_map< std::string, std::function< void() > > areaCmds;
  areaCmds["EVEN"] = std::bind(areaEven, std::ref(out), std::cref(src));
  areaCmds["ODD"] = std::bind(areaOdd, std::ref(out), std::cref(src));
  areaCmds["MEAN"] = std::bind(areaMean, std::ref(out), std::cref(src));
  ScopeGuard scopeGuard(out);
  std::string subcommand;
  in >> subcommand;
  try
  {
    areaCmds.at(subcommand)();
  }
  catch (...)
  {
    size_t vertexes = std::stoi(subcommand);
    areaVertexes(out, src, vertexes);
  }
}
void shapkov::maxArea(std::ostream& out, const VecOfPolygons& src)
{
  std::vector< double > areas(src.size());
  std::transform(src.begin(), src.end(), areas.begin(), getArea);
  out << std::fixed << std::setprecision(1) << (*std::max_element(areas.begin(), areas.end())) << '\n';
}
void shapkov::maxVertexes(std::ostream& out, const VecOfPolygons& src)
{
  out << (std::max_element(src.begin(), src.end(), compareByVertexes)->points.size()) << '\n';
}
void shapkov::max(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{
  if (src.empty())
  {
    throw std::logic_error("no polygons");
  }
  std::unordered_map< std::string, std::function< void() > > maxCmds;
  maxCmds["AREA"] = std::bind(maxArea, std::ref(out), std::cref(src));
  maxCmds["VERTEXES"] = std::bind(maxVertexes, std::ref(out), std::cref(src));
  ScopeGuard scopeGuard(out);
  std::string subcommand;
  in >> subcommand;
  maxCmds.at(subcommand)();
}
void shapkov::minArea(std::ostream& out, const VecOfPolygons& src)
{
  std::vector< double > areas(src.size());
  std::transform(src.begin(), src.end(), areas.begin(), getArea);
  out << std::fixed << std::setprecision(1) << (*std::min_element(areas.begin(), areas.end())) << '\n';
}
void shapkov::minVertexes(std::ostream& out, const VecOfPolygons& src)
{
  out << (std::min_element(src.begin(), src.end(), compareByVertexes)->points.size()) << '\n';
}
void shapkov::min(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{
  if (src.empty())
  {
    throw std::logic_error("no polygons");
  }
  std::unordered_map< std::string, std::function< void() > > minCmds;
  minCmds["AREA"] = std::bind(minArea, std::ref(out), std::cref(src));
  minCmds["VERTEXES"] = std::bind(minVertexes, std::ref(out), std::cref(src));
  ScopeGuard scopeGuard(out);
  std::string subcommand;
  in >> subcommand;
  minCmds.at(subcommand)();
}
void shapkov::countEven(std::ostream& out, const VecOfPolygons& src)
{
  out << std::count_if(src.begin(), src.end(), isEven) << '\n';
}
void shapkov::countOdd(std::ostream& out, const VecOfPolygons& src)
{
  out << std::count_if(src.begin(), src.end(), isOdd) << '\n';
}
void shapkov::countVertexes(std::ostream& out, const VecOfPolygons& src, size_t vertexes)
{
  if (vertexes < 3)
  {
    throw std::logic_error("wrong number of vertexes");
  }
  out << std::count_if(src.begin(), src.end(), std::bind(isSize, _1, vertexes)) << '\n';
}
void shapkov::count(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{
  std::unordered_map< std::string, std::function< void() > > countCmds;
  countCmds["EVEN"] = std::bind(countEven, std::ref(out), std::cref(src));
  countCmds["ODD"] = std::bind(countOdd, std::ref(out), std::cref(src));
  ScopeGuard scopeGuard(out);
  std::string subcommand;
  in >> subcommand;
  try
  {
    countCmds.at(subcommand)();
  }
  catch (...)
  {
    size_t vertexes = std::stoi(subcommand);
    countVertexes(out, src, vertexes);
  }
}
void shapkov::rects(std::ostream& out, const VecOfPolygons& src)
{
  out << std::count_if(src.begin(), src.end(), isRectangle) << '\n';
}
void shapkov::same(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{
  Polygon polygon;
  in >> polygon;
  if (!in || in.peek() != '\n')
  {
    throw std::logic_error("wrong polygon");
  }
  out << std::count_if(src.begin(), src.end(), isSame{ polygon }) << '\n';
}
