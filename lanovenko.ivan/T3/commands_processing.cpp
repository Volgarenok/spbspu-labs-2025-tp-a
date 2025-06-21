#include "commands_processing.hpp"

namespace
{
  double getSkewComposition(const lanovenko::Point& p1, const lanovenko::Point& p2)
  {
    return p1.x * p2.y - p2.x * p1.y;
  }

  double getArea(const lanovenko::Polygon& poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    lanovenko::Polygon rotatedLeft = poly;
    std::rotate(rotatedLeft.points.begin(), rotatedLeft.points.begin() + 1, rotatedLeft.points.end());
    auto first1 = poly.points.begin();
    auto last1 = poly.points.end();
    auto first2 = rotatedLeft.points.begin();
    double orientedArea = std::inner_product(first1, last1, first2, double{}, std::plus < double >{}, getSkewComposition);
    return std::abs(orientedArea) / 2.0;
  }

  size_t getVertices(const lanovenko::Polygon& rhs) noexcept
  {
    return rhs.points.size();
  }

  bool evenVertices(const lanovenko::Polygon& rhs) noexcept
  {
    return rhs.points.size() % 2 == 0;
  }

  bool oddVertices(const lanovenko::Polygon& rhs) noexcept
  {
    return rhs.points.size() % 2 != 0;
  }

  bool compareVerticesCount(const lanovenko::Polygon& rhs, size_t num) noexcept
  {
    return rhs.points.size() == num;
  }

  void getAreaEven(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs)
  {
    std::vector< lanovenko::Polygon > temp;
    std::copy_if(rhs.begin(), rhs.end(), std::back_inserter(temp), evenVertices);
    std::vector< double > area(temp.size());
    std::transform(temp.begin(), temp.end(), area.begin(), getArea);
    double res = std::accumulate(area.begin(), area.end(), double{});
    lanovenko::StreamGuard a{ out };
    out << std::fixed << std::setprecision(1) << res << '\n';
  }

  void getAreaOdd(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs)
  {
    std::vector< lanovenko::Polygon > temp;
    std::copy_if(rhs.begin(), rhs.end(), std::back_inserter(temp), oddVertices);
    std::vector< double > areas(temp.size());
    std::transform(temp.begin(), temp.end(), areas.begin(), getArea);
    double res = std::accumulate(areas.begin(), areas.end(), double{});
    lanovenko::StreamGuard a{ out };
    out << std::fixed << std::setprecision(1) << res << '\n';
  }

  void getAreaMean(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs)
  {
    if (rhs.empty())
    {
      throw std::logic_error("Аt least one figure must be");
    }
    std::vector< double > areas(rhs.size());
    std::transform(rhs.begin(), rhs.end(), areas.begin(), getArea);
    double res = std::accumulate(areas.begin(), areas.end(), double{}) / areas.size();
    lanovenko::StreamGuard a{ out };
    out << std::fixed << std::setprecision(1) << res << '\n';
  }

  void getAreaVertexes(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs, size_t n)
  {
    std::vector< lanovenko::Polygon > temp;
    auto pred = std::bind(compareVerticesCount, std::placeholders::_1, n);
    std::copy_if(rhs.begin(), rhs.end(), std::back_inserter(temp), pred);
    std::vector< double > areas(temp.size());
    std::transform(temp.begin(), temp.end(), areas.begin(), getArea);
    double res = std::accumulate(areas.begin(), areas.end(), double{});
    lanovenko::StreamGuard a{ out };
    out << std::fixed << std::setprecision(1) << res << '\n';
  }

  void getMaxArea(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs)
  {
    if (rhs.empty())
    {
      throw std::logic_error("Аt least one figure must be");
    }
    std::vector< double > areas(rhs.size());
    std::transform(rhs.begin(), rhs.end(), areas.begin(), getArea);
    lanovenko::StreamGuard a{ out };
    double res = *std::max_element(areas.begin(), areas.end());
    out << std::fixed << std::setprecision(1) << res << '\n';
  }

  void getMaxVertexes(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs)
  {
    if (rhs.empty())
    {
      throw std::logic_error("Аt least one figure must be");
    }
    std::vector< size_t > vertices(rhs.size());
    std::transform(rhs.begin(), rhs.end(), vertices.begin(), getVertices);
    size_t res = *std::max_element(vertices.begin(), vertices.end());
    out << res << '\n';
  }

  void getMinArea(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs)
  {
    if (rhs.empty())
    {
      throw std::logic_error("Аt least one figure must be");
    }
    std::vector< double > areas(rhs.size());
    std::transform(rhs.begin(), rhs.end(), areas.begin(), getArea);
    lanovenko::StreamGuard a{ out };
    double res = *std::min_element(areas.begin(), areas.end());
    out << std::fixed << std::setprecision(1) << res << '\n';
  }

