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

namespace
{
  using namespace aleksandrov;

  double multPoints(const Point& a, const Point& b)
  {
    return a.x * b.y - a.y * b.x;
  }

  double calcArea(const Polygon& polygon)
  {
    const auto& p = polygon.points;
    auto plus = std::plus< double >{};
    double sum = std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, plus, multPoints);
    sum += multPoints(p.back(), p.front());
    return std::abs(sum) / 2.0;
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

  struct FrameRectFinder
  {
    FrameRect operator()(const FrameRect& rect, const Point& point) const
    {
      Point newMin{ std::min(rect.first.x, point.x), std::min(rect.first.y, point.y) };
      Point newMax{ std::max(rect.second.x, point.x), std::max(rect.second.y, point.y) };
      return { newMin, newMax };
    }
  };

  struct FrameRectsMerger
  {
    FrameRect operator()(const FrameRect& a, const FrameRect& b) const
    {
      Point newMin{ std::min(a.first.x, b.first.x), std::min(a.first.y, b.first.y) };
      Point newMax{ std::max(a.second.x, b.second.x), std::max(a.second.y, b.second.y) };
      return { newMin, newMax };
    }
  };

  struct VectorCoordsCalculator
  {
    Vector operator()(const Point& a, const Point& b) const
    {
      return Vector{ b.x - a.x, b.y - a.y };
    }
  };

  struct EdgeVectorsCalculator
  {
    std::vector< Vector > operator()(const Polygon& polygon) const
    {
      std::vector< Vector > vectors;
      const auto& pts = polygon.points;
      VectorCoordsCalculator calculator{};
      auto end = std::back_inserter(vectors);
      std::transform(pts.begin(), pts.end() - 1, pts.begin() + 1, end, calculator);
      vectors.emplace_back(calculator(pts.back(), pts.front()));
      return vectors;
    }
  };

  struct RightAngleChecker
  {
    bool operator()(const std::vector< Vector >& vectors) const
    {
      struct PerpendicularChecker
      {
        bool operator()(const Vector& a, const Vector& b) const
        {
          return !(a.first * b.first + a.second * b.second);
        }
      };

      std::vector< Vector > rotated(vectors);
      std::rotate(rotated.begin(), rotated.begin() + 1, rotated.end());

      auto begin = vectors.begin();
      auto end = vectors.end();
      auto logicalOr = std::logical_or< bool >{};
      return std::inner_product(begin, end, rotated.begin(), false, logicalOr, PerpendicularChecker{});
    }
  };

  struct DummyChecker
  {
    bool operator()(const Polygon&) const
    {
      return true;
    }
  };

  struct DummyOperator
  {
    Point operator()(const Point&, const Point& b) const
    {
      return b;
    }
    FrameRect operator()(const FrameRect&, const FrameRect& b) const
    {
      return b;
    }
  };
}


void aleksandrov::getPolygons(std::istream& in, std::vector< Polygon >& polygons)
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

double aleksandrov::areaEven(const std::vector< Polygon >& polygons)
{
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, CondAreaAccumulator< EvenVerticesChecker >{});
}

double aleksandrov::areaOdd(const std::vector< Polygon >& polygons)
{
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, CondAreaAccumulator< OddVerticesChecker >{});
}

double aleksandrov::areaMean(const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("There are no polygons!");
  }
  auto AreaAccumulator = CondAreaAccumulator< DummyChecker >{};
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaAccumulator) / polygons.size();
}

double aleksandrov::areaN(const std::vector< Polygon >& polygons, size_t numOfVertices)
{
  NVerticesChecker checker{ numOfVertices };
  CondAreaAccumulator< NVerticesChecker > accumulator{ checker };
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, accumulator);
}

void aleksandrov::area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;

  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< double() > > subcommands;
  subcommands["EVEN"] = std::bind(areaEven, std::cref(polygons));
  subcommands["ODD"] = std::bind(areaOdd, std::cref(polygons));
  subcommands["MEAN"] = std::bind(areaMean, std::cref(polygons));

  auto it = subcommands.find(subcommand);
  if (it != subcommands.end())
  {
    out << it->second();
    return;
  }

  size_t numOfVertices = std::stoull(subcommand);
  if (numOfVertices < 3)
  {
    throw std::logic_error("Incorrect number of vertices!");
  }
  out << areaN(polygons, numOfVertices);
}

