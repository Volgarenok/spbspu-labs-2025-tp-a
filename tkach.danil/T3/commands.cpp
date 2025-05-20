#include "commands.hpp"
#include <map>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <cmath>
#include <streamguard.hpp>
#include <iomanip>

namespace
{
  struct PolyAreaAccumulator
  {
    const tkach::Polygon& poly;
    size_t index;
    double operator()(double area, const tkach::Point& polygon)
    {
      size_t next = (index + 1) % poly.points.size();
      area += (polygon.x * poly.points[next].y) - (polygon.y * poly.points[next].x);
      index++;
      return area;
    }
  };
  bool pointCmp(const tkach::Point& lhs, const tkach::Point& rhs)
  {
    if (lhs.x != rhs.x)
    {
      return lhs.x < rhs.x;
    }
    else
    {
      return lhs.y < rhs.y;
    }
  }
  bool deltaCmp(const tkach::Point& lhs, const tkach::Point& rhs, double deltax, double deltay)
  {
    return std::fabs(lhs.x - rhs.x - deltax) < 1e-9 && std::fabs(lhs.y - rhs.y - deltay) < 1e-9;
  }

  struct PolySame
  {
    const tkach::Polygon& poly;
    double deltax;
    double deltay;
    bool operator()(const tkach::Polygon& polygon)
    {
      if (poly.points.size() != polygon.points.size())
      {
        return false;
      }
      std::vector< tkach::Point > temp = polygon.points;
      std::sort(temp.begin(), temp.end(), pointCmp);
      deltax = temp[0].x - poly.points[0].x;
      deltay = temp[0].y - poly.points[0].y;
      auto cmp = std::bind(deltaCmp, std::placeholders::_1, std::placeholders::_2, deltax, deltay);
      return std::equal(temp.begin(), temp.end(), poly.points.begin(), cmp);
    }
  };

  double calculatePolygonArea(const tkach::Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      return 0.0;
    }
    PolyAreaAccumulator accum{polygon, 0};
    double sum = std::fabs(std::accumulate(polygon.points.begin(), polygon.points.end(), 0.0, accum));
    return sum / 2.0;
  }

  bool isEven(const tkach::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const tkach::Polygon& polygon)
  {
    return polygon.points.size() % 2 != 0;
  }

  bool checkVertexCount(const tkach::Polygon& polygon, size_t required_count)
  {
    return polygon.points.size() == required_count;
  }

  std::vector< tkach::Polygon > clearedEven(const std::vector< tkach::Polygon >& data)
  {
    std::vector< tkach::Polygon > result;
    std::copy_if(data.begin(), data.end(), std::back_inserter(result), isEven);
    return result;
  }

  std::vector< tkach::Polygon > clearedOdd(const std::vector< tkach::Polygon >& data)
  {
    std::vector< tkach::Polygon > result;
    std::copy_if(data.begin(), data.end(), std::back_inserter(result), isOdd);
    return result;
  }

  std::vector< tkach::Polygon > clearedMean(const std::vector< tkach::Polygon >& data)
  {
    return data;
  }

  std::vector< tkach::Polygon > clearedCount(const std::vector< tkach::Polygon >& data, size_t required_count)
  {
    std::vector< tkach::Polygon > result;
    auto predicate = std::bind(checkVertexCount, std::placeholders::_1, required_count);
    std::copy_if(data.begin(), data.end(), std::back_inserter(result), predicate);
    return result;
  }

  bool compareArea(const tkach::Polygon& lhs, const tkach::Polygon& rhs)
  {
    return calculatePolygonArea(lhs) < calculatePolygonArea(rhs);
  }

  bool compareVertex(const tkach::Polygon& lhs, const tkach::Polygon& rhs)
  {
    return lhs.points.size() < rhs.points.size();
  }

  std::ostream& printMaxArea(std::ostream& out, const std::vector< tkach::Polygon >& data)
  {
    double result = calculatePolygonArea(*(std::max_element(data.cbegin(), data.cend(), compareArea)));
    tkach::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << result << "\n";
    return out;
  }

  std::ostream& printMaxVert(std::ostream& out, const std::vector< tkach::Polygon >& data)
  {
    int result = (*(std::max_element(data.cbegin(), data.cend(), compareVertex))).points.size();
    tkach::StreamGuard guard(out);
    out << result << "\n";
    return out;
  }

  std::ostream& printMinArea(std::ostream& out, const std::vector< tkach::Polygon >& data)
  {
    double result = calculatePolygonArea(*(std::min_element(data.cbegin(), data.cend(), compareArea)));
    tkach::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << result << "\n";
    return out;
  }

  std::ostream& printMinVert(std::ostream& out, const std::vector< tkach::Polygon >& data)
  {
    int result = (*(std::min_element(data.cbegin(), data.cend(), compareVertex))).points.size();
    tkach::StreamGuard guard(out);
    out << result << "\n";
    return out;
  }

  double getDistance(const tkach::Point& lhs, const tkach::Point& rhs)
  {
    return std::sqrt(std::pow((lhs.x - rhs.x), 2) + std::pow((lhs.y - rhs.y), 2));
  }

  bool isRect(const tkach::Polygon& polygon)
  {
    if (polygon.points.size() != 4)
    {
      return false;
    }
    return (getDistance(polygon.points[0], polygon.points[2]) - getDistance(polygon.points[1], polygon.points[3])) < 1e-9;
  }
}

