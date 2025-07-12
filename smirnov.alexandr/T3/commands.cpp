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

  double sumEvenArea(double acc, const smirnov::Polygon & poly)
  {
    if (isEven(poly))
    {
      return acc + getArea(poly);
    }
    else
    {
      return acc + 0.0;
    }
  }

  double sumOddArea(double acc, const smirnov::Polygon & poly)
  {
    if (isOdd(poly))
    {
      return acc + getArea(poly);
    }
    else
    {
      return acc + 0.0;
    }
  }

  double sumAllArea(double acc, const smirnov::Polygon & poly)
  {
    return acc + getArea(poly);
  }

  struct SumAreaByVertex
  {
    size_t count;
    SumAreaByVertex(size_t c):
      count(c)
    {}
    double operator()(double acc, const smirnov::Polygon & poly) const
    {
      if (poly.points.size() == count)
      {
        return acc + getArea(poly);
      }
      else
      {
        return acc + 0.0;
      }
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

  struct CompareX
  {
    bool operator()(const smirnov::Point & a, const smirnov::Point & b) const
    {
      return a.x < b.x;
    }
  };

  struct CompareY
  {
    bool operator()(const smirnov::Point & a, const smirnov::Point & b) const
    {
      return a.y < b.y;
    }
  };

  struct MinXInPolygon
  {
    int operator()(int acc, const smirnov::Polygon & poly) const
    {
      int local_min = std::min_element(poly.points.begin(), poly.points.end(), CompareX())->x;
      if (local_min < acc)
      {
        return local_min;
      }
      else
      {
        return acc;
      }
    }
  };

  struct MaxXInPolygon
  {
    int operator()(int acc, const smirnov::Polygon & poly) const
    {
      int local_max = std::max_element(poly.points.begin(), poly.points.end(), CompareX())->x;
      if (local_max > acc)
      {
        return local_max;
      }
      else
      {
        return acc;
      }
    }
  };

  struct MinYInPolygon
  {
    int operator()(int acc, const smirnov::Polygon & poly) const
    {
      int local_min = std::min_element(poly.points.begin(), poly.points.end(), CompareY())->y;
      if (local_min < acc)
      {
        return local_min;
      }
      else
      {
        return acc;
      }
    }
  };

  struct MaxYInPolygon
  {
    int operator()(int acc, const smirnov::Polygon & poly) const
    {
      int local_max = std::max_element(poly.points.begin(), poly.points.end(), CompareY())->y;
      if (local_max > acc)
      {
        return local_max;
      }
      else
      {
        return acc;
      }
    }
  };

  struct MaxSeq
  {
    const smirnov::Polygon & pattern;
    size_t cur_seq;
    size_t max_seq;
    MaxSeq(const smirnov::Polygon & p):
      pattern(p),
      cur_seq(0),
      max_seq(0)
    {}
    void operator()(const smirnov::Polygon & poly)
    {
      if (poly == pattern)
      {
        ++cur_seq;
        if (cur_seq > max_seq)
        {
          max_seq = cur_seq;
        }
      }
      else
      {
        cur_seq = 0;
      }
    }
  };

  void printAreaEven(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, sumEvenArea);
    out << sum << "\n";
  }

  void printAreaOdd(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, sumOddArea);
    out << sum << "\n";
  }

  void printAreaMean(const std::vector< smirnov::Polygon > & polygons, std::ostream & out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, sumAllArea);
    out << (sum / polygons.size()) << "\n";
  }

  void printAreaByVertex(const std::vector< smirnov::Polygon > & polygons, std::ostream & out, size_t numVtx)
  {
    if (numVtx < 3)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, SumAreaByVertex(numVtx));
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
  typedef std::map< std::string, std::function< void() > > sub_map;
  sub_map sub_cmds;
  sub_cmds["EVEN"] = std::bind(printAreaEven, std::cref(polygons), std::ref(out));
  sub_cmds["ODD"] = std::bind(printAreaOdd, std::cref(polygons), std::ref(out));
  sub_cmds["MEAN"] = std::bind(printAreaMean, std::cref(polygons), std::ref(out));
  try
  {
    if (sub_cmds.count(arg))
    {
      sub_cmds.at(arg)();
      return;
    }
    size_t numVtx = std::stoul(arg);
    printAreaByVertex(polygons, out, numVtx);
  }
  catch (...)
  {
    throw std::logic_error("<INVALID COMMAND>");
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
  typedef std::map< std::string, std::function< void() > > sub_map;
  sub_map sub_cmds;
  sub_cmds["AREA"] = std::bind(printMaxArea, std::cref(polygons), std::ref(out));
  sub_cmds["VERTEXES"] = std::bind(printMaxVertexes, std::cref(polygons), std::ref(out));
  try
  {
    sub_cmds.at(arg)();
  }
  catch (...)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
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
  typedef std::map< std::string, std::function< void() > > sub_map;
  sub_map sub_cmds;
  sub_cmds["AREA"] = std::bind(printMinArea, std::cref(polygons), std::ref(out));
  sub_cmds["VERTEXES"] = std::bind(printMinVertexes, std::cref(polygons), std::ref(out));
  try
  {
    sub_cmds.at(arg)();
  }
  catch (...)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void smirnov::printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::string arg;
  in >> arg;
  typedef std::map< std::string, std::function< void() > > sub_map;
  sub_map sub_cmds;
  sub_cmds["EVEN"] = std::bind(printCountEven, std::cref(polygons), std::ref(out));
  sub_cmds["ODD"] = std::bind(printCountOdd, std::cref(polygons), std::ref(out));
  try
  {
    if (sub_cmds.count(arg))
    {
      sub_cmds.at(arg)();
      return;
    }
    size_t numVtx = std::stoul(arg);
    printCountByVertex(polygons, out, numVtx);
  }
  catch (...)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void smirnov::printInFrame(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  Polygon poly;
  if (!(in >> poly) || poly.points.size() < 3 || in.peek() != '\n')
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  int min_x = std::accumulate(polygons.begin(), polygons.end(), std::numeric_limits< int >::max(), MinXInPolygon());
  int max_x = std::accumulate(polygons.begin(), polygons.end(), std::numeric_limits< int >::min(), MaxXInPolygon());
  int min_y = std::accumulate(polygons.begin(), polygons.end(), std::numeric_limits< int >::max(), MinYInPolygon());
  int max_y = std::accumulate(polygons.begin(), polygons.end(), std::numeric_limits< int >::min(), MaxYInPolygon());
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
  MaxSeq finder(pattern);
  std::for_each(polygons.begin(), polygons.end(), std::ref(finder));
  out << finder.max_seq << "\n";
}
