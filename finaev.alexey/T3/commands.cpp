#include "commands.hpp"
#include <iomanip>
#include <streamGuard.hpp>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>

namespace
{
  bool isOdd(const finaev::Polygon& poly)
  {
    return poly.points.size() % 2 != 0;
  }

  bool isEven(const finaev::Polygon& poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isNumOfVertex(const finaev::Polygon& poly, size_t s)
  {
    return poly.points.size() == s;
  }

  size_t countOdd(const std::vector< finaev::Polygon >& shapes)
  {
    return std::count_if(shapes.begin(), shapes.end(), isOdd);
  }

  size_t countEven(const std::vector< finaev::Polygon >& shapes)
  {
    return std::count_if(shapes.begin(), shapes.end(), isEven);
  }

  size_t countVertexes(const std::vector< finaev::Polygon >& shapes, size_t num)
  {
    auto compare = std::bind(isNumOfVertex, std::placeholders::_1, num);
    return std::count_if(shapes.begin(), shapes.end(), compare);
  }

  struct PolygonArea
  {
    const std::vector< finaev::Point >& points;
    size_t size;

    double operator()(double sum, size_t i) const
    {
      size_t j = (i + 1) % size;
      return sum + (points[i].x * points[j].y) - (points[i].y * points[j].x);
    }
  };

  double areaOnePoly(const finaev::Polygon& poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    size_t s = poly.points.size();
    std::vector< size_t > indexes(s);
    std::iota(indexes.begin(), indexes.end(), 0);
    double res = std::accumulate(indexes.begin(), indexes.end(), 0.0, PolygonArea{ poly.points, s });
    return std::abs(res) / 2.0;
  }

  struct AreaAccumulator
  {
    double operator()(double sum, const finaev::Polygon& poly) const
    {
      return sum + areaOnePoly(poly);
    }
  };

  double areaOdd(const std::vector< finaev::Polygon >& shapes)
  {
    std::vector< finaev::Polygon > oddShapes;
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(oddShapes), isOdd);
    return std::accumulate(oddShapes.begin(), oddShapes.end(), 0.0, AreaAccumulator{});
  }

  double areaEven(const std::vector< finaev::Polygon >& shapes)
  {
    std::vector< finaev::Polygon > evenShapes;
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(evenShapes), isEven);
    return std::accumulate(evenShapes.begin(), evenShapes.end(), 0.0, AreaAccumulator{});
  }

  double areaMean(const std::vector< finaev::Polygon >& shapes)
  {
    if (shapes.empty())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    double res = std::accumulate(shapes.begin(), shapes.end(), 0.0, AreaAccumulator{});
    return res / shapes.size();
  }

  double areaVertexes(const std::vector< finaev::Polygon >& shapes, size_t num)
  {
    std::vector< finaev::Polygon > filteredShapes;
    auto isVertexNum = std::bind(isNumOfVertex, std::placeholders::_1, num);
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(filteredShapes), isVertexNum);
    return std::accumulate(filteredShapes.begin(), filteredShapes.end(), 0.0, AreaAccumulator{});
  }

  bool compareVertex(const finaev::Polygon& poly1, const finaev::Polygon& poly2)
  {
    return poly1.points.size() < poly2.points.size();
  }

  bool compareArea(const finaev::Polygon& poly1, const finaev::Polygon& poly2)
  {
    return areaOnePoly(poly1) < areaOnePoly(poly2);
  }

  bool isDigit(const std::string& s)
  {
    return !s.empty() && std::all_of(s.begin(), s.end(), isdigit);
  }

  void maxArea(const std::vector< finaev::Polygon >& shapes, std::ostream& out)
  {
    auto max = (*std::max_element(shapes.begin(), shapes.end(), compareArea));
    finaev::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << areaOnePoly(max);
  }

  void minArea(const std::vector< finaev::Polygon >& shapes, std::ostream& out)
  {
    auto min = (*std::max_element(shapes.begin(), shapes.end(), compareArea));
    finaev::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << areaOnePoly(min);
  }

  void maxVertexes(const std::vector< finaev::Polygon >& shapes, std::ostream& out)
  {
    auto max = (*std::max_element(shapes.begin(), shapes.end(), compareVertex));
    out << max.points.size();
  }

  void minVertexes(const std::vector< finaev::Polygon >& shapes, std::ostream& out)
  {
    auto min = (*std::min_element(shapes.begin(), shapes.end(), compareVertex));
    out << min.points.size();
  }

  struct IsOffsetEqual
  {
    int dx;
    int dy;

    bool operator()(const finaev::Point& a, const finaev::Point& b) const
    {
      return b.x == a.x + dx && b.y == a.y + dy;
    }
  };

  bool isSame(const finaev::Polygon& first, const finaev::Polygon& second)
  {
    if (first.points.size() != second.points.size())
    {
      return false;
    }
    int dx = second.points[0].x - first.points[0].x;
    int dy = second.points[0].y - first.points[0].y;
    return std::equal(first.points.begin() + 1, first.points.end(), second.points.begin() + 1, IsOffsetEqual{dx, dy});
  }

  bool isExactSame(const finaev::Polygon& first, const finaev::Polygon& second)
  {
    bool firstCondition = (first.points.size() == second.points.size());
    bool secondCondition = (std::equal(first.points.begin(), first.points.end(), second.points.begin()));
    return firstCondition && secondCondition;
  }
}

