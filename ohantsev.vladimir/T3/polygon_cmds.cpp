#include "polygon_cmds.h"
#include <array>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <numeric>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <iofmtguard.h>

bool ohantsev::isOdd(const Polygon& polygon) noexcept
{
  return polygon.size() % 2;
}

bool ohantsev::isEven(const Polygon& polygon) noexcept
{
  return !(polygon.size() % 2);
}

bool ohantsev::thisSize(const Polygon& polygon, std::size_t size) noexcept
{
  return polygon.size() == size;
}

bool ohantsev::lessSize(const Polygon& lhs, const Polygon& rhs) noexcept
{
  return lhs.size() < rhs.size();
}

ohantsev::Area::Area(const std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out):
  CommandHandler(in, out),
  polygons_(polygons)
{
  add("EVEN", std::bind(even, std::cref(polygons), std::ref(out)));
  add("ODD", std::bind(odd, std::cref(polygons), std::ref(out)));
  add("MEAN", std::bind(mean, std::cref(polygons), std::ref(out)));
}

void ohantsev::Area::operator()()
{
  std::string subcmd;
  in_ >> subcmd;
  try
  {
    run(subcmd);
  }
  catch (const std::out_of_range&)
  {
    static constexpr std::size_t TRIANGLE_VERTEXES_COUNT = 3;
    std::size_t symbolsProcessed;
    std::size_t vertexes = std::stoull(subcmd, &symbolsProcessed);
    if (symbolsProcessed < subcmd.size() || vertexes < TRIANGLE_VERTEXES_COUNT)
    {
      throw std::invalid_argument("invalid subcommand");
    }
    numOfVertexes(polygons_, out_, vertexes);
  }
}

double ohantsev::Area::accumulateArea(const std::vector< Polygon >& polygons)
{
  std::vector< double > areas(polygons.size());
  std::transform(polygons.cbegin(), polygons.cend(), areas.begin(), getArea);
  return std::accumulate(areas.cbegin(), areas.cend(), 0.0);
}

double ohantsev::Area::accumulateAreaIf(const std::vector< Polygon >& polygons, const std::function< bool(const Polygon&) >& sign)
{
  std::vector< Polygon > validPolygons;
  validPolygons.reserve(polygons.size());
  std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(validPolygons), sign);
  return accumulateArea(validPolygons);
}

void ohantsev::Area::odd(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << accumulateAreaIf(polygons, isOdd) << '\n';
}

void ohantsev::Area::even(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << accumulateAreaIf(polygons, isEven) << '\n';
}

void ohantsev::Area::numOfVertexes(const std::vector< Polygon >& polygons, std::ostream& out, std::size_t num)
{
  using namespace std::placeholders;
  out << accumulateAreaIf(polygons, std::bind(thisSize, _1, num)) << '\n';
}

void ohantsev::Area::mean(const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("there are no polygons");
  }
  out << accumulateArea(polygons) / polygons.size() << '\n';
}

ohantsev::Max::Max(const std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out):
  CommandHandler(in, out),
  polygons_(polygons)
{
  add("AREA", std::bind(area, std::cref(polygons), std::ref(out)));
  add("VERTEXES", std::bind(vertexes, std::cref(polygons), std::ref(out)));
}

void ohantsev::Max::area(const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("there are no polygons");
  }
  std::vector< double > areas(polygons.size());
  std::transform(polygons.cbegin(), polygons.cend(), areas.begin(), getArea);
  out << *std::max_element(areas.cbegin(), areas.cend()) << '\n';
}

void ohantsev::Max::vertexes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("there are no polygons");
  }
  out << std::max_element(polygons.cbegin(), polygons.cend(), lessSize)->size() << '\n';
}

ohantsev::Min::Min(const std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out):
  CommandHandler(in, out),
  polygons_(polygons)
{
  add("AREA", std::bind(area, std::cref(polygons), std::ref(out)));
  add("VERTEXES", std::bind(vertexes, std::cref(polygons), std::ref(out)));
}

void ohantsev::Min::area(const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("there are no polygons");
  }
  std::vector< double > areas(polygons.size());
  std::transform(polygons.cbegin(), polygons.cend(), areas.begin(), getArea);
  out << *std::min_element(areas.cbegin(), areas.cend()) << '\n';
}

void ohantsev::Min::vertexes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("there are no polygons");
  }
  out << std::min_element(polygons.cbegin(), polygons.cend(), lessSize)->size() << '\n';
}

ohantsev::Count::Count(const std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out):
  CommandHandler(in, out),
  polygons_(polygons)
{
  add("EVEN", std::bind(even, std::cref(polygons), std::ref(out)));
  add("ODD", std::bind(odd, std::cref(polygons), std::ref(out)));
}

