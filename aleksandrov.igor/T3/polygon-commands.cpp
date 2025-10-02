#include "polygon-commands.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <map>
#include <stream-guard.hpp>

namespace
{
  using namespace aleksandrov;

  double abs(double num) noexcept
  {
    return num < 0 ? -num : num;
  }

  double det(const Point& a, const Point& b) noexcept
  {
    return a.x * b.y - a.y * b.x;
  }

  double calcArea(const Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      return 0.0;
    }
    auto begin = polygon.points.begin();
    auto end = polygon.points.end();
    auto plus = std::plus<>{};
    double sum = std::inner_product(begin, end - 1, begin + 1, 0.0, plus, det);
    sum += det(polygon.points.back(), polygon.points.front());
    return abs(sum) / 2.0;
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
    size_t n;

    bool operator()(const Polygon& polygon) const
    {
      return polygon.points.size() == n;
    }
  };

  struct PointXComparator
  {
    bool operator()(const Point& a, const Point& b) const
    {
      return a.x < b.x;
    }
  };

  struct PointYComparator
  {
    bool operator()(const Point& a, const Point& b) const
    {
      return a.y < b.y;
    }
  };

  FrameRect getFrameRect(const Polygon& polygon)
  {
    const auto& pts = polygon.points;
    if (pts.empty())
    {
      return {};
    }
    auto minmaxX = std::minmax_element(pts.begin(), pts.end(), PointXComparator{});
    auto minmaxY = std::minmax_element(pts.begin(), pts.end(), PointYComparator{});

    return { Point{ minmaxX.first->x, minmaxY.first->y }, Point{ minmaxX.second->x, minmaxY.second->y } };
  }

  struct MinXComparator
  {
    bool operator()(const FrameRect& a, const FrameRect& b) const
    {
      return a.first.x < b.first.x;
    }
  };

  struct MinYComparator
  {
    bool operator()(const FrameRect& a, const FrameRect& b) const
    {
      return a.first.y < b.first.y;
    }
  };

  struct MaxXComparator
  {
    bool operator()(const FrameRect& a, const FrameRect& b) const
    {
      return a.second.x < b.second.x;
    }
  };

  struct MaxYComparator
  {
    bool operator()(const FrameRect& a, const FrameRect& b) const
    {
      return a.second.y < b.second.y;
    }
  };

  FrameRect mergeFrameRects(const std::vector< FrameRect >& frameRects)
  {
    if (frameRects.empty())
    {
      return {};
    }
    auto minX = std::min_element(frameRects.begin(), frameRects.end(), MinXComparator{});
    auto minY = std::min_element(frameRects.begin(), frameRects.end(), MinYComparator{});
    auto maxX = std::max_element(frameRects.begin(), frameRects.end(), MaxXComparator{});
    auto maxY = std::max_element(frameRects.begin(), frameRects.end(), MaxYComparator{});

    return { { minX->first.x, minY->first.y }, { maxX->second.x, maxY->second.y } };
  }

  struct PerpendicularChecker
  {
    const Polygon& polygon;

    bool operator()(size_t i) const
    {
      size_t size = polygon.points.size();
      auto a = polygon.points[i];
      auto b = polygon.points[(i + 1) % size];
      auto c = polygon.points[(i + 2) % size];

      double xAB = b.x - a.x;
      double yAB = b.y - a.y;
      double xBC = c.x - b.x;
      double yBC = c.y - b.y;

      return xAB * yAB + xBC * yBC == 0;
    }
  };

  struct RightAngleChecker
  {
    bool operator()(const Polygon& polygon) const
    {
      std::vector< size_t > indexes(polygon.points.size());
      std::iota(indexes.begin(), indexes.end(), 0);
      PerpendicularChecker checker{ polygon };
      return std::any_of(indexes.begin(), indexes.end(), checker);
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

void aleksandrov::processCommands(std::istream& in, std::ostream& out, Polygons& polygons)
{
  std::map< std::string, std::function< void() > > commands;

  commands["AREA"] = std::bind(execArea, std::cref(polygons), std::ref(in), std::ref(out));
  size_t max = polygons.size() - 1;
  commands["MAX"] = std::bind(execIthSmallest, std::cref(polygons), max, std::ref(in), std::ref(out));
  commands["MIN"] = std::bind(execIthSmallest, std::cref(polygons), 0, std::ref(in), std::ref(out));
  commands["COUNT"] = std::bind(execCount, std::cref(polygons), std::ref(in), std::ref(out));
  commands["INFRAME"] = std::bind(execInFrame, std::cref(polygons), std::ref(in), std::ref(out));
  commands["RIGHTSHAPES"] = std::bind(execRightShapes, std::cref(polygons), std::ref(out));

  std::string command;
  while (!(in >> command).eof())
  {
    try
    {
      commands.at(command)();
      out << '\n';
    }
    catch (const std::exception&)
    {
      if (in.fail())
      {
        in.clear(in.rdstate() ^ std::ios::failbit);
      }
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      out << "<INVALID COMMAND>\n";
    }
  }
}

void aleksandrov::execArea(const Polygons& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;

  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void() > > subcommands;

  using evenF = EvenVerticesChecker;
  using oddF = OddVerticesChecker;

  subcommands["EVEN"] = std::bind(execAreaIf< evenF >, std::cref(polygons), evenF{}, std::ref(out));
  subcommands["ODD"] = std::bind(execAreaIf< oddF >, std::cref(polygons), oddF{}, std::ref(out));
  subcommands["MEAN"] = std::bind(execAreaMean, std::cref(polygons), std::ref(out));

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
  using NF = NVerticesChecker;
  execAreaIf< NF >(polygons, NVerticesChecker{ numOfVertices }, out);
}

template< class F >
void aleksandrov::execAreaIf(const Polygons& polygons, F f, std::ostream& out)
{
  Polygons satisfying;
  size_t size = std::count_if(polygons.begin(), polygons.end(), f);
  satisfying.reserve(size);
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(satisfying), f);
  std::vector< double > areas(size);
  std::transform(satisfying.begin(), satisfying.end(), areas.begin(), calcArea);
  out << std::accumulate(areas.begin(), areas.end(), 0.0);
}

void aleksandrov::execAreaMean(const Polygons& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("There are no polygons!");
  }
  std::vector< double > areas(polygons.size());
  std::transform(polygons.begin(), polygons.end(), areas.begin(), calcArea);
  double areaSum = std::accumulate(areas.begin(), areas.end(), 0.0);
  out << areaSum / polygons.size();
}

