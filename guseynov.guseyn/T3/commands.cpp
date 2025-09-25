#include "commands.hpp"
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <cmath>
#include "guard.hpp"

namespace
{
  double getDeterminant(const guseynov::Point & p1, const guseynov::Point & p2)
  {
    return static_cast< double >(p1.x) * p2.y - static_cast< double >(p2.x) * p1.y;
  }

  double getArea(const guseynov::Polygon & poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    std::vector< guseynov::Point > rotated = poly.points;
    std::rotate(rotated.begin(), rotated.begin() + 1, rotated.end());
    auto currentPoint = poly.points.begin();
    auto endPoint = poly.points.end();
    auto nextPoint = rotated.begin();
    double areaSum = std::inner_product(currentPoint, endPoint, nextPoint, 0.0, std::plus< double >(), getDeterminant);
    return std::abs(areaSum) / 2.0;
  }

  bool isEven(const guseynov::Polygon & poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isOdd(const guseynov::Polygon & poly)
  {
    return poly.points.size() % 2 == 1;
  }

  struct IsVertexCount
  {
    size_t count;
    IsVertexCount(size_t c):
      count(c)
    {}
    bool operator()(const guseynov::Polygon & poly) const
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
    bool operator()(const guseynov::Point & pt) const
    {
      return pt.x >= min_x && pt.x <= max_x && pt.y >= min_y && pt.y <= max_y;
    }
  };

  bool areaLess(const guseynov::Polygon & a, const guseynov::Polygon & b)
  {
    return getArea(a) < getArea(b);
  }

  bool vertexLess(const guseynov::Polygon & a, const guseynov::Polygon & b)
  {
    return a.points.size() < b.points.size();
  }

  bool compareX(const guseynov::Point & a, const guseynov::Point & b)
  {
    return a.x < b.x;
  }

  bool compareY(const guseynov::Point & a, const guseynov::Point & b)
  {
    return a.y < b.y;
  }

  struct MinXInAll
  {
    bool operator()(const guseynov::Polygon & a, const guseynov::Polygon & b) const
    {
      int minA = std::min_element(a.points.begin(), a.points.end(), compareX)->x;
      int minB = std::min_element(b.points.begin(), b.points.end(), compareX)->x;
      return minA < minB;
    }
  };

  struct MaxXInAll
  {
    bool operator()(const guseynov::Polygon & a, const guseynov::Polygon & b) const
    {
      int maxA = std::max_element(a.points.begin(), a.points.end(), compareX)->x;
      int maxB = std::max_element(b.points.begin(), b.points.end(), compareX)->x;
      return maxA < maxB;
    }
  };

  struct MinYInAll
  {
    bool operator()(const guseynov::Polygon & a, const guseynov::Polygon & b) const
    {
      int minA = std::min_element(a.points.begin(), a.points.end(), compareY)->y;
      int minB = std::min_element(b.points.begin(), b.points.end(), compareY)->y;
      return minA < minB;
    }
  };

  struct MaxYInAll
  {
    bool operator()(const guseynov::Polygon & a, const guseynov::Polygon & b) const
    {
      int maxA = std::max_element(a.points.begin(), a.points.end(), compareY)->y;
      int maxB = std::max_element(b.points.begin(), b.points.end(), compareY)->y;
      return maxA < maxB;
    }
  };

  int getMinX(const std::vector< guseynov::Polygon > & polygons)
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), MinXInAll());
    return std::min_element(it->points.begin(), it->points.end(), compareX)->x;
  }

  int getMaxX(const std::vector< guseynov::Polygon > & polygons)
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), MaxXInAll());
    return std::max_element(it->points.begin(), it->points.end(), compareX)->x;
  }

  int getMinY(const std::vector< guseynov::Polygon > & polygons)
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), MinYInAll());
    return std::min_element(it->points.begin(), it->points.end(), compareY)->y;
  }

  int getMaxY(const std::vector< guseynov::Polygon > & polygons)
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), MaxYInAll());
    return std::max_element(it->points.begin(), it->points.end(), compareY)->y;
  }

  void printAreaEven(const std::vector< guseynov::Polygon > & polygons, std::ostream & out)
  {
    std::vector< guseynov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isEven);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << sum << "\n";
  }

  void printAreaOdd(const std::vector< guseynov::Polygon > & polygons, std::ostream & out)
  {
    std::vector< guseynov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isOdd);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << sum << "\n";
  }

  void printAreaMean(const std::vector< guseynov::Polygon > & polygons, std::ostream & out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << (sum / polygons.size()) << "\n";
  }

  void printAreaByVertex(const std::vector< guseynov::Polygon > & polygons, std::ostream & out, size_t numVtx)
  {
    if (numVtx < 3)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    std::vector< guseynov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), IsVertexCount(numVtx));
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << sum << "\n";
  }

  void printMaxArea(const std::vector< guseynov::Polygon > & polygons, std::ostream & out)
  {
    const guseynov::Polygon & maxPoly = *std::max_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << getArea(maxPoly) << "\n";
  }

  void printMaxVertexes(const std::vector< guseynov::Polygon > & polygons, std::ostream & out)
  {
    const guseynov::Polygon & maxPoly = *std::max_element(polygons.begin(), polygons.end(), vertexLess);
    out << maxPoly.points.size() << "\n";
  }

  void printMinArea(const std::vector< guseynov::Polygon > & polygons, std::ostream & out)
  {
    const guseynov::Polygon & minPoly = *std::min_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << getArea(minPoly) << "\n";
  }

  void printMinVertexes(const std::vector< guseynov::Polygon > & polygons, std::ostream & out)
  {
    const guseynov::Polygon & minPoly = *std::min_element(polygons.begin(), polygons.end(), vertexLess);
    out << minPoly.points.size() << "\n";
  }

  void printCountEven(const std::vector< guseynov::Polygon > & polygons, std::ostream & out)
  {
    size_t cnt = std::count_if(polygons.begin(), polygons.end(), isEven);
    out << cnt << "\n";
  }

  void printCountOdd(const std::vector< guseynov::Polygon > & polygons, std::ostream & out)
  {
    size_t cnt = std::count_if(polygons.begin(), polygons.end(), isOdd);
    out << cnt << "\n";
  }

  void printCountByVertex(const std::vector< guseynov::Polygon > & polygons, std::ostream & out, size_t numVtx)
  {
    if (numVtx < 3)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    size_t cnt = std::count_if(polygons.begin(), polygons.end(), IsVertexCount(numVtx));
    out << cnt << "\n";
  }

  void printLessAreaHelper(const std::vector< guseynov::Polygon > & polygons, std::ostream & out, const guseynov::Polygon & compPoly)
  {
    double compArea = getArea(compPoly);
    struct AreaLessThan
    {
      double threshold;
      AreaLessThan(double thresh) : threshold(thresh) {}
      bool operator()(const guseynov::Polygon & poly) const
      {
        return getArea(poly) < threshold;
      }
    };
    size_t cnt = std::count_if(polygons.begin(), polygons.end(), AreaLessThan(compArea));
    out << cnt << "\n";
  }
}

