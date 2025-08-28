#include "commands.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <vector>
#include <numeric>
#include <limits>
#include "polygon.hpp"
#include <streamGuard.hpp>

namespace
{
  bool is_even(const brevnov::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool is_odd(const brevnov::Polygon& polygon)
  {
    return !is_even(polygon);
  }

  bool accept_all(const brevnov::Polygon&)
  {
    return true;
  }

  struct VertexPred
  {
    size_t count;
    bool operator()(const brevnov::Polygon& polygon)
    {
      return polygon.points.size() == count;
    }
  };

  template< typename Predicate >
  double area_sum(const std::vector< brevnov::Polygon >& polygons, Predicate p)
  {
    std::vector< brevnov::Polygon > filter;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filter), p);
    std::vector< double > areas;
    std::transform(filter.begin(), filter.end(), std::back_inserter(areas), brevnov::get_area);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double area_even(const std::vector< brevnov::Polygon >& polygons)
  {
    return area_sum(polygons, is_even);
  }

  double area_odd(const std::vector< brevnov::Polygon >& polygons)
  {
    return area_sum(polygons, is_odd);
  }

  double area_mean(const std::vector< brevnov::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons");
    }
    return area_sum(polygons, accept_all) / polygons.size();
  }

  double area_num(const std::vector< brevnov::Polygon >& polygons, size_t n)
  {
    return area_sum(polygons, VertexPred{ n });
  }

  bool compare_vertex(const brevnov::Polygon& polygon1, const brevnov::Polygon& polygon2)
  {
    return polygon1.points.size() < polygon2.points.size();
  }

  bool compare_area(const brevnov::Polygon& polygon1, const brevnov::Polygon& polygon2)
  {
    return brevnov::get_area(polygon1) < brevnov::get_area(polygon2);
  }

  void max_area(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compare_area));
    brevnov::StreamGuard stream(out);
    out << std::fixed << std::setprecision(1) << brevnov::get_area(max) << "\n";
  }

  void max_vertex(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compare_vertex));
    out << max.points.size() << "\n";
  }

  void min_area(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compare_area));
    brevnov::StreamGuard stream(out);
    out << std::fixed << std::setprecision(1) << brevnov::get_area(min) << "\n";
  }

  void min_vertex(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compare_vertex));
    out << min.points.size() << "\n";
  }

  template< typename Predicate >
  size_t count_if(const std::vector< brevnov::Polygon >& polygons, Predicate p)
  {
    std::vector< brevnov::Polygon > filter;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filter), p);
    return filter.size();
  }

  void count_even(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    out << count_if(polygons, is_even) << "\n";
  }

  void count_odd(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    out << count_if(polygons, is_odd) << "\n";
  }

  void count_num(const std::vector< brevnov::Polygon >& polygons, std::ostream& out, size_t n)
  {
    out << count_if(polygons, VertexPred{ n }) << "\n";
  }

  int point_f(const brevnov::Point& A, const brevnov::Point& B, const brevnov::Point& C)
  {
    int cross = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
    if (cross > 0)
    {
      return 1;
    }
    if (cross < 0)
    {
      return -1;
    }
    return 0;
  }

  bool on_segment(const brevnov::Point& A, const brevnov::Point& B, const brevnov::Point& P)
  {
    if (P.x >= std::min(A.x, B.x) && P.x <= std::max(A.x, B.x) && P.y >= std::min(A.y, B.y) && P.y <= std::max(A.y, B.y))
    {
      int cross = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
      return cross == 0;
    }
    return false;
  }

  bool segment_insert(const brevnov::Point& A, const brevnov::Point& B, const brevnov::Point& C, const brevnov::Point& D)
  {
    int ccw1 = point_f(A, B, C);
    int ccw2 = point_f(A, B, D);
    int ccw3 = point_f(C, D, A);
    int ccw4 = point_f(C, D, B);
    bool pred1 = ccw1 * ccw2 < 0;
    pred1 = pred1 && ccw3 * ccw4 < 0;
    if (pred1)
    {
      return true;
    }
    pred1 = ccw1 == 0;
    pred1 = pred1 && ccw2 == 0;
    pred1 = pred1 && ccw3 == 0;
    pred1 = pred1 && ccw4 == 0;
    if (pred1)
    {
      return !((std::max(A.x, B.x) < std::min(C.x, D.x) || std::max(C.x, D.x) < std::min(A.x, B.x))
        || (std::max(A.y, B.y) < std::min(C.y, D.y) || std::max(C.y, D.y) < std::min(A.y, B.y)));
    }
    pred1 = ccw1 == 0 && on_segment(A, B, C);
    pred1 = pred1 || (ccw2 == 0 && on_segment(A, B, D));
    pred1 = pred1 || (ccw3 == 0 && on_segment(C, D, A));
    pred1 = pred1 || (ccw4 == 0 && on_segment(C, D, B));
    return pred1;
  }

  struct Bounds
  {
    int min_x;
    int max_x;
    int min_y;
    int max_y;
  };

  struct BoundsReducer
  {
    Bounds operator()(Bounds acc, const brevnov::Point& point) const
    {
      return {std::min(acc.min_x, point.x), std::max(acc.max_x, point.x),
        std::min(acc.min_y, point.y), std::max(acc.max_y, point.y)};
    }
  };

  Bounds calculate_bounds(const brevnov::Polygon& poly)
  {
    if (poly.points.empty())
    {
      return {std::numeric_limits<int>::max(), std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
    }
    Bounds initial{poly.points[0].x, poly.points[0].x, poly.points[0].y, poly.points[0].y};
    return std::accumulate(poly.points.begin(), poly.points.end(), initial, BoundsReducer());
  }

  bool check_bounding_box(const brevnov::Polygon& poly1, const brevnov::Polygon& poly2)
  {
    if (poly1.points.empty() || poly2.points.empty())
    {
      return false;
    }
    const Bounds b1 = calculate_bounds(poly1);
    const Bounds b2 = calculate_bounds(poly2);
    return !(b1.max_x < b2.min_x || b2.max_x < b1.min_x || b1.max_y < b2.min_y || b2.max_y < b1.min_y);
  }

  struct PointPair
  {
    const brevnov::Point * current;
    const brevnov::Point * next;
  };

  struct PolygonPointPairs
  {
    explicit PolygonPointPairs(const brevnov::Polygon& p):
      poly(p),
      i(0),
      j(p.points.size()-1),
      initialized(false)
    {}
    bool get_next(PointPair& pair)
    {
      if (!initialized)
      {
        if (poly.points.empty())
        {
          return false;
        }
        pair.current = &poly.points[j];
        pair.next = &poly.points[i];
        initialized = true;
        return true;
      }
      if (++i >= poly.points.size())
      {
        return false;
      }
      j = i - 1;
      pair.current = &poly.points[j];
      pair.next = &poly.points[i];
      return true;
    }
    const brevnov::Polygon& poly;
    size_t i, j;
    bool initialized;
  };

class PointInPolygonChecker
  {
    explicit PointInPolygonChecker(const brevnov::Point& p):
      test_point(p),
      inside(false)
    {}
    void process_pair(const PointPair& pair)
    {
      const brevnov::Point& p1 = *pair.current;
      const brevnov::Point& p2 = *pair.next;
      if ((p1.y > test_point.y) != (p2.y > test_point.y))
      {
        if (test_point.x < (p2.x - p1.x) * (test_point.y - p1.y) / (p2.y - p1.y) + p1.x)
        {
          inside = !inside;
        }
      }
    }
    const brevnov::Point& test_point;
    bool inside;
  };

  bool point_in_poly(const brevnov::Point& point, const brevnov::Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      return false;
    }
    struct State
    {
      bool inside = false;
      const brevnov::Point* prev_point = nullptr;
    };
    struct RayCrossingChecker
    {
      explicit RayCrossingChecker(const brevnov::Point& pt):
        point(pt)
      {}
      State operator()(State state, const brevnov::Point& current_point) const
      {
        if (state.prev_point)
        {
          const brevnov::Point& p1 = current_point;
          const brevnov::Point& p2 = *state.prev_point;
          if ((p1.y > point.y) != (p2.y > point.y))
          {
            double x_intersect = (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x;
            if (point.x < x_intersect)
            {
              state.inside = !state.inside;
            }
          }
        }
        state.prev_point = &current_point;
        return state;
      }
      const brevnov::Point& point;
    };
    State init_state;
    init_state.prev_point = &polygon.points.back();
    RayCrossingChecker checker(point);
    State result = std::accumulate(polygon.points.begin(), polygon.points.end(),init_state, checker);
    return result.inside;
  }

  struct SegmentIntersectionChecker
  {
    bool operator()(int i) const
    {
      const brevnov::Point& A = poly1.points[i];
      const brevnov::Point& B = poly1.points[(i + 1) % poly1.points.size()];
      struct InnerChecker
      {
        bool operator()(int j) const
        {
          const brevnov::Point& C = poly2.points[j];
          const brevnov::Point& D = poly2.points[(j + 1) % poly2.points.size()];
          return segment_insert(A, B, C, D);
        }
        const brevnov::Point& A;
        const brevnov::Point& B;
        const brevnov::Polygon& poly2;
      };
      InnerChecker inner{A, B, poly2};
      const int n2 = poly2.points.size();
      std::vector< int > indices(n2);
      std::iota(indices.begin(), indices.end(), 0);
      return std::any_of(indices.begin(), indices.end(), inner);
    }
    const brevnov::Polygon& poly1;
    const brevnov::Polygon& poly2;
  };

  bool poly_intersect(const brevnov::Polygon& poly1, const brevnov::Polygon& poly2)
  {
    if (!check_bounding_box(poly1, poly2))
    {
      return false;
    }
    int n1 = poly1.points.size();
    std::vector< int > indices(n1);
    std::iota(indices.begin(), indices.end(), 0);
    SegmentIntersectionChecker checker{poly1, poly2};
    bool segments_intersect = std::any_of(indices.begin(), indices.end(), checker);
    if (segments_intersect)
    {
      return true;
    }
    return (!poly1.points.empty() && point_in_poly(poly1.points[0], poly2))
      || (!poly2.points.empty() && point_in_poly(poly2.points[0], poly1));
  }

  struct Check_intersect
  {
    bool operator()(const brevnov::Polygon & a)
    {
      return poly_intersect(a, p);
    }
    const brevnov::Polygon & p;
  };
}

