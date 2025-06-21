#include "commands.hpp"
#include <iomanip>
#include <streamGuard.hpp>
#include <algorithm>
#include <functional>
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
  double areaOnePoly(const finaev::Polygon& poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    double res = 0.0;
    size_t s = poly.points.size();
    for (size_t i = 0; i < s; ++i)
    {
      int j = (i + 1) % s;
      res += (poly.points[i].x * poly.points[j].y) - (poly.points[i].y * poly.points[j].x);
    }
    return std::abs(res) / 2.0;
  }
  bool isDigit(const std::string& s)
  {
    return !s.empty() && std::all_of(s.begin(), s.end(), isdigit);
  }
  size_t getCountVertex(const finaev::Polygon& poly)
  {
    return poly.points.size();
  }
  bool isSame(const finaev::Polygon& first, const finaev::Polygon& second)
  {
    if (first.points.size() != second.points.size())
    {
      return false;
    }
    int dx = second.points[0].x - first.points[0].x;
    int dy = second.points[0].y - first.points[0].y;
    for (size_t i = 1; i < first.points.size(); ++i)
    {
      if (second.points[i].x != first.points[i].x + dx || second.points[i].y != first.points[i].y + dy)
      {
        return false;
      }
    }
    return true;
  }
  bool isExactSame(const finaev::Polygon& first, const finaev::Polygon& second)
  {
    if (first.points.size() != second.points.size())
    {
      return false;
    }
    for (size_t i = 0; i < first.points.size(); ++i)
    {
      if (first.points[i].x != second.points[i].x || first.points[i].y != second.points[i].y)
      {
        return false;
      }
    }
    return true;
  }
}

void finaev::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  std::string substr;
  in >> substr;
  if (substr == "ODD")
  {
    out << std::count_if(shapes.begin(), shapes.end(), isOdd);
  }
  else if (substr == "EVEN")
  {
    out << std::count_if(shapes.begin(), shapes.end(), isEven);
  }
  else if(isDigit(substr))
  {
    size_t s = std::stoul(substr);
    if (s < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    auto compare = std::bind(isNumOfVertex, std::placeholders::_1, s);
    out << std::count_if(shapes.begin(), shapes.end(), compare);
  }
  else
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
  if (substr == "ODD")
  {
    std::vector< Polygon > odd;
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(odd), isOdd);
    double res = 0.0;
    for (size_t i = 0; i < odd.size(); ++i)
    {
      res += areaOnePoly(odd[i]);
    }
    out << res;
  }
  else if (substr == "EVEN")
  {
    std::vector< Polygon > even;
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(even), isEven);
    double res = 0.0;
    for (size_t i = 0; i < even.size(); ++i)
    {
      res += areaOnePoly(even[i]);
    }
    out << res;
  }
  else if (substr == "MEAN")
  {
    if (shapes.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    double res = 0.0;
    for (size_t i = 0; i < shapes.size(); ++i)
    {
      res += areaOnePoly(shapes[i]);
    }
    out << res / shapes.size();
  }
  else if (isDigit(substr))
  {
    size_t s = std::stoul(substr);
    if (s < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::vector< Polygon > temp;
    auto compare = std::bind(isNumOfVertex, std::placeholders::_1, s);
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(temp), compare);
    double res = 0.0;
    for (size_t i = 0; i < temp.size(); ++i)
    {
      res += areaOnePoly(temp[i]);
    }
    out << res;
  }
  else
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
  if (substr == "AREA")
  {
    std::vector< double > areas;
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(areas), areaOnePoly);
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);
    out << *(std::max_element(areas.begin(), areas.end()));
  }
  else if (substr == "VERTEXES")
  {
    std::vector< size_t > countVertex;
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(countVertex), getCountVertex);
    out << *(std::max_element(countVertex.begin(), countVertex.end()));
  }
  else
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
  if (substr == "AREA")
  {
    std::vector< double > areas;
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(areas), areaOnePoly);
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);
    out << *(std::min_element(areas.begin(), areas.end()));
  }
  else if (substr == "VERTEXES")
  {
    std::vector< size_t > countVertex;
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(countVertex), getCountVertex);
    out << *(std::min_element(countVertex.begin(), countVertex.end()));
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void finaev::echo(std::istream& in, std::ostream& out, std::vector< Polygon >& shapes)
{
  Polygon poly;
  in >> poly;
  if (in.fail() || !in)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  if (in.peek() != '\n' && in.peek() != EOF)
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
  in >> poly;
  if (in.fail() || !in)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  if (in.peek() != '\n' && in.peek() != EOF)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  auto same = std::bind(isSame, poly, std::placeholders::_1);
  out << std::count_if(shapes.begin(), shapes.end(), same);
}
