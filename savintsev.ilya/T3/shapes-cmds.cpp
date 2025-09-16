#include "shapes-cmds.h"
#include <string>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <cmath>
#include <set>
#include <scope-guard.hpp>
#include "geometry-utils.h"

namespace
{
  double sum_with_area(double sum, const savintsev::Polygon & a)
  {
    return savintsev::calc_polygon_area(a) + sum;
  }

  bool is_polygon_even(const savintsev::Polygon & a)
  {
    return !(a.points.size() % 2);
  }

  bool is_polygon_odd(const savintsev::Polygon & a)
  {
    return a.points.size() % 2;
  }

  bool compare_areas(const savintsev::Polygon & a, const savintsev::Polygon & b)
  {
    return calc_polygon_area(a) < calc_polygon_area(b);
  }

  bool compare_vert_amt(const savintsev::Polygon & a, const savintsev::Polygon & b)
  {
    return a.points.size() < b.points.size();
  }

  struct CheckPolygonVert
  {
    bool operator()(const savintsev::Polygon & a) const
    {
      return a.points.size() == num;
    }
    size_t num;
  };

  struct PointShifter
  {
    bool operator()(const savintsev::Point & point) const
    {
      return shifted.find(point) != shifted.end();
    }
    const std::set< savintsev::Point > & shifted;
  };

  struct PointMatchChecker
  {
    bool operator()(const savintsev::Point & q) const
    {
      savintsev::Point shifted{q.x + dx, q.y + dy};
      return shifter(shifted);
    }
    int dx;
    int dy;
    const PointShifter & shifter;
  };

  struct InnerPointProcessor
  {
    bool operator()(const savintsev::Point & pb) const
    {
      int dx = pa.x - pb.x;
      int dy = pa.y - pb.y;
      PointShifter shifter{set_p};
      return std::all_of(a.points.begin(), a.points.end(), PointMatchChecker{dx, dy, shifter});
    }
    const savintsev::Point & pa;
    const savintsev::Polygon & a;
    const std::set< savintsev::Point > & set_p;
  };

  struct PointPairProcessor
  {
    bool operator()(const savintsev::Point & pa) const
    {
      InnerPointProcessor processor{pa, a, set_p};
      return std::any_of(a.points.begin(), a.points.end(), processor);
    }
    const savintsev::Polygon & a;
    const std::set< savintsev::Point > & set_p;
  };

  struct CheckSame
  {
    bool operator()(const savintsev::Polygon & a) const
    {
      if (a.points.size() != p.points.size())
      {
        return false;
      }

      std::set< savintsev::Point > set_p(p.points.begin(), p.points.end());
      PointPairProcessor processor{a, set_p};
      return std::any_of(p.points.begin(), p.points.end(), processor);
    }
    const savintsev::Polygon & p;
  };

  struct SegmentIntersectionCounter
  {
    SegmentIntersectionCounter(const savintsev::Polygon & polyA, const savintsev::Polygon & polyB, size_t indexA):
      a(polyA),
      p(polyB),
      i(indexA)
    {}

    int operator()(int acc, size_t j) const
    {
      const auto & a1 = a.points[j];
      const auto & a2 = a.points[(i + 1) % a.points.size()];
      const auto & b1 = p.points[j];
      const auto & b2 = p.points[(j + 1) % p.points.size()];

      return acc + savintsev::is_lines_int(a1, a2, b1, b2);
    }

    const savintsev::Polygon & a;
    const savintsev::Polygon & p;
    size_t i;
  };

  struct CheckIntersect
  {
    CheckIntersect(const savintsev::Polygon & polygon):
      p(polygon)
    {}

    int operator()(const savintsev::Polygon & a) const
    {
      std::vector< size_t > a_ix(a.points.size());
      std::iota(a_ix.begin(), a_ix.end(), 0);

      return std::accumulate(a_ix.begin(), a_ix.end(), 0, PolygonIntersectionAccumulator(a, p));
    }

    struct PolygonIntersectionAccumulator
    {
      PolygonIntersectionAccumulator(const savintsev::Polygon & polyA, const savintsev::Polygon & polyB):
        a(polyA),
        p(polyB)
      {}

      int operator()(int acc, size_t i) const
      {
        std::vector< size_t > p_ix(p.points.size());
        std::iota(p_ix.begin(), p_ix.end(), 0);

        return acc + std::accumulate(p_ix.begin(), p_ix.end(), 0, SegmentIntersectionCounter(a, p, i));
      }