void ohantsev::Count::operator()()
{
  std::string subcmd;
  in_ >> subcmd;
  try
  {
    run(subcmd);
  }
  catch (const std::out_of_range&)
  {
    static constexpr std::size_t TRIANGLE_VERTEXES_COUNT = 3;
    std::size_t processedCount;
    std::size_t num = std::stoull(subcmd, &processedCount);
    if (processedCount < subcmd.size() || num < TRIANGLE_VERTEXES_COUNT)
    {
      throw std::invalid_argument("invalid subcommand");
    }
    numOfVertexes(polygons_, out_, num);
  }
}

void ohantsev::Count::odd(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << std::count_if(polygons.cbegin(), polygons.cend(), isOdd) << '\n';
}

void ohantsev::Count::even(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << std::count_if(polygons.cbegin(), polygons.cend(), isEven) << '\n';
}

void ohantsev::Count::numOfVertexes(const std::vector< Polygon >& polygons, std::ostream& out, std::size_t num)
{
  using namespace std::placeholders;
  out << std::count_if(polygons.cbegin(), polygons.cend(), std::bind(thisSize, _1, num)) << '\n';
}

ohantsev::PolygonCmdsHandler::PolygonCmdsHandler(std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out):
  CommandHandler(in, out)
{
  add("AREA", Area{ polygons, in, out });
  add("MAX", Max{ polygons, in, out });
  add("MIN", Min{ polygons, in, out });
  add("COUNT", Count{ polygons, in, out });
  add("PERMS", std::bind(perms, std::cref(polygons), std::ref(in), std::ref(out)));
  add("RECTS", std::bind(rects, std::cref(polygons), std::ref(out)));
}

void ohantsev::PolygonCmdsHandler::operator()()
{
  iofmtguard fmt(out_);
  out_ << std::fixed << std::setprecision(1);
  try
  {
    CommandHandler::operator()();
  }
  catch (...)
  {
    if (in_.eof())
    {
      return;
    }
    out_ << "<INVALID COMMAND>\n";
    if (in_.fail())
    {
      in_.clear(in_.rdstate() ^ std::ios::failbit);
    }
    in_.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}

void ohantsev::PolygonCmdsHandler::processUntilEOF()
{
  while (!in_.eof())
  {
    operator()();
  }
}

auto ohantsev::getSorted(const Polygon& polygon) -> Polygon
{
  Polygon res = polygon;
  std::sort(res.points.begin(), res.points.end());
  return res;
}

void ohantsev::perms(const std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;
  Polygon example;
  if (!(in >> example))
  {
    throw std::invalid_argument("invalid perms polygon");
  }
  std::sort(example.points.begin(), example.points.end());
  std::vector< Polygon > sorted;
  sorted.reserve(polygons.size());
  auto equalToExample = std::bind(&Polygon::operator==, &example, _1);
  std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(sorted), getSorted);
  out << std::count_if(sorted.cbegin(), sorted.cend(), equalToExample) << '\n';
}

void ohantsev::rects(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << std::count_if(polygons.cbegin(), polygons.cend(), isRect) << '\n';
}

auto ohantsev::getVec(const Point& lhs, const Point& rhs) -> Point
{
  return Point{ rhs.x - lhs.x, rhs.y - lhs.y };
}

bool ohantsev::isOrthogonal(const Point& lhs, const Point& rhs)
{
  return (lhs.x * rhs.x + lhs.y * rhs.y) == 0;
}

bool ohantsev::isRect(const Polygon& polygon)
{
  static constexpr std::size_t RECT_VERTEX_COUNT = 4;
  if (polygon.size() != RECT_VERTEX_COUNT)
  {
    return false;
  }
  std::array< Point, RECT_VERTEX_COUNT > geomVects;
  auto vecFromBegin = polygon.points.cbegin();
  auto vecFromEnd = polygon.points.cend() - 1;
  auto vecToBegin = vecFromBegin + 1;
  std::transform(vecFromBegin, vecFromEnd, vecToBegin, geomVects.begin(), getVec);
  geomVects.back() = getVec(polygon.points.back(), polygon.points.front());
  std::array< bool, RECT_VERTEX_COUNT - 1 > orthogonal;
  auto firstVecBegin = geomVects.cbegin();
  auto firstVecEnd = geomVects.cend() - 1;
  auto secondVecBegin = firstVecBegin + 1;
  auto resBegin = orthogonal.begin();
  std::transform(firstVecBegin, firstVecEnd, secondVecBegin, resBegin, isOrthogonal);
  return std::find(orthogonal.cbegin(), orthogonal.cend(), true) != orthogonal.cend();
}