void brevnov::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  double result = 0.0;
  std::map< std::string, std::function< double() > > subcommands;
  subcommands["EVEN"] = std::bind(area_even, std::cref(polygons));
  subcommands["ODD"] = std::bind(area_odd, std::cref(polygons));
  subcommands["MEAN"] = std::bind(area_mean, std::cref(polygons));
  try
  {
    result = subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t n = std::stoull(subcommand);
    if (n < 3)
    {
      throw std::logic_error("Few vertices");
    }
    result = area_num(polygons, n);
  }
  brevnov::StreamGuard stream(out);
  out << std::fixed << std::setprecision(1) << result << "\n";
}

void brevnov::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(max_area, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(max_vertex, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("Unknown command");
  }
}

void brevnov::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(min_area, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(min_vertex, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("Unknown command");
  }
}

void brevnov::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["EVEN"] = std::bind(count_even, std::cref(polygons), std::ref(out));
  subcommands["ODD"] = std::bind(count_odd, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t n = std::stoull(subcommand);
    if (n < 3)
    {
      throw std::logic_error("Few vertices");
    }
    count_num(polygons, out, n);
  }
}

void brevnov::rightshapes(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (in.peek() != '\n')
  {
    throw std::logic_error("ERROR: Do not need any parameters");
  }
  out << std::count_if(polygons.cbegin(), polygons.cend(), has_right_angle) << '\n';
}

void brevnov::intersections(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  Polygon p;
  in >> p;
  if (!in || !(in.peek() == '\n'))
  {
    throw std::logic_error("ERROR: Not correct parameters");
  }
  Check_intersect check{p};
  out << std::count_if(data.begin(),data.end(), check) << '\n';
}
