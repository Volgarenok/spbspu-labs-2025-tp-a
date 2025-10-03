#include "commands.hpp"
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>
#include <limits>
#include <map>
#include <stdexcept>
#include <guard.hpp>

namespace
{
  double getDeterminant(const smirnov::Point & p1, const smirnov::Point & p2)
  {
    return static_cast< double >(p1.x) * p2.y - static_cast< double >(p2.x) * p1.y;
  }

  double getArea(const smirnov::Polygon & poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    std::vector< smirnov::Point > rotated = poly.points;
    std::rotate(rotated.begin(), rotated.begin() + 1, rotated.end());
    auto currentPoint = poly.points.begin();
    auto endPoint = poly.points.end();
    auto nextPoint = rotated.begin();
    double areaSum = std::inner_product(currentPoint, endPoint, nextPoint, 0.0, std::plus< double >(), getDeterminant);
    return std::abs(areaSum) / 2.0;
  }

  bool isEven(const smirnov::Polygon & poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isOdd(const smirnov::Polygon & poly)
  {
    return poly.points.size() % 2 == 1;
  }

  struct IsVertexCount
  {
    size_t count;
    IsVertexCount(size_t c):
      count(c)
    {}
    bool operator()(const smirnov::Polygon & poly) const
    {
      return poly.points.size() == count;
    }
  };

  struct InFrameCheck
  {
    int min_x, max_x, min_y, max_y;
    InFrameCheck(int minx, int maxx, int miny, int maxy):
      min_x(minx),
      max_x(maxx),
      min_y(miny),
      max_y(maxy)
    {}
    bool operator()(const smirnov::Point & pt) const
    {
      return pt.x >= min_x && pt.x <= max_x && pt.y >= min_y && pt.y <= max_y;
    }
  };

  bool areaLess(const smirnov::Polygon & a, const smirnov::Polygon & b)
  {
    return getArea(a) < getArea(b);
  }

  bool vertexLess(const smirnov::Polygon & a, const smirnov::Polygon & b)
  {
    return a.points.size() < b.points.size();
  }

  bool compareX(const smirnov::Point & a, const smirnov::Point & b)
  {
    return a.x < b.x;
  }

  bool compareY(const smirnov::Point & a, const smirnov::Point & b)
  {
    return a.y < b.y;
  }

  struct MinXInAll
  {
    int operator()(const smirnov::Polygon & a, const smirnov::Polygon & b) const
    {
      int minA = std::min_element(a.points.begin(), a.points.end(), compareX)->x;
      int minB = std::min_element(b.points.begin(), b.points.end(), compareX)->x;
      return minA < minB;
    }
  };

  struct MaxXInAll
  {
    int operator()(const smirnov::Polygon & a, const smirnov::Polygon & b) const
    {
      int maxA = std::max_element(a.points.begin(), a.points.end(), compareX)->x;
      int maxB = std::max_element(b.points.begin(), b.points.end(), compareX)->x;
      return maxA < maxB;
    }
  };

  struct MinYInAll
  {
    int operator()(const smirnov::Polygon & a, const smirnov::Polygon & b) const
    {
      int minA = std::min_element(a.points.begin(), a.points.end(), compareY)->y;
      int minB = std::min_element(b.points.begin(), b.points.end(), compareY)->y;
      return minA < minB;
    }
  };

  struct MaxYInAll
  {
    int operator()(const smirnov::Polygon & a, const smirnov::Polygon & b) const
    {
      int maxA = std::max_element(a.points.begin(), a.points.end(), compareY)->y;
      int maxB = std::max_element(b.points.begin(), b.points.end(), compareY)->y;
      return maxA < maxB;
    }
  };

  int getMinX(const std::vector< smirnov::Polygon > & polygons)
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), MinXInAll());
    return std::min_element(it->points.begin(), it->points.end(), compareX)->x;
  }

  int getMaxX(const std::vector< smirnov::Polygon > & polygons)
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), MaxXInAll());
    return std::max_element(it->points.begin(), it->points.end(), compareX)->x;
  }

  int getMinY(const std::vector< smirnov::Polygon > & polygons)
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), MinYInAll());
    return std::min_element(it->points.begin(), it->points.end(), compareY)->y;
  }

  int getMaxY(const std::vector< smirnov::Polygon > & polygons)
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), MaxYInAll());
    return std::max_element(it->points.begin(), it->points.end(), compareY)->y;
  }

  struct MaxSeq
  {
    smirnov::Polygon pattern;
    size_t cur_seq;
    size_t max_seq;
    MaxSeq(const smirnov::Polygon & p):
      pattern(p),
      cur_seq(0),
      max_seq(0)
    {}
    MaxSeq operator()(const MaxSeq & acc, const smirnov::Polygon & poly) const
    {
      MaxSeq res = acc;
      if (poly == pattern)
      {
        res.cur_seq = res.cur_seq + 1;
        if (res.cur_seq > res.max_seq)
        {
          res.max_seq = res.cur_seq;
        }
      }
      else
      {
        res.cur_seq = 0;
      }
      return res;
    }
  };

  void printAreaEven(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    std::vector< smirnov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isEven);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << sum << "\n";
  }

  void printAreaOdd(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    std::vector< smirnov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isOdd);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << sum << "\n";
  }

  void printAreaMean(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << (sum / polygons.size()) << "\n";
  }

  void printAreaByVertex(const std::vector< smirnov::Polygon > & polygons, std::ostream & out, size_t numVtx)
  {
    if (numVtx < 3)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    std::vector< smirnov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), IsVertexCount(numVtx));
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << sum << "\n";
  }

  void printMaxArea(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    const smirnov::Polygon & maxPoly = *std::max_element(polygons.begin(), polygons.end(), areaLess);
    out << getArea(maxPoly) << "\n";
  }

  void printMaxVertexes(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    const smirnov::Polygon & maxPoly = *std::max_element(polygons.begin(), polygons.end(), vertexLess);
    out << maxPoly.points.size() << "\n";
  }

  void printMinArea(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    const smirnov::Polygon & minPoly = *std::min_element(polygons.begin(), polygons.end(), areaLess);
    out << getArea(minPoly) << "\n";
  }

  void printMinVertexes(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    const smirnov::Polygon & minPoly = *std::min_element(polygons.begin(), polygons.end(), vertexLess);
    out << minPoly.points.size() << "\n";
  }

  void printCountEven(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    size_t cnt = std::count_if(polygons.begin(), polygons.end(), isEven);
    out << cnt << "\n";
  }

  void printCountOdd(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    size_t cnt = std::count_if(polygons.begin(), polygons.end(), isOdd);
    out << cnt << "\n";
  }

  void printCountByVertex(const std::vector< smirnov::Polygon > & polygons, std::ostream & out, size_t numVtx)
  {
    if (numVtx < 3)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    size_t cnt = std::count_if(polygons.begin(), polygons.end(), IsVertexCount(numVtx));
    out << cnt << "\n";
  }
}

