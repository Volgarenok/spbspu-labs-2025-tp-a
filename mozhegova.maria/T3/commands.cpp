#include "commands.hpp"
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>
#include <scopeGuard.hpp>

namespace
{
  double calcPoints(const mozhegova::Point & p1, const mozhegova::Point & p2)
  {
    return p1.x * p2.y - p1.y * p2.x;
  }

  double getArea(const mozhegova::Polygon & poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    mozhegova::Polygon rotPoly = poly;
    std::rotate(rotPoly.points.begin(), rotPoly.points.begin() + 1, rotPoly.points.end());
    auto it1 = poly.points.begin();
    auto it2 = poly.points.end();
    auto it3 = rotPoly.points.begin();
    double sum = std::inner_product(it1, it2, it3, 0.0, std::plus< double >(), calcPoints);
    return std::abs(sum) * 0.5;
  }

  size_t getNum(const mozhegova::Polygon & poly)
  {
    return poly.points.size();
  }

  bool isEvenPoly(const mozhegova::Polygon & poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isOddPoly(const mozhegova::Polygon & poly)
  {
    return poly.points.size() % 2 != 0;
  }

  bool hasNumVert(const mozhegova::Polygon & poly, size_t num)
  {
    return poly.points.size() == num;
  }

  void getAreaEven(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    std::vector< mozhegova::Polygon > temp;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), isEvenPoly);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    mozhegova::iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaOdd(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    std::vector< mozhegova::Polygon > temp;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), isOddPoly);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    mozhegova::iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaMean(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("not polygons");
    }
    std::vector< mozhegova::Polygon > temp = polygons;
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0) / polygons.size();
    mozhegova::iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaVertexes(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons, size_t n)
  {
    std::vector< mozhegova::Polygon > temp;
    auto pred = std::bind(hasNumVert, std::placeholders::_1, n);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), pred);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    mozhegova::iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxArea(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    std::vector< double > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getArea);
    mozhegova::iofmtguard fmtguard(out);
    double result = *std::max_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxVertexes(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    std::vector< size_t > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getNum);
    mozhegova::iofmtguard fmtguard(out);
    size_t result = *std::max_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMinArea(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    std::vector< double > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getArea);
    mozhegova::iofmtguard fmtguard(out);
    double result = *std::min_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMinVertexes(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    std::vector< size_t > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getNum);
    mozhegova::iofmtguard fmtguard(out);
    size_t result = *std::min_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getCountEven(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    size_t result = std::count_if(polygons.begin(), polygons.end(), isEvenPoly);
    out << result << '\n';
  }

  void getCountOdd(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    size_t result = std::count_if(polygons.begin(), polygons.end(), isOddPoly);
    out << result << '\n';
  }

  void getCountVertexes(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons, size_t n)
  {
    auto pred = std::bind(hasNumVert, std::placeholders::_1, n);
    size_t result = std::count_if(polygons.begin(), polygons.end(), pred);
    mozhegova::iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  bool cmpPoint(const mozhegova::Point & p1, const mozhegova::Point & p2)
  {
    if (p1.x != p2.x)
    {
      return p1.x < p2.x;
    }
    else
    {
      return p1.y < p2.y;
    }
  }

  double crossProduct(const mozhegova::Point & a, const mozhegova::Point & b, const mozhegova::Point & c)
  {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
  }

  bool isCounterClockwise(const mozhegova::Polygon & polygon)
  {
    return crossProduct(polygon.points[0], polygon.points[1], polygon.points[2]) > 0;
  }

  void sortPolygon(mozhegova::Polygon & poly)
  {
    auto itMin = std::min_element(poly.points.begin(), poly.points.end(), cmpPoint);
    std::rotate(poly.points.begin(), itMin, poly.points.end());
    if (isCounterClockwise(poly))
    {
      std::reverse(poly.points.begin() + 1, poly.points.end());
    }
  }

  struct isSame
  {
    const mozhegova::Polygon & poly;
    bool operator()(const mozhegova::Polygon & temp) const
    {
      if (temp.points.size() != poly.points.size())
      {
        return false;
      }
      mozhegova::Polygon sortTemp = temp;
      sortPolygon(sortTemp);
      std::vector< mozhegova::Point > diffsPoly;
      std::vector< mozhegova::Point > diffsTemp;
      std::adjacent_difference(poly.points.begin(), poly.points.end(), std::back_inserter(diffsPoly));
      std::adjacent_difference(sortTemp.points.begin(), sortTemp.points.end(), std::back_inserter(diffsTemp));
      diffsTemp[0] = diffsPoly[0];
      if (diffsPoly == diffsTemp)
      {
        return true;
      }
      return false;
    }
  };
}

void mozhegova::printArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getAreaEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(getAreaOdd, std::ref(out), std::cref(polygons));
  subcmds["MEAN"] = std::bind(getAreaMean, std::ref(out), std::cref(polygons));
  std::string subcommand;
  in >> subcommand;
  try
  {
    subcmds.at(subcommand)();
  }
  catch (...)
  {
    size_t count = std::stoull(subcommand);
    if (count < 3)
    {
      throw std::logic_error("not that command");
    }
    getAreaVertexes(out, polygons, count);
  }
}

void mozhegova::printMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("not polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMaxArea, std::ref(out), std::cref(polygons));
  subcmds["VERTEXES"] = std::bind(getMaxVertexes, std::ref(out), std::cref(polygons));
  std::string subcommand;
  in >> subcommand;
  try
  {
    subcmds.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("not that command");
  }
}

void mozhegova::printMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("not polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMinArea, std::ref(out), std::cref(polygons));
  subcmds["VERTEXES"] = std::bind(getMinVertexes, std::ref(out), std::cref(polygons));
  std::string subcommand;
  in >> subcommand;
  try
  {
    subcmds.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("not that command");
  }
}

void mozhegova::printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getCountEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(getCountOdd, std::ref(out), std::cref(polygons));
  std::string subcommand;
  in >> subcommand;
  try
  {
    subcmds.at(subcommand)();
  }
  catch (...)
  {
    size_t count = std::stoull(subcommand);
    if (count < 3)
    {
      throw std::logic_error("not that command");
    }
    getCountVertexes(out, polygons, count);
  }
}

void mozhegova::printEcho(std::istream & in, std::ostream & out, std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("not polygons");
  }
  Polygon poly;
  if (!(in >> poly))
  {
    throw std::logic_error("incorrect input");
  }
  if (in.peek() != '\n')
  {
    throw std::logic_error("incorrect input");
  }
  std::vector< Polygon > tempPolygons = polygons;
  size_t count = 0;
  auto it = std::find(polygons.begin(), polygons.end(), poly);
  while (it != polygons.end())
  {
    ++count;
    tempPolygons.insert(std::next(tempPolygons.begin(), std::distance(polygons.begin(), it) + count), poly);
    it = std::find(std::next(it), polygons.end(), poly);
  }
  polygons = std::move(tempPolygons);
  out << count << '\n';
}

void mozhegova::printSame(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("not polygons");
  }
  Polygon poly;
  if (!(in >> poly) || in.peek() != '\n')
  {
    throw std::logic_error("incorrect input");
  }
  sortPolygon(poly);
  isSame cmp{poly};
  out << std::count_if(polygons.begin(), polygons.end(), cmp) << '\n';
}
