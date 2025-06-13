#include "commands.hpp"
#include <map>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <cmath>
#include <iomanip>
#include <streamguard.hpp>

namespace
{
  struct PolyAreaAccumulator
  {
    double operator()(const tkach::Point& p1, const tkach::Point& p2) const
    {
      return static_cast< double >(p1.x) * p2.y - static_cast< double >(p1.y) * p2.x;
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

  bool pointEqual(const tkach::Point& lhs, const tkach::Point& rhs)
  {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
  }

  tkach::Point findMinXYPoint(const tkach::Polygon& polygon)
  {
    return *std::min_element(polygon.points.cbegin(), polygon.points.cend(), pointCmp);
  }

  bool hasPoint(const tkach::Point& delta, const tkach::Point& point, const tkach::Polygon& polygon)
  {
    tkach::Point res = {point.x + delta.x, point.y + delta.y};
    using namespace std::placeholders;
    auto accum_same = std::bind(pointEqual, res, _1);
    return std::find_if(polygon.points.cbegin(), polygon.points.cend(), accum_same) != polygon.points.cend();
  }

  struct PolySame
  {
    const tkach::Polygon& poly;
    bool operator()(const tkach::Polygon& polygon)
    {
      if (poly.points.size() != polygon.points.size())
      {
        return false;
      }
      if (polygon.points.empty())
      {
        return true;
      }
      tkach::Point target_min = findMinXYPoint(poly);
      tkach::Point poly_min = findMinXYPoint(polygon);
      tkach::Point delta = {poly_min.x - target_min.x, poly_min.y - target_min.y};
      auto predicate = std::bind(hasPoint, delta, std::placeholders::_1, std::cref(polygon));
      size_t count_matched_points = std::count_if(poly.points.cbegin(), poly.points.cend(), predicate);
      return count_matched_points == poly.points.size();
    }
  };

  double calculatePolygonArea(const tkach::Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      return 0.0;
    }
    PolyAreaAccumulator accum{};
    auto begin_it = polygon.points.begin();
    auto end_it = polygon.points.end();
    double sum = std::inner_product(begin_it, end_it - 1, begin_it + 1, 0.0, std::plus< double >(), accum);
    sum += accum(polygon.points[polygon.points.size() - 1], polygon.points[0]);
    return std::fabs(sum) / 2.0;
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

  double sumPolyVector(const std::vector< tkach::Polygon >& poly)
  {
    std::vector< double > areas;
    std::transform(poly.begin(), poly.end(), std::back_inserter(areas), calculatePolygonArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double sumPolyVectorEven(const std::vector< tkach::Polygon >& poly)
  {
    return sumPolyVector(poly);
  }

  double sumPolyVectorOdd(const std::vector< tkach::Polygon >& poly)
  {
    return sumPolyVector(poly);
  }

  double sumPolyVectorMean(const std::vector< tkach::Polygon >& poly)
  {
    if (poly.empty())
    {
      throw std::logic_error("Error: zero polygons");
    }
    return sumPolyVector(poly) / poly.size();
  }

  double sumPolyVectorCount(const std::vector< tkach::Polygon >& poly)
  {
    return sumPolyVector(poly);
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
  PolySame cmp{target};
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
  using sub_commands_map = std::map< std::string, std::function< double() > >;
  StreamGuard guard(out);
  sub_commands_map sub_cmds;
  sub_cmds["EVEN"] = std::bind(sumPolyVectorEven, std::bind(clearedEven, std::cref(data)));
  sub_cmds["ODD"] = std::bind(sumPolyVectorOdd, std::bind(clearedOdd, std::cref(data)));
  sub_cmds["MEAN"] = std::bind(sumPolyVectorMean, std::bind(clearedMean, std::cref(data)));
  std::string sub_cmd;
  in >> sub_cmd;
  double res = 0.0;
  try
  {
    res = sub_cmds.at(sub_cmd)();
  }
  catch (...)
  {
    size_t count = std::stoull(sub_cmd);
    if (count < 3)
    {
      throw std::logic_error("Error: not polygon");
    }
    res = sumPolyVectorCount(clearedCount(data, count));
  }
  out << std::fixed << std::setprecision(1);
  out << res << "\n";
}