void aleksandrov::maxArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  auto maxAreaPolygon = std::max_element(polygons.begin(), polygons.end(), AreaComparator{});
  out << calcArea(*maxAreaPolygon);
}

void aleksandrov::maxVertices(const std::vector< Polygon >& polygons, std::ostream& out)
{
  auto maxVertPolygon = std::max_element(polygons.begin(), polygons.end(), VerticesComparator{});
  out << (*maxVertPolygon).points.size();
}

void aleksandrov::max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("There are no polygons!");
  }
  std::string subcommand;
  in >> subcommand;

  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(maxArea, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(maxVertices, std::cref(polygons), std::ref(out));

  subcommands.at(subcommand)();
}

void aleksandrov::minArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  auto minAreaPolygon = std::min_element(polygons.begin(), polygons.end(), AreaComparator{});
  out << calcArea(*minAreaPolygon);
}

void aleksandrov::minVertices(const std::vector< Polygon >& polygons, std::ostream& out)
{
  auto minVertPolygon = std::min_element(polygons.begin(), polygons.end(), VerticesComparator{});
  out << (*minVertPolygon).points.size();
}

void aleksandrov::min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("There are no polygons!");
  }
  std::string subcommand;
  in >> subcommand;

  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(minArea, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(minVertices, std::cref(polygons), std::ref(out));

  subcommands.at(subcommand)();
}

size_t aleksandrov::countEven(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), EvenVerticesChecker{});
}

size_t aleksandrov::countOdd(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), OddVerticesChecker{});
}

size_t aleksandrov::countN(const std::vector< Polygon >& polygons, size_t numOfVertices)
{
  return std::count_if(polygons.begin(), polygons.end(), NVerticesChecker{ numOfVertices });
}

void aleksandrov::count(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;

  std::map< std::string, std::function< size_t() > > subcommands;
  subcommands["EVEN"] = std::bind(countEven, std::cref(polygons));
  subcommands["ODD"] = std::bind(countOdd, std::cref(polygons));

  auto it = subcommands.find(subcommand);
  if (it != subcommands.end())
  {
    out << it->second();
    return;
  }

  size_t numOfVertices = std::stoull(subcommand);
  if (numOfVertices < 3)
  {
    throw std::logic_error("Incorrect number of vertexes!");
  }
  out << countN(polygons, numOfVertices);
}

aleksandrov::FrameRect aleksandrov::findFrameRect(const Polygon& polygon)
{
  const auto& pts = polygon.points;
  if (pts.empty())
  {
    return {{0, 0}, {0, 0}};
  }
  FrameRect init{ pts.front(), pts.front() };
  return std::inner_product(pts.begin(), pts.end(), pts.begin(), init, FrameRectFinder{}, DummyOperator{});
}

aleksandrov::FrameRect aleksandrov::findGlobalFrameRect(const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    return {{0, 0}, {0, 0}};
  }
  std::vector< FrameRect > rects;
  std::transform(polygons.begin(), polygons.end(), std::back_inserter(rects), findFrameRect);

  auto begin = rects.begin();
  auto end = rects.end();
  return std::inner_product(begin, end, begin, rects.front(), FrameRectsMerger{}, DummyOperator{});
}

void aleksandrov::inFrame(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon polygon;
  if (!(in >> polygon) || polygons.empty())
  {
    throw std::logic_error("Incorrect polygon!");
  }
  while (in.peek() != '\n' && std::isspace(in.peek()))
  {
    in.ignore(1);
  }
  if (in.peek() != '\n' && in.peek() != EOF)
  {
    throw std::logic_error("Incorrect polygon!");
  }

  FrameRect globalRect = findGlobalFrameRect(polygons);
  FrameRect inputRect = findFrameRect(polygon);

  const bool isMinInside = globalRect.first.x <= inputRect.first.x && globalRect.first.y <= inputRect.first.y;
  const bool isMaxInside = globalRect.second.x >= inputRect.second.x && globalRect.second.y >= inputRect.second.y;
  out << ((isMinInside && isMaxInside) ? "<TRUE>" : "<FALSE>");
}

void aleksandrov::rightShapes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  std::vector< std::vector< Vector > > allEdges;
  auto begin = polygons.begin();
  auto end = polygons.end();
  std::transform(begin, end, std::back_inserter(allEdges), EdgeVectorsCalculator{});
  out << std::count_if(allEdges.begin(), allEdges.end(), RightAngleChecker{});
}