void tkach::printMax(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  if (data.empty())
  {
    throw std::logic_error("Error: zero polygons");
  }
  using sub_commands_map = std::map< std::string, std::function< std::ostream&() > >;
  StreamGuard guard(out);
  sub_commands_map sub_cmds;
  sub_cmds["AREA"] = std::bind(printMaxArea, std::ref(out), std::cref(data));
  sub_cmds["VERTEXES"] = std::bind(printMaxVert, std::ref(out), std::cref(data));
  std::string sub_cmd;
  in >> sub_cmd;
  try
  {
    sub_cmds.at(sub_cmd)();
  }
  catch (...)
  {
    throw std::logic_error("Error: not that command");
  }
}

void tkach::printMin(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  if (data.empty())
  {
    throw std::logic_error("Error: zero polygons");
  }
  using sub_commands_map = std::map< std::string, std::function< std::ostream&() > >;
  StreamGuard guard(out);
  sub_commands_map sub_cmds;
  sub_cmds["AREA"] = std::bind(printMinArea, std::ref(out), std::cref(data));
  sub_cmds["VERTEXES"] = std::bind(printMinVert, std::ref(out), std::cref(data));
  std::string sub_cmd;
  in >> sub_cmd;
  try
  {
    sub_cmds.at(sub_cmd)();
  }
  catch (...)
  {
    throw std::logic_error("Error: not that command");
  }
}

void tkach::printSame(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  Polygon target;
  if (!(in >> target) || target.points.size() < 3 || in.peek() != '\n')
  {
    throw std::logic_error("Error: not polygon");
  }
  std::sort(target.points.begin(), target.points.end(), pointCmp);
  PolySame cmp{target, 0, 0};
  out << std::count_if(data.begin(), data.end(), cmp) << '\n';
}

void tkach::printRects(std::ostream& out, const std::vector< Polygon >& data)
{
  out << std::count_if(data.begin(), data.end(), isRect) << "\n";
}

void tkach::printCount(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  using sub_commands_map = std::map< std::string, std::function< std::vector< Polygon >() > >;
  StreamGuard guard(out);
  sub_commands_map sub_cmds;
  sub_cmds["EVEN"] = std::bind(clearedEven, std::cref(data));
  sub_cmds["ODD"] = std::bind(clearedOdd, std::cref(data));
  std::string sub_cmd;
  in >> sub_cmd;
  std::vector< tkach::Polygon > cleared;
  try
  {
    cleared = sub_cmds.at(sub_cmd)();
  }
  catch (...)
  {
    size_t count = std::stoull(sub_cmd);
    if (count < 3)
    {
      throw std::logic_error("Error: not polygon");
    }
    cleared = clearedCount(data, count);
  }
  out << cleared.size() << "\n";
}

void tkach::printArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  using sub_commands_map = std::map< std::string, std::function< std::vector< Polygon >() > >;
  StreamGuard guard(out);
  sub_commands_map sub_cmds;
  sub_cmds["EVEN"] = std::bind(clearedEven, std::cref(data));
  sub_cmds["ODD"] = std::bind(clearedOdd, std::cref(data));
  sub_cmds["MEAN"] = std::bind(clearedMean, std::cref(data));
  std::string sub_cmd;
  in >> sub_cmd;
  std::vector< tkach::Polygon > cleared;
  try
  {
    cleared = sub_cmds.at(sub_cmd)();
  }
  catch (...)
  {
    size_t count = std::stoull(sub_cmd);
    if (count < 3)
    {
      throw std::logic_error("Error: not polygon");
    }
    cleared = clearedCount(data, count);
  }
  std::vector< double > areas;
  std::transform(cleared.begin(), cleared.end(), std::back_inserter(areas), calculatePolygonArea);
  out << std::fixed << std::setprecision(1);
  if (sub_cmd == "MEAN")
  {
    if (!areas.empty())
    {
      out << std::accumulate(areas.begin(), areas.end(), 0.0) / areas.size() << "\n";
    }
    else
    {
      throw std::logic_error("Error: zero polygons");
    }
  }
  else
  {
    out << std::accumulate(areas.begin(), areas.end(), 0.0) << "\n";
  }
}