void finaev::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  std::string substr;
  in >> substr;
  std::map< std::string, std::function< size_t() > > cmd;
  cmd["ODD"] = std::bind(countOdd, std::cref(shapes));
  cmd["EVEN"] = std::bind(countEven, std::cref(shapes));
  try
  {
    if (isDigit(substr))
    {
      size_t s = std::stoul(substr);
      if (s < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      out << countVertexes(shapes, s);
    }
    else
    {
      out << cmd.at(substr)();
    }
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void finaev::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  std::string substr;
  in >> substr;
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  std::map< std::string, std::function< double() > > cmd;
  cmd["ODD"] = std::bind(areaOdd, std::cref(shapes));
  cmd["EVEN"] = std::bind(areaEven, std::cref(shapes));
  cmd["MEAN"] = std::bind(areaMean, std::cref(shapes));
  try
  {
    if (isDigit(substr))
    {
      size_t s = std::stoul(substr);
      if (s < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      out << areaVertexes(shapes, s);
    }
    else
    {
      out << cmd.at(substr)();
    }
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void finaev::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  if (shapes.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  std::string substr;
  in >> substr;
  std::map< std::string, std::function< void() > > cmd;
  cmd["AREA"] = std::bind(maxArea, std::cref(shapes), std::ref(out));
  cmd["VERTEXES"] = std::bind(maxVertexes, std::cref(shapes), std::ref(out));
  try
  {
    cmd.at(substr)();
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void finaev::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  if (shapes.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  std::string substr;
  in >> substr;
  std::map< std::string, std::function< void() > > cmd;
  cmd["AREA"] = std::bind(minArea, std::cref(shapes), std::ref(out));
  cmd["VERTEXES"] = std::bind(minVertexes, std::cref(shapes), std::ref(out));
  try
  {
    cmd.at(substr)();
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void finaev::echo(std::istream& in, std::ostream& out, std::vector< Polygon >& shapes)
{
  Polygon poly;
  std::string temp;
  in >> poly;
  if (in.fail() || !in)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  if (in.peek() != '\n')
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  size_t countAdd = 0;
  std::vector< Polygon > res;
  for (auto it = shapes.begin(); it != shapes.end(); ++it)
  {
    res.push_back(*it);
    if (isExactSame(*it, poly))
    {
      res.push_back(*it);
      ++countAdd;
    }
  }
  shapes = std::move(res);
  out << countAdd;
}

void finaev::same(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  Polygon poly;
  std::string temp;
  in >> poly;
  if (in.fail() || !in)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  if (in.peek() != '\n')
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  auto same = std::bind(isSame, poly, std::placeholders::_1);
  out << std::count_if(shapes.begin(), shapes.end(), same);
}

using CmdMap = std::map< std::string, std::function< void() > >;
CmdMap finaev::createCommandsHandler(std::istream& in, std::ostream& out, std::vector< Polygon >& shapes)
{
  CmdMap commands;
  commands["AREA"] = std::bind(finaev::area, std::ref(in), std::ref(out), std::cref(shapes));
  commands["COUNT"] = std::bind(finaev::count, std::ref(in), std::ref(out), std::cref(shapes));
  commands["MAX"] = std::bind(finaev::max, std::ref(in), std::ref(out), std::cref(shapes));
  commands["MIN"] = std::bind(finaev::min, std::ref(in), std::ref(out), std::cref(shapes));
  commands["SAME"] = std::bind(finaev::same, std::ref(in), std::ref(out), std::cref(shapes));
  commands["ECHO"] = std::bind(finaev::echo, std::ref(in), std::ref(out), std::ref(shapes));
  return commands;
}
