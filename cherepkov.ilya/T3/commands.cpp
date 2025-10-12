#include "commands.hpp"
#include <algorithm>
#include <exception>
#include <string>

namespace
{
  double calcPoints(const cherepkov::Point & p1, const cherepkov::Point & p2)
  {
    return p1.x * p2.y - p1.y * p2.x;
  }

  double getArea(const cherepkov::Polygon & poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    cherepkov::Polygon rotPoly = poly;
    std::rotate(rotPoly.points.begin(), rotPoly.points.begin() + 1, rotPoly.points.end());
    auto it1 = poly.points.begin();
    auto it2 = poly.points.end();
    auto it3 = rotPoly.points.begin();
    double sum = std::inner_product(it1, it2, it3, 0.0, std::plus< double >(), calcPoints);
    return std::abs(sum) * 0.5;
  }

  bool hasEvenVertexCount(const cherepkov::Polygon & poly)
  {
    return poly.points.size() % 2 == 0;
  }

  void getAreaEven(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons)
  {
    std::vector< cherepkov::Polygon > temp;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), hasEvenVertexCount);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    cherepkov::StreamGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  bool isOddPoly(const cherepkov::Polygon & poly)
  {
    return poly.points.size() % 2 != 0;
  }

  void getAreaOdd(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons)
  {
    std::vector< cherepkov::Polygon > temp;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), isOddPoly);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    cherepkov::StreamGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaMean(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("not polygons");
    }
    std::vector< cherepkov::Polygon > temp = polygons;
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0) / polygons.size();
    cherepkov::StreamGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  bool hasNumVert(const cherepkov::Polygon & poly, size_t n)
  {
    return poly.points.size() == n;
  }

  void getAreaVertexes(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons, size_t n)
  {
    std::vector< cherepkov::Polygon > temp;
    auto pred = std::bind(hasNumVert, std::placeholders::_1, n);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), pred);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    cherepkov::StreamGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxArea(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons)
  {
    std::vector< double > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getArea);
    cherepkov::StreamGuard fmtguard(out);
    double result = *std::max_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  size_t getNum(const cherepkov::Polygon & poly)
  {
    return poly.points.size();
  }

  void getMaxVertexes(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons)
  {
    std::vector< size_t > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getNum);
    cherepkov::StreamGuard fmtguard(out);
    size_t result = *std::max_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMinArea(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons)
  {
    std::vector< double > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getArea);
    cherepkov::StreamGuard fmtguard(out);
    double result = *std::min_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMinVertexes(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons)
  {
    std::vector< size_t > areaPoly(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areaPoly.begin(), getNum);
    cherepkov::StreamGuard fmtguard(out);
    size_t result = *std::min_element(areaPoly.begin(), areaPoly.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getCountEven(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons)
  {
    size_t result = std::count_if(polygons.begin(), polygons.end(), hasEvenVertexCount);
    out << result << '\n';
  }

  void getCountOdd(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons)
  {
    size_t result = std::count_if(polygons.begin(), polygons.end(), isOddPoly);
    out << result << '\n';
  }

  void getCountVertexes(std::ostream & out, const std::vector< cherepkov::Polygon > & polygons, size_t n)
  {
    auto pred = std::bind(hasNumVert, std::placeholders::_1, n);
    size_t result = std::count_if(polygons.begin(), polygons.end(), pred);
    cherepkov::StreamGuard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  bool cmpPoint(const cherepkov::Point & p1, const cherepkov::Point & p2)
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

  double crossProduct(const cherepkov::Point & a, const cherepkov::Point & b, const cherepkov::Point & c)
  {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
  }
  bool isCounterClockwise(const cherepkov::Polygon & polygon)
  {
    return crossProduct(polygon.points[0], polygon.points[1], polygon.points[2]) > 0;
  }

  void sortPolygon(cherepkov::Polygon & poly)
  {
    auto itMin = std::min_element(poly.points.begin(), poly.points.end(), cmpPoint);
    std::rotate(poly.points.begin(), itMin, poly.points.end());
    if (isCounterClockwise(poly))
    {
      std::reverse(poly.points.begin() + 1, poly.points.end());
    }
  }

  struct iSame
  {
    const cherepkov::Polygon & poly;
    bool operator()(const cherepkov::Polygon & temp) const
    {
      if (temp.points.size() != poly.points.size())
      {
        return false;
      }
      cherepkov::Polygon sortTemp = temp;
      sortPolygon(sortTemp);
      std::vector< cherepkov::Point > diffsPoly;
      std::vector< cherepkov::Point > diffsTemp;
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

  void inFrameOutput(std::ostream& out, bool res)
  {
    if (res)
    {
      out << "<TRUE>\n";
      return;
    }
    out << "<FALSE>\n";
  }

  bool compareXPoint(const cherepkov::Point& a, const cherepkov::Point& b)
  {
    return a.x < b.x;
  }

  bool compareYPoint(const cherepkov::Point& a, const cherepkov::Point& b)
  {
    return a.y < b.y;
  }

  int findMaxXPolygon(const cherepkov::Polygon& polygon)
  {
    auto res = std::max_element(polygon.points.begin(), polygon.points.end(), compareXPoint);
    return (*res).x;
  }

  int findMaxYPolygon(const cherepkov::Polygon& polygon)
  {
    auto res = std::max_element(polygon.points.begin(), polygon.points.end(), compareYPoint);
    return (*res).y;
  }

  int findMinXPolygon(const cherepkov::Polygon& polygon)
  {
    auto res = std::min_element(polygon.points.begin(), polygon.points.end(), compareXPoint);
    return (*res).x;
  }

  int findMinYPolygon(const cherepkov::Polygon& polygon)
  {
    auto res = std::min_element(polygon.points.begin(), polygon.points.end(), compareYPoint);
    return (*res).y;
  }

  bool compareMaxXPolygon(const cherepkov::Polygon& polygon1, const cherepkov::Polygon& polygon2)
  {
    return findMaxXPolygon(polygon1) < findMaxXPolygon(polygon2);
  }

  bool compareMaxYPolygon(const cherepkov::Polygon& polygon1, const cherepkov::Polygon& polygon2)
  {
    return findMaxYPolygon(polygon1) < findMaxYPolygon(polygon2);
  }

  bool compareMinXPolygon(const cherepkov::Polygon& polygon1, const cherepkov::Polygon& polygon2)
  {
    return findMinXPolygon(polygon1) < findMinXPolygon(polygon2);
  }

  bool compareMinYPolygon(const cherepkov::Polygon& polygon1, const cherepkov::Polygon& polygon2)
  {
    return findMinYPolygon(polygon1) < findMinYPolygon(polygon2);
  }

  int findMaxXVector(const std::vector< cherepkov::Polygon >& polygons)
  {
    auto res = std::max_element(polygons.begin(), polygons.end(), compareMaxXPolygon);
    return findMaxXPolygon(*res);
  }

  int findMaxYVector(const std::vector< cherepkov::Polygon >& polygons)
  {
    auto res = std::max_element(polygons.begin(), polygons.end(), compareMaxYPolygon);
    return findMaxYPolygon(*res);
  }

  int findMinXVector(const std::vector< cherepkov::Polygon >& polygons)
  {
    auto res = std::min_element(polygons.begin(), polygons.end(), compareMinXPolygon);
    return findMinXPolygon(*res);
  }

  int findMinYVector(const std::vector< cherepkov::Polygon >& polygons)
  {
    auto res = std::min_element(polygons.begin(), polygons.end(), compareMinYPolygon);
    return findMinYPolygon(*res);
  }
}

void cherepkov::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

void cherepkov::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

void cherepkov::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

void cherepkov::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

void cherepkov::inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon framed;
  in >> framed;
  if (!(in >> framed))
  {
    throw std::logic_error("incorrect input");
  }

  if (in.peek() != '\n' && in.peek() != EOF)
  {
    throw std::logic_error("incorrect input");
  }

  if (polygons.empty())
  {
    throw std::logic_error("not polygons");
  }

  int max_framed_x = findMaxXPolygon(framed);
  int max_framed_y = findMaxYPolygon(framed);
  int min_framed_x = findMinXPolygon(framed);
  int min_framed_y = findMinYPolygon(framed);

  int max_x = findMaxXVector(polygons);
  int max_y = findMaxYVector(polygons);
  int min_x = findMinXVector(polygons);
  int min_y = findMinYVector(polygons);

  bool res = max_framed_x <= max_x && max_framed_y <= max_y && min_framed_x >= min_x && min_framed_y >= min_y;
  inFrameOutput(out, res);
}

void cherepkov::same(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

  if (in.peek() != '\n' && in.peek() != EOF)
  {
    throw std::logic_error("incorrect input");
  }

  sortPolygon(poly);
  iSame cmp{poly};
  out << std::count_if(polygons.begin(), polygons.end(), cmp) << '\n';
}
