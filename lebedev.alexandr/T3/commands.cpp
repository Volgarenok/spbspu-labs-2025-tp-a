#include "commands.hpp"
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
#include <map>
#include <cmath>
#include <streamGuard.hpp>

namespace
{
  double calcPoints(const lebedev::Point & p1, const lebedev::Point & p2)
  {
    return p1.x * p2.y - p1.y * p2.x;
  }

  double getArea(const lebedev::Polygon & poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    lebedev::Polygon rotPoly = poly;
    std::rotate(rotPoly.points.begin(), rotPoly.points.begin() + 1, rotPoly.points.end());
    auto it1 = poly.points.begin();
    auto it2 = poly.points.end();
    auto it3 = rotPoly.points.begin();
    double sum = std::inner_product(it1, it2, it3, 0.0, std::plus< double >(), calcPoints);
    return std::abs(sum) * 0.5;
  }

  size_t getNum(const lebedev::Polygon & poly)
  {
    return poly.points.size();
  }

  bool isEvenPoly(const lebedev::Polygon & poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isOddPoly(const lebedev::Polygon & poly)
  {
    return poly.points.size() % 2 != 0;
  }

  bool hasNumVert(const lebedev::Polygon & poly, size_t num)
  {
    return poly.points.size() == num;
  }

  void getAreaEven(std::ostream & out, const std::vector< lebedev::Polygon > & polygons)
  {
    std::vector< lebedev::Polygon > temp;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), isEvenPoly);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    lebedev::StreamGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaOdd(std::ostream & out, const std::vector< lebedev::Polygon > & polygons)
  {
    std::vector< lebedev::Polygon > temp;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), isOddPoly);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    lebedev::StreamGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaMean(std::ostream & out, const std::vector< lebedev::Polygon > & polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("not polygons");
    }
    std::vector< lebedev::Polygon > temp = polygons;
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0) / polygons.size();
    lebedev::StreamGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaVertexes(std::ostream & out, const std::vector< lebedev::Polygon > & polygons, size_t n)
  {
    std::vector< lebedev::Polygon > temp;
    auto pred = std::bind(hasNumVert, std::placeholders::_1, n);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), pred);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    lebedev::StreamGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxArea(std::ostream & out, const std::vector< lebedev::Polygon > & polygons)
  {
    std::vector< double > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getArea);
    lebedev::StreamGuard fmtguard(out);
    double result = *std::max_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxVertexes(std::ostream & out, const std::vector< lebedev::Polygon > & polygons)
  {
    std::vector< size_t > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getNum);
    lebedev::StreamGuard fmtguard(out);
    size_t result = *std::max_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMinArea(std::ostream & out, const std::vector< lebedev::Polygon > & polygons)
  {
    std::vector< double > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getArea);
    lebedev::StreamGuard fmtguard(out);
    double result = *std::min_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMinVertexes(std::ostream & out, const std::vector< lebedev::Polygon > & polygons)
  {
    std::vector< size_t > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getNum);
    lebedev::StreamGuard fmtguard(out);
    size_t result = *std::min_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getCountEven(std::ostream & out, const std::vector< lebedev::Polygon > & polygons)
  {
    size_t result = std::count_if(polygons.begin(), polygons.end(), isEvenPoly);
    out << result << '\n';
  }

  void getCountOdd(std::ostream & out, const std::vector< lebedev::Polygon > & polygons)
  {
    size_t result = std::count_if(polygons.begin(), polygons.end(), isOddPoly);
    out << result << '\n';
  }

  void getCountVertexes(std::ostream & out, const std::vector< lebedev::Polygon > & polygons, size_t n)
  {
    auto pred = std::bind(hasNumVert, std::placeholders::_1, n);
    size_t result = std::count_if(polygons.begin(), polygons.end(), pred);
    lebedev::StreamGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  bool isPointOnSegment(const lebedev::Point &p1, const lebedev::Point &p2, const lebedev::Point &q)
  {
    return (q.x <= std::max(p1.x, p2.x) && q.x >= std::min(p1.x, p2.x) &&
        q.y <= std::max(p1.y, p2.y) && q.y >= std::min(p1.y, p2.y));
  }

  bool segmentsIntersect(const lebedev::Point &a1, const lebedev::Point &a2, const lebedev::Point &b1, const lebedev::Point &b2)
  {
    auto cross = [](const lebedev::Point &p1, const lebedev::Point &p2, const lebedev::Point &p3)
    {
      return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    };

    double d1 = cross(a1, a2, b1);
    double d2 = cross(a1, a2, b2);
    double d3 = cross(b1, b2, a1);
    double d4 = cross(b1, b2, a2);

    if ((d1 * d2 < 0) && (d3 * d4 < 0))
    {
      return true;
    }

    if ((d1 == 0 && isPointOnSegment(a1, a2, b1)) || (d2 == 0 && isPointOnSegment(a1, a2, b2))
        || (d3 == 0 && isPointOnSegment(b1, b2, a1)) || (d4 == 0 && isPointOnSegment(b1, b2, a2)))
    {
      return true;
    }
    return false;
  }

  bool polygonsIntersect(const lebedev::Polygon &a, const lebedev::Polygon &b)
  {
    for (size_t i = 0; i < a.points.size(); ++i)
    {
      lebedev::Point a1 = a.points[i];
      lebedev::Point a2 = a.points[(i + 1) % a.points.size()];
      for (size_t j = 0; j < b.points.size(); ++j)
      {
        lebedev::Point b1 = b.points[j];
        lebedev::Point b2 = b.points[(j + 1) % b.points.size()];
        if (segmentsIntersect(a1, a2, b1, b2))
        {
          return true;
        }
      }
    }
    return false;
  }
}

void lebedev::printArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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

void lebedev::printMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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

void lebedev::printMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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

void lebedev::printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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

void lebedev::printEcho(std::istream & in, std::ostream & out, std::vector< Polygon > & polygons)
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

void lebedev::printIntersections(std::istream &in, std::ostream &out, const std::vector<Polygon> &polygons)
{
  Polygon polygon;
  in >> polygon;

  if (!in || in.peek() != '\n')
  {
    throw std::logic_error("invalid input");
  }
  if (polygon.points.size() < 3)
  {
    throw std::logic_error("not enough points");
  }

  auto intersects = [&](const Polygon &p)
  {
    return polygonsIntersect(p, polygon);
  };
  size_t count = std::count_if(polygons.begin(), polygons.end(), intersects);
  out << count << "\n";
}