void guseynov::printArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  StreamGuard guard(out);
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
    try
    {
      size_t numVtx = std::stoull(arg);
      if (numVtx < 3)
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
      printAreaByVertex(polygons, out, numVtx);
    }
    catch (const std::invalid_argument&)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
}

void guseynov::printMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  StreamGuard guard(out);
  std::string arg;
  in >> arg;
  using sub_map = std::map< std::string, std::function< void() > >;
  sub_map sub_cmds;
  sub_cmds["AREA"] = std::bind(printMaxArea, std::cref(polygons), std::ref(out));
  sub_cmds["VERTEXES"] = std::bind(printMaxVertexes, std::cref(polygons), std::ref(out));
  sub_cmds.at(arg)();
}

void guseynov::printMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  StreamGuard guard(out);
  std::string arg;
  in >> arg;
  using sub_map =  std::map< std::string, std::function< void() > >;
  sub_map sub_cmds;
  sub_cmds["AREA"] = std::bind(printMinArea, std::cref(polygons), std::ref(out));
  sub_cmds["VERTEXES"] = std::bind(printMinVertexes, std::cref(polygons), std::ref(out));
  sub_cmds.at(arg)();
}

void guseynov::printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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
    try
    {
      size_t numVtx = std::stoull(arg);
      if (numVtx < 3)
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
      printCountByVertex(polygons, out, numVtx);
    }
    catch (const std::invalid_argument&)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
}

void guseynov::printInFrame(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  Polygon poly;
  if (!(in >> poly) || poly.points.size() < 3 || polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  int min_x = getMinX(polygons);
  int max_x = getMaxX(polygons);
  int min_y = getMinY(polygons);
  int max_y = getMaxY(polygons);
  InFrameCheck checker(min_x, max_x, min_y, max_y);
  bool in_frame = std::all_of(poly.points.begin(), poly.points.end(), checker);
  out << (in_frame ? "<TRUE>" : "<FALSE>") << "\n";
}

void guseynov::printLessArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  Polygon compPoly;
  if (!(in >> compPoly) || compPoly.points.size() < 3)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  printLessAreaHelper(polygons, out, compPoly);
}