void smirnov::printArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);
  std::string arg;
  in >> arg;
  using sub_map =  std::map< std::string, std::function< void() > >;
  sub_map sub_cmds;
  sub_cmds["EVEN"] = std::bind(printAreaEven, std::cref(polygons), std::ref(out));
  sub_cmds["ODD"] = std::bind(printAreaOdd, std::cref(polygons), std::ref(out));
  sub_cmds["MEAN"] = std::bind(printAreaMean, std::cref(polygons), std::ref(out));
  try
  {
    sub_cmds.at(arg)();
  }
  catch (...)
  {
    size_t numVtx = std::stoull(arg);
    if (numVtx < 3)
    {
      throw;
    }
    printAreaByVertex(polygons, out, numVtx);
  }
}

void smirnov::printMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);
  std::string arg;
  in >> arg;
  using sub_map = std::map< std::string, std::function< void() > >;
  sub_map sub_cmds;
  sub_cmds["AREA"] = std::bind(printMaxArea, std::cref(polygons), std::ref(out));
  sub_cmds["VERTEXES"] = std::bind(printMaxVertexes, std::cref(polygons), std::ref(out));
  sub_cmds.at(arg)();
}

void smirnov::printMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);
  std::string arg;
  in >> arg;
  using sub_map =  std::map< std::string, std::function< void() > >;
  sub_map sub_cmds;
  sub_cmds["AREA"] = std::bind(printMinArea, std::cref(polygons), std::ref(out));
  sub_cmds["VERTEXES"] = std::bind(printMinVertexes, std::cref(polygons), std::ref(out));
  sub_cmds.at(arg)();
}

void smirnov::printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::string arg;
  in >> arg;
  using sub_map = std::map< std::string, std::function< void() > >;
  sub_map sub_cmds;
  sub_cmds["EVEN"] = std::bind(printCountEven, std::cref(polygons), std::ref(out));
  sub_cmds["ODD"] = std::bind(printCountOdd, std::cref(polygons), std::ref(out));
  try
  {
    sub_cmds.at(arg)();
  }
  catch (...)
  {
    size_t numVtx = std::stoull(arg);
    if (numVtx < 3)
    {
      throw;
    }
    printCountByVertex(polygons, out, numVtx);
  }
}

void smirnov::printInFrame(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  Polygon poly;
  if (!(in >> poly) || poly.points.size() < 3 || polygons.empty() || in.peek() != '\n')
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  in >> std::ws;
  int min_x = getMinX(polygons);
  int max_x = getMaxX(polygons);
  int min_y = getMinY(polygons);
  int max_y = getMaxY(polygons);
  InFrameCheck checker(min_x, max_x, min_y, max_y);
  bool in_frame = std::all_of(poly.points.begin(), poly.points.end(), checker);
  out << (in_frame ? "<TRUE>" : "<FALSE>") << "\n";
}

void smirnov::printMaxSeq(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  smirnov::Polygon pattern;
  if (!(in >> pattern) || in.peek() != '\n')
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  MaxSeq start(pattern);
  MaxSeq result = std::accumulate(polygons.begin(), polygons.end(), start, MaxSeq(pattern));
  out << result.max_seq << "\n";
}
