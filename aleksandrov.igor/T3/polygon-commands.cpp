#include "polygon-commands.hpp"
#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <limits>
#include <cmath>
#include <map>
#include <stream-guard.hpp>

namespace
{
  using namespace aleksandrov;

  double multPoints(const Point& a, const Point& b)
  {
    return a.x * b.y - a.y * b.x;
  }

  double calcArea(const Polygon& polygon)
  {
    auto begin = polygon.points.begin();
    auto end = polygon.points.end();
    auto plus = std::plus< double >{};
    double sum = std::inner_product(begin, end - 1, begin + 1, 0.0, plus, multPoints);
    sum += multPoints(polygon.points.back(), polygon.points.front());
    return std::abs(sum) / 2.0;
  }

  size_t calcVertices(const Polygon& polygon)
  {
    return polygon.points.size();
  }

  int getMinX(int curr, const Point& point)
  {
    return std::min(curr, point.x);
  }

  int getMinY(int curr, const Point& point)
  {
    return std::min(curr, point.y);
  }

  int getMaxX(int curr, const Point& point)
  {
    return std::max(curr, point.x);
  }

  int getMaxY(int curr, const Point& point)
  {
    return std::max(curr, point.y);
  }

  struct EvenVerticesChecker
  {
    bool operator()(const Polygon& polygon) const
    {
      return polygon.points.size() % 2 == 0;
    }
  };

  struct OddVerticesChecker
  {
    bool operator()(const Polygon& polygon) const
    {
      return polygon.points.size() % 2 != 0;
    }
  };

  struct NVerticesChecker
  {
    bool operator()(const Polygon& polygon) const
    {
      return polygon.points.size() == numOfVertices;
    }
    size_t numOfVertices;
  };

  template< class C >
  struct CondAreaAccumulator
  {
    double operator()(double sum, const Polygon& polygon) const
    {
      if (cond(polygon))
      {
        return sum + calcArea(polygon);
      }
      return sum;
    }
    C cond;
  };

  struct AreaAccumulator
  {
    double operator()(const Polygon& polygon) const
    {
      return calcArea(polygon);
    }
    double operator()(double sum, const Polygon& polygon) const
    {
      return sum + calcArea(polygon);
    }
  };

  struct VerticesAccumulator
  {
    size_t operator()(const Polygon& polygon) const
    {
      return calcVertices(polygon);
    }
  };

  struct AreaComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return calcArea(a) < calcArea(b);
    }
  };

  struct VerticesComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return a.points.size() < b.points.size();
    }
  };

  FrameRect getFrameRect(const Polygon& polygon)
  {
    const auto& pts = polygon.points;
    if (pts.empty())
    {
      return {};
    }
    int minX = std::accumulate(pts.begin() + 1, pts.end(), pts[0].x, getMinX);
    int minY = std::accumulate(pts.begin() + 1, pts.end(), pts[0].y, getMinY);
    int maxX = std::accumulate(pts.begin() + 1, pts.end(), pts[0].x, getMaxX);
    int maxY = std::accumulate(pts.begin() + 1, pts.end(), pts[0].y, getMaxY);
    return { { minX, minY }, { maxX, maxY } };
  }

  void getFrameRects(std::vector< FrameRect >& dest, const Polygons& src)
  {
    auto inserter = std::back_inserter(dest);
    std::transform(src.begin(), src.end(), inserter, getFrameRect);
  }

  struct FrameRectsMerger
  {
    FrameRect operator()(const FrameRect& a, const FrameRect& b) const
    {
      int minX = getMinX(a.first.x, b.first);
      int minY = getMinY(a.first.y, b.first);
      int maxX = getMaxX(a.second.x, b.second);
      int maxY = getMaxY(a.second.y, b.second);
      return { { minX, minY }, { maxX, maxY } };
    }
  };

  FrameRect mergeFrameRects(std::vector< FrameRect >& rects)
  {
    if (rects.empty())
    {
      return {};
    }
    return std::accumulate(rects.begin(), rects.end(), rects[0], FrameRectsMerger{});
  }

  Vector calcVectorCoords(const Point& a, const Point& b)
  {
    return { b.x - a.x, b.y - a.y };
  }

  std::vector< Vector > getEdgeVectors(const Polygon& polygons)
  {
    std::vector< Vector > vectors;
    auto begin = polygons.points.begin();
    auto end = polygons.points.end();
    auto inserter = std::back_inserter(vectors);
    std::transform(begin, end - 1, begin + 1, inserter, calcVectorCoords);
    vectors.emplace_back(calcVectorCoords(*(end - 1), *begin));
    return vectors;
  }

  void getEdges(std::vector< std::vector< Vector > >& dest, const Polygons& src)
  {
    auto begin = src.begin();
    auto end = src.end();
    auto inserter = std::back_inserter(dest);
    std::transform(begin, end, inserter, getEdgeVectors);
  }

  struct RightAngleChecker
  {
    bool operator()(const std::vector< Vector >& vectors) const
    {
      struct PerpendicularChecker
      {
        bool operator()(const Vector& a, const Vector& b) const
        {
          return (a.first * b.first + a.second * b.second) == 0;
        }
      };
      PerpendicularChecker checker{};
      auto begin = vectors.begin();
      auto end = vectors.end();
      bool hasRightAngle = std::adjacent_find(begin, end, checker) != end;

      if (!hasRightAngle && vectors.size() >= 2)
      {
        return checker(vectors.back(), vectors.front());
      }
      return hasRightAngle;
    }
  };
}