  void getMinVertexes(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs)
  {
    if (rhs.empty())
    {
      throw std::logic_error("Аt least one figure must be");
    }
    std::vector< size_t > vertices(rhs.size());
    std::transform(rhs.begin(), rhs.end(), vertices.begin(), getVertices);
    size_t res = *std::min_element(vertices.begin(), vertices.end());
    out << res << '\n';
  }

  void countEven(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs)
  {
    size_t res = std::count_if(rhs.begin(), rhs.end(), evenVertices);
    out << res << '\n';
  }

  void countOdd(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs)
  {
    size_t res = std::count_if(rhs.begin(), rhs.end(), oddVertices);
    out << res << '\n';
  }

  void getCountVertexes(std::ostream& out, const std::vector< lanovenko::Polygon >& rhs, size_t n)
  {
    auto pred = std::bind(compareVerticesCount, std::placeholders::_1, n);
    size_t res = std::count_if(rhs.begin(), rhs.end(), pred);
    out << res << '\n';
  }

  bool comparePolygonsWithExp(const lanovenko::Polygon& a, const lanovenko::Polygon& b, const lanovenko::Polygon& pattern)
  {
    return (a == pattern) && (b == pattern);
  }

  lanovenko::Point getSide(const lanovenko::Point& point1, const lanovenko::Point& point2)
  {
    return { point1 - point2 };
  }

  bool isRightAngle(const lanovenko::Point& point1, const lanovenko::Point& point2)
  {
    return (point1.x * point2.x + point1.y * point2.y) == 0;
  }

  bool haveRightAngles(const lanovenko::Polygon& polygon)
  {
    std::vector< lanovenko::Point > sides;
    auto begin = polygon.points.begin(), end = polygon.points.end();
    std::transform(begin + 1, end, begin, std::back_inserter(sides), getSide);
    sides.push_back(getSide(polygon.points.front(), polygon.points.back()));
    sides.push_back(sides.front());
    std::vector< bool > isRightShape;
    std::transform(sides.begin() + 1, sides.end(), sides.begin(), std::back_inserter(isRightShape), isRightAngle);
    return std::accumulate(isRightShape.begin(), isRightShape.end(), 0) > 0;
  }

}

void lanovenko::printArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& rhs)
{
  std::map< std::string, std::function< void() > > commands;
  commands["EVEN"] = std::bind(getAreaEven, std::ref(out), std::cref(rhs));
  commands["ODD"] = std::bind(getAreaOdd, std::ref(out), std::cref(rhs));
  commands["MEAN"] = std::bind(getAreaMean, std::ref(out), std::cref(rhs));
  std::string command;
  in >> command;
  try
  {
    commands.at(command)();
  }
  catch (...)
  {
    size_t count = std::stoull(command);
    if (count < 3)
    {
      throw std::logic_error("Wrong command");
    }
    getAreaVertexes(out, rhs, count);
  }
}

void lanovenko::printMax(std::istream& in, std::ostream& out, const std::vector< Polygon >& rhs)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMaxArea, std::ref(out), std::cref(rhs));
  subcmds["VERTEXES"] = std::bind(getMaxVertexes, std::ref(out), std::cref(rhs));
  std::string subcommand;
  in >> subcommand;
  try
  {
    subcmds.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("Wrond command");
  }
}

void lanovenko::printMin(std::istream& in, std::ostream& out, const std::vector< Polygon >& rhs)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMinArea, std::ref(out), std::cref(rhs));
  subcmds["VERTEXES"] = std::bind(getMinVertexes, std::ref(out), std::cref(rhs));
  std::string subcommand;
  in >> subcommand;
  try
  {
    subcmds.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("Wrong command");
  }
}

void lanovenko::printCount(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(countEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(countOdd, std::ref(out), std::cref(polygons));
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
      throw std::logic_error("Wrong command");
    }
    getCountVertexes(out, polygons, count);
  }
}

void lanovenko::printRmecho(std::istream& in, std::ostream& out, std::vector< Polygon >& rhs)
{
  using namespace std::placeholders;
  Polygon current;
  if (!(in >> current))
  {
    throw std::logic_error("Wrong input");
  }
  auto pred = std::bind(comparePolygonsWithExp, _1, _2, current);
  auto del = std::unique(rhs.begin(), rhs.end(), pred);
  size_t diff = rhs.size();
  rhs.erase(del, rhs.end());
  diff -= rhs.size();
  out << diff;
}

void lanovenko::printRightShapes(std::ostream& out, const std::vector< Polygon >& polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), haveRightAngles) << '\n';
}
