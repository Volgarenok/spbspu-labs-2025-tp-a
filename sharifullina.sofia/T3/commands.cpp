#include "commands.hpp"
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>
#include <iomanip>
#include <scopeGuard.hpp>
#include "predicates.hpp"

namespace
{
  double getArea(const sharifullina::Polygon & poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    sharifullina::Polygon rotPoly = poly;
    std::rotate(rotPoly.points.begin(), rotPoly.points.begin() + 1, rotPoly.points.end());
    auto it1 = poly.points.begin();
    auto it2 = poly.points.end();
    auto it3 = rotPoly.points.begin();
    double sum = std::inner_product(it1, it2, it3, 0.0, std::plus< double >(), sharifullina::detail::calcPoints);
    return std::abs(sum) * 0.5;
  }

  size_t getNum(const sharifullina::Polygon & poly)
  {
    return poly.points.size();
  }

  bool isEvenPoly(const sharifullina::Polygon & poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isOddPoly(const sharifullina::Polygon & poly)
  {
    return poly.points.size() % 2 != 0;
  }

  bool hasNumVert(const sharifullina::Polygon & poly, size_t num)
  {
    return poly.points.size() == num;
  }

  void getAreaEven(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons)
  {
    std::vector< sharifullina::Polygon > temp;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), isEvenPoly);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    sharifullina::IofmtGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaOdd(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons)
  {
    std::vector< sharifullina::Polygon > temp;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), isOddPoly);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    sharifullina::IofmtGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaMean(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("not polygons");
    }
    std::vector< sharifullina::Polygon > temp = polygons;
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0) / polygons.size();
    sharifullina::IofmtGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaVertexes(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons, size_t n)
  {
    std::vector< sharifullina::Polygon > temp;
    auto pred = std::bind(hasNumVert, std::placeholders::_1, n);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), pred);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    sharifullina::IofmtGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxArea(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons)
  {
    std::vector< double > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getArea);
    sharifullina::IofmtGuard fmtguard(out);
    double result = *std::max_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxVertexes(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons)
  {
    std::vector< size_t > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getNum);
    sharifullina::IofmtGuard fmtguard(out);
    size_t result = *std::max_element(areaPoly.begin(), areaPoly.end());
    out << result << '\n';
  }

  void getMinArea(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons)
  {
    std::vector< double > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getArea);
    sharifullina::IofmtGuard fmtguard(out);
    double result = *std::min_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMinVertexes(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons)
  {
    std::vector< size_t > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getNum);
    sharifullina::IofmtGuard fmtguard(out);
    size_t result = *std::min_element(areaPoly.begin(), areaPoly.end());
    out << result << '\n';
  }

  void getCountEven(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons)
  {
    size_t result = std::count_if(polygons.begin(), polygons.end(), isEvenPoly);
    out << result << '\n';
  }

  void getCountOdd(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons)
  {
    size_t result = std::count_if(polygons.begin(), polygons.end(), isOddPoly);
    out << result << '\n';
  }

  void getCountVertexes(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons, size_t n)
  {
    auto pred = std::bind(hasNumVert, std::placeholders::_1, n);
    size_t result = std::count_if(polygons.begin(), polygons.end(), pred);
    out << result << '\n';
  }

  void getPerms(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons, const sharifullina::Polygon & target)
  {
    sharifullina::detail::IsPermutationPredicate predicate(target);
    size_t result = std::count_if(polygons.begin(), polygons.end(), predicate);
    out << result << '\n';
  }

  void getLessArea(std::ostream & out, const std::vector< sharifullina::Polygon > & polygons, const sharifullina::Polygon & target)
  {
    double targetArea = getArea(target);
    sharifullina::detail::LessAreaPredicate predicate(targetArea);
    size_t result = std::count_if(polygons.begin(), polygons.end(), predicate);
    out << result << '\n';
  }
}

void sharifullina::printArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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
  catch (const std::exception &)
  {
    size_t count = std::stoull(subcommand);
    if (count < 3)
    {
      throw;
    }
    getAreaVertexes(out, polygons, count);
  }
}

void sharifullina::printMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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
  subcmds.at(subcommand)();
}

void sharifullina::printMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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
  subcmds.at(subcommand)();
}

void sharifullina::printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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
  catch (const std::exception &)
  {
    size_t count = std::stoull(subcommand);
    if (count < 3)
    {
      throw;
    }
    getCountVertexes(out, polygons, count);
  }
}

void sharifullina::printPerms(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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
  getPerms(out, polygons, poly);
}

void sharifullina::printLessArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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
  getLessArea(out, polygons, poly);
}