void aleksandrov::getPolygons(std::istream& in, Polygons& polygons)
{
  while (!in.eof())
  {
    auto begin = std::istream_iterator< Polygon >(in);
    auto end = std::istream_iterator< Polygon >();
    std::copy(begin, end, std::back_inserter(polygons));
    if (!in)
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

template< class Pred >
void aleksandrov::printAreaSumIf(const Polygons& polygons, Pred p, std::ostream& out)
{
  CondAreaAccumulator< Pred > accumulator{ p };
  out << std::accumulate(polygons.begin(), polygons.end(), 0.0, accumulator);
}

void aleksandrov::printMeanArea(const Polygons& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("There are no polygons!");
  }
  out << std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaAccumulator{}) / polygons.size();
}

void aleksandrov::area(const Polygons& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;

  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void() > > subcommands;

  using evenPred = EvenVerticesChecker;
  subcommands["EVEN"] = std::bind(printAreaSumIf< evenPred >, std::cref(polygons), evenPred{}, std::ref(out));

  using oddPred = OddVerticesChecker;
  subcommands["ODD"] = std::bind(printAreaSumIf< oddPred >, std::cref(polygons), oddPred{}, std::ref(out));

  subcommands["MEAN"] = std::bind(printMeanArea, std::cref(polygons), std::ref(out));

  auto it = subcommands.find(subcommand);
  if (it != subcommands.end())
  {
    it->second();
    return;
  }

  size_t numOfVertices = std::stoull(subcommand);
  if (numOfVertices < 3)
  {
    throw std::logic_error("Incorrect number of vertices!");
  }
  NVerticesChecker checker{ numOfVertices };
  printAreaSumIf(polygons, checker, out);
}

template< class F >
void aleksandrov::printIthSmallest(const Polygons& polygons, size_t i, F f, std::ostream& out)
{
  using ReturnType = decltype(f(polygons.front()));
  std::vector< ReturnType > data;
  std::transform(polygons.begin(), polygons.end(), std::back_inserter(data), f);

  if (i < polygons.size())
  {
    std::nth_element(data.begin(), data.begin() + i, data.end());
    out << data[i];
  }
  else
  {
    out << *std::max_element(data.begin(), data.end());
  }
}

void aleksandrov::ithSmallest(const Polygons& polygons, size_t i, std::istream& in, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("There are no polygons!");
  }
  std::string subcommand;
  in >> subcommand;

  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void() > > subcommands;

  using areaAcc = AreaAccumulator;
  subcommands["AREA"] = std::bind(printIthSmallest< areaAcc >, std::cref(polygons), i, areaAcc{}, std::ref(out));

  using vertsAcc = VerticesAccumulator;
  subcommands["VERTEXES"] = std::bind(printIthSmallest< vertsAcc >, std::cref(polygons), i, vertsAcc{}, std::ref(out));

  subcommands.at(subcommand)();
}

template< class Pred >
void aleksandrov::printCountIf(const Polygons& polygons, Pred p, std::ostream& out)
{
  out << std::count_if(polygons.begin(), polygons.end(), p);
}

void aleksandrov::count(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;

  std::map< std::string, std::function< void() > > subcommands;

  using evenPred = EvenVerticesChecker;
  subcommands["EVEN"] = std::bind(printCountIf< evenPred >, std::cref(polygons), evenPred{}, std::ref(out));

  using oddPred = OddVerticesChecker;
  subcommands["ODD"] = std::bind(printCountIf< oddPred >, std::cref(polygons), oddPred{}, std::ref(out));

  auto it = subcommands.find(subcommand);
  if (it != subcommands.end())
  {
    it->second();
    return;
  }

  size_t numOfVertices = std::stoull(subcommand);
  if (numOfVertices < 3)
  {
    throw std::logic_error("Incorrect number of vertexes!");
  }
  printCountIf(polygons, NVerticesChecker{ numOfVertices }, out);
}

void aleksandrov::inFrame(const Polygons& polygons, std::istream& in, std::ostream& out)
{
  Polygon polygon;
  if (!(in >> polygon) || polygons.empty())
  {
    throw std::logic_error("Incorrect polygon!");
  }

  char next = '0';
  in >> std::noskipws >> next >> std::skipws;
  if (next != '\n' && next != EOF)
  {
    throw std::logic_error("Incorrect input!");
  }

  std::vector< FrameRect > frameRects;
  getFrameRects(frameRects, polygons);

  FrameRect merged = mergeFrameRects(frameRects);
  FrameRect input = getFrameRect(polygon);

  bool isMinInside = merged.first.x <= input.first.x && merged.first.y <= input.first.y;
  bool isMaxInside = merged.second.x >= input.second.x && merged.second.y >= input.second.y;

  out << ((isMinInside && isMaxInside) ? "<TRUE>" : "<FALSE>");
}

void aleksandrov::rightShapes(const Polygons& polygons, std::ostream& out)
{
  std::vector< std::vector< Vector > > allEdges;
  getEdges(allEdges, polygons);

  RightAngleChecker checker{};
  out << std::count_if(allEdges.begin(), allEdges.end(), checker);
}

