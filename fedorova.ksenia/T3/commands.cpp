#include <functional>
#include <algorithm>
#include <iomanip>
#include <string>
#include <map>
#include <numeric>
#include "geom.h"
#include <io-guard.h>
#include "commands.h"


namespace
{
  using namespace fedorova;

  double subArea(const Point& a, const Point& b)
  {
    return a.x * b.y - a.y * b.x;
  }

  double areaPolygon(const Polygon& polygon)
  {
    const auto& p = polygon.points;
    double sum = std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, std::plus< double >{}, subArea);
    sum += subArea(p.back(), p.front());
    return std::abs(sum) / 2.0;
  }

  bool isEven(const Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const Polygon& polygon)
  {
    return !isEven(polygon);
  }

  bool isNum(const Polygon& polygon, size_t numOfVer)
  {
    return polygon.points.size() == numOfVer;
  }

  bool noCondition(const Polygon&)
  {
    return true;
  }

  template < typename T >
  double calcArea(const std::vector< Polygon >& polygons, T function)
  {
    std::vector< Polygon > newPolygons;
    std::vector< double > areas;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(newPolygons), function);
    std::transform(newPolygons.begin(), newPolygons.end(), std::back_inserter(areas), areaPolygon);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  bool areaComparator(const Polygon& a, const Polygon& b)
  {
    return areaPolygon(a) < areaPolygon(b);
  }

  bool verComparator(const Polygon& a, const Polygon& b)
  {
    return a.points.size() < b.points.size();
  }

  bool isPermutation(const Polygon& polygon1, const Polygon& polygon2)
  {
    if (polygon1.points.size() != polygon2.points.size())
    {
      return false;
    }
    return std::is_permutation(polygon1.points.begin(), polygon1.points.end(), polygon2.points.begin());
  }

  struct SideTriangleGenerator
  {
    const std::vector< Point >& points;
    size_t i;

    SideTriangleGenerator(const std::vector< Point >& pts);
    Polygon operator()();
  };

  SideTriangleGenerator::SideTriangleGenerator(const std::vector< Point >& pts):
    points(pts), i(1)
  {}

  Polygon SideTriangleGenerator::operator()()
  {
    Polygon triangle;
    triangle.points = { points[i - 1], points[i], points[i + 1] };
    ++i;
    return triangle;
  }

  bool isRightAngle(const Polygon& polygon)
  {
    auto side1 = polygon.points[1] - polygon.points[0];
    auto side2 = polygon.points[2] - polygon.points[1];
    return (side1.x * side2.x + side1.y * side2.y) == 0;
  }

  bool isThereRightAngleInPolygon(const Polygon& shape)
  {
    std::vector< Polygon > triangles(shape.points.size() - 2);
    std::generate(triangles.begin(), triangles.end(), SideTriangleGenerator{ shape.points });

    const auto n = shape.points.size();
    auto firstPolygon = Polygon{ { shape.points[n - 1], shape.points[0], shape.points[1] } };
    auto lastPolygon = Polygon{ { shape.points[n - 2], shape.points[n - 1], shape.points[0] } };
    triangles.push_back(std::move(firstPolygon));
    triangles.push_back(std::move(lastPolygon));

    return std::any_of(triangles.begin(), triangles.end(), isRightAngle);
  }
}

double fedorova::areaMean(const std::vector< Polygon >& polygons)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  return calcArea(polygons, noCondition) / polygons.size();
}

double fedorova::areaNum(const std::vector< Polygon >& polygons, size_t numOfVer)
{
  using namespace std::placeholders;
  auto function = std::bind(isNum, _1, numOfVer);
  return calcArea(polygons, function);
}

void fedorova::area(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
  std::string subCommand;
  is >> subCommand;
  IoGuard guard(os);
  os << std::fixed << std::setprecision(1);
  using namespace std::placeholders;
  using Predicate = std::function< bool(const Polygon&) >;

  std::map< std::string, std::function< double(const std::vector< Polygon >&) > > subcmds;
  subcmds["EVEN"] = std::bind(calcArea< Predicate >, _1, isEven);
  subcmds["ODD"] = std::bind(calcArea< Predicate >, _1, isOdd);
  subcmds["MEAN"] = areaMean;
  try
  {
    os << subcmds.at(subCommand)(polygons);
  }
  catch (const std::out_of_range&)
  {
    int numOfVer = std::stoull(subCommand);
    if (numOfVer < 3)
    {
      throw std::logic_error("<WRONG SUBCOMMAND>");
    }
    os << areaNum(polygons, numOfVer);
  }
}

void fedorova::maxArea(const std::vector< Polygon >& polygons, std::ostream& os)
{
  os << areaPolygon(*std::max_element(polygons.begin(), polygons.end(), areaComparator));
}

void fedorova::maxVertexes(const std::vector< Polygon >& polygons, std::ostream& os)
{
  os << (*std::max_element(polygons.begin(), polygons.end(), verComparator)).points.size();
}

void fedorova::max(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  std::string subcommand;
  is >> subcommand;
  IoGuard guard(os);
  os << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > subcmds;
  subcmds["AREA"] = maxArea;
  subcmds["VERTEXES"] = maxVertexes;
  subcmds.at(subcommand)(polygons, os);
}

void fedorova::minArea(const std::vector< Polygon >& polygons, std::ostream& os)
{
  os << areaPolygon(*std::min_element(polygons.begin(), polygons.end(), areaComparator));
}

void fedorova::minVertexes(const std::vector< Polygon >& polygons, std::ostream& os)
{
  os << (*std::min_element(polygons.begin(), polygons.end(), verComparator)).points.size();
}

void fedorova::min(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  std::string subcommand;
  is >> subcommand;
  IoGuard guard(os);
  os << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > subcmds;
  subcmds["AREA"] = minArea;
  subcmds["VERTEXES"] = minVertexes;
  subcmds.at(subcommand)(polygons, os);
}

size_t fedorova::countEven(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isEven);
}

size_t fedorova::countOdd(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isOdd);
}

size_t fedorova::countNum(const std::vector< Polygon >& polygons, size_t numOfVer)
{
  using namespace std::placeholders;
  return std::count_if(polygons.begin(), polygons.end(), std::bind(isNum, _1, numOfVer));
}

void fedorova::count(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
  std::string subCommand;
  is >> subCommand;
  IoGuard guard(os);
  os << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< size_t(const std::vector< Polygon >&) > > subcmds;
  subcmds["EVEN"] = countEven;
  subcmds["ODD"] = countOdd;
  try
  {
    os << subcmds.at(subCommand)(polygons);
  }
  catch (const std::out_of_range&)
  {
    size_t numOfVer = std::stoull(subCommand);
    if (numOfVer < 3)
    {
      throw std::logic_error("<WRONG SUBCOMMAND>");
    }
    os << countNum(polygons, numOfVer);
  }
}

void fedorova::perms(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
  Polygon polygon;
  is >> polygon;
  if (polygon.points.size() < 3)
  {
    throw std::logic_error("WRONG POLYGON SIZE");
  }
  using namespace std::placeholders;
  os << std::count_if(polygons.begin(), polygons.end(), std::bind(isPermutation, _1, polygon));
}

void fedorova::rightShapes(const std::vector< Polygon >& polygons, std::ostream& os)
{
  os << std::count_if(polygons.cbegin(), polygons.cend(), isThereRightAngleInPolygon);
  return;
}