      const savintsev::Polygon & a;
      const savintsev::Polygon & p;
    };

    const savintsev::Polygon & p;
  };
}

void savintsev::area(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  size_t num = 0;
  ScopeGuard guard(out);
  out.precision(1);
  out << std::fixed;
  in >> num;
  if (in)
  {
    if (num <= 2)
    {
      throw std::runtime_error("area: invalid arg");
    }
    CheckPolygonVert is{num};
    std::vector< Polygon > temp;
    std::copy_if(data.begin(), data.end(), std::back_inserter(temp), is);
    out << std::accumulate(temp.begin(), temp.end(), 0.0, sum_with_area) << '\n';
    return;
  }
  in.clear(in.rdstate() ^ std::ios::failbit);
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN")
  {
    std::vector< Polygon > temp;
    std::copy_if(data.begin(), data.end(), std::back_inserter(temp), is_polygon_even);
    out << std::accumulate(temp.begin(), temp.end(), 0.0, sum_with_area);
  }
  else if (subcommand == "ODD")
  {
    std::vector< Polygon > temp;
    std::copy_if(data.begin(), data.end(), std::back_inserter(temp), is_polygon_odd);
    out << std::accumulate(temp.begin(), temp.end(), 0.0, sum_with_area);
  }
  else if (subcommand == "MEAN")
  {
    if (data.empty())
    {
      throw std::runtime_error("area: div by zero: no polygons");
    }
    out << std::accumulate(data.begin(), data.end(), 0.0, sum_with_area) / data.size();
  }
  else
  {
    throw std::runtime_error("area: invalid arg");
  }
  out << '\n';
}

void savintsev::count(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  size_t num = 0;
  in >> num;
  if (in)
  {
    if (num <= 2)
    {
      throw std::runtime_error("count: invalid arg");
    }
    CheckPolygonVert is{num};
    out << std::count_if(data.begin(), data.end(), is) << '\n';
    return;
  }
  in.clear(in.rdstate() ^ std::ios::failbit);
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN")
  {
    out << std::count_if(data.begin(), data.end(), is_polygon_even);
  }
  else if (subcommand == "ODD")
  {
    out << std::count_if(data.begin(), data.end(), is_polygon_odd);
  }
  else
  {
    throw std::runtime_error("count: invalid arg");
  }
  out << '\n';
}

void savintsev::max(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  std::string subcommand;
  in >> subcommand;
  if (data.empty())
  {
    throw std::runtime_error("max: no polygons for search");
  }
  if (subcommand == "AREA")
  {
    ScopeGuard guard(out);
    out.precision(1);
    out << std::fixed;
    out << calc_polygon_area(*(std::max_element(data.begin(), data.end(), compare_areas)));
  }
  else if (subcommand == "VERTEXES")
  {
    out << std::max_element(data.begin(), data.end(), compare_vert_amt)->points.size();
  }
  else
  {
    throw std::runtime_error("max: invalid arg");
  }
  out << '\n';
}

void savintsev::min(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  std::string subcommand;
  in >> subcommand;
  if (data.empty())
  {
    throw std::runtime_error("min: no polygons for search");
  }
  if (subcommand == "AREA")
  {
    ScopeGuard guard(out);
    out.precision(1);
    out << std::fixed;
    out << calc_polygon_area(*(std::min_element(data.begin(), data.end(), compare_areas)));
  }
  else if (subcommand == "VERTEXES")
  {
    out << std::min_element(data.begin(), data.end(), compare_vert_amt)->points.size();
  }
  else
  {
    throw std::runtime_error("min: invalid arg");
  }
  out << '\n';
}

void savintsev::intersections(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  Polygon p;
  in >> p;
  if (!in || !(in.peek() == '\n'))
  {
    throw std::runtime_error("intersections: invalid polygon desc");
  }
  CheckIntersect check{p};
  out << std::count_if(data.begin(), data.end(), check) << '\n';
}

void savintsev::same(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  Polygon p;
  in >> p;
  if (!in || !(in.peek() == '\n'))
  {
    throw std::runtime_error("same: invalid polygon desc");
  }
  CheckSame check{p};
  out << std::count_if(data.begin(), data.end(), check) << '\n';
}

void savintsev::print(std::ostream & out, const std::vector< Polygon > & data)
{
  std::copy(data.begin(), data.end(), std::ostream_iterator< Polygon >(out, "\n"));
}