void aleksandrov::execIthSmallest(const Polygons& polygons, size_t i, std::istream& in, std::ostream& out)
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

  const auto areaAcc = std::bind(calcArea, _1);
  using areaF = decltype(areaAcc);
  subcommands["AREA"] = std::bind(execIthSmallestImpl< areaF >, std::cref(polygons), std::ref(areaAcc), i, std::ref(out));

  auto verts = std::bind(std::mem_fn(&Polygon::points), _1);
  auto size = std::bind(std::mem_fn(&std::vector< Point >::size), verts);
  using vertsF = decltype(size);
  subcommands["VERTEXES"] = std::bind(execIthSmallestImpl< vertsF >, std::cref(polygons), std::ref(size), i, std::ref(out));

  subcommands.at(subcommand)();
}

template< class F >
void aleksandrov::execIthSmallestImpl(const Polygons& polygons, F f, size_t i, std::ostream& out)
{
  std::vector< decltype(f(polygons.front())) > data;
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

template< class Pred >
void aleksandrov::execCountIf(const Polygons& polygons, Pred p, std::ostream& out)
{
  out << std::count_if(polygons.begin(), polygons.end(), p);
}

void aleksandrov::execCount(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;

  std::map< std::string, std::function< void() > > subcommands;

  using evenF = EvenVerticesChecker;
  using oddF = OddVerticesChecker;

  subcommands["EVEN"] = std::bind(execCountIf< evenF >, std::cref(polygons), evenF{}, std::ref(out));
  subcommands["ODD"] = std::bind(execCountIf< oddF >, std::cref(polygons), oddF{}, std::ref(out));

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
  execCountIf(polygons, NVerticesChecker{ numOfVertices }, out);
}

void aleksandrov::execInFrame(const Polygons& polygons, std::istream& in, std::ostream& out)
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

  std::vector< FrameRect > rects(polygons.size());
  std::transform(polygons.begin(), polygons.end(), rects.begin(), getFrameRect);

  FrameRect merged = mergeFrameRects(rects);
  FrameRect input = getFrameRect(polygon);

  bool isMinInside = merged.first.x <= input.first.x && merged.first.y <= input.first.y;
  bool isMaxInside = merged.second.x >= input.second.x && merged.second.y >= input.second.y;

  out << (isMinInside && isMaxInside ? "<TRUE>" : "<FALSE>");
}

void aleksandrov::execRightShapes(const Polygons& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("There are no polygons!");
  }
  RightAngleChecker checker{};
  out << std::count_if(polygons.begin(), polygons.end(), checker);
}

