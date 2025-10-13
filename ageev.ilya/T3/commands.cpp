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
#include "geom.hpp"
#include "streamGuard.hpp"

namespace
{
  struct IsEven
  {
    bool operator()(const ageev::Polygon& polygon) const
    {
      return polygon.points.size() % 2 == 0;
    }
  };

  struct IsOdd
  {
    bool operator()(const ageev::Polygon& polygon) const
    {
      return polygon.points.size() % 2 != 0;
    }
  };

  struct AcceptAll
  {
    bool operator()(const ageev::Polygon&) const
    {
      return true;
    }
  };

  struct VertexPred
  {
    size_t count;
    bool operator()(const ageev::Polygon& polygon) const
    {
      return polygon.points.size() == count;
    }
  };

  template< typename Predicate >
  double area_sum(const std::vector< ageev::Polygon >& polygons, Predicate p)
  {
    std::vector< ageev::Polygon > filter;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filter), p);
    std::vector< double > areas;
    std::transform(filter.begin(), filter.end(), std::back_inserter(areas), ageev::get_area);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double area_even(const std::vector< ageev::Polygon >& polygons)
  {
    return area_sum(polygons, IsEven{});
  }

  double area_odd(const std::vector< ageev::Polygon >& polygons)
  {
    return area_sum(polygons, IsOdd{});
  }

  double area_mean(const std::vector< ageev::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons");
    }
    return area_sum(polygons, AcceptAll{}) / polygons.size();
  }

  double area_num(const std::vector< ageev::Polygon >& polygons, size_t n)
  {
    return area_sum(polygons, VertexPred{ n });
  }

  struct CompareVertex
  {
    bool operator()(const ageev::Polygon& polygon1, const ageev::Polygon& polygon2) const
    {
      return polygon1.points.size() < polygon2.points.size();
    }
  };

  struct CompareArea
  {
    bool operator()(const ageev::Polygon& polygon1, const ageev::Polygon& polygon2) const
    {
      return ageev::get_area(polygon1) < ageev::get_area(polygon2);
    }
  };

  void max_area(const std::vector< ageev::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), CompareArea{}));
    ageev::StreamGuard stream(out);
    out << std::fixed << std::setprecision(1) << ageev::get_area(max) << "\n";
  }

  void max_vertex(const std::vector< ageev::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), CompareVertex{}));
    out << max.points.size() << "\n";
  }

  void min_area(const std::vector< ageev::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), CompareArea{}));
    ageev::StreamGuard stream(out);
    out << std::fixed << std::setprecision(1) << ageev::get_area(min) << "\n";
  }

  void min_vertex(const std::vector< ageev::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), CompareVertex{}));
    out << min.points.size() << "\n";
  }

  template< typename Predicate >
  size_t count_if(const std::vector< ageev::Polygon >& polygons, Predicate p)
  {
    return std::count_if(polygons.begin(), polygons.end(), p);
  }

  void count_even(const std::vector< ageev::Polygon >& polygons, std::ostream& out)
  {
    out << count_if(polygons, IsEven{}) << "\n";
  }

  void count_odd(const std::vector< ageev::Polygon >& polygons, std::ostream& out)
  {
    out << count_if(polygons, IsOdd{}) << "\n";
  }

  void count_num(const std::vector< ageev::Polygon >& polygons, std::ostream& out, size_t n)
  {
    out << count_if(polygons, VertexPred{ n }) << "\n";
  }

  struct PointOrientation
  {
    int operator()(const ageev::Point& A, const ageev::Point& B, const ageev::Point& C) const
    {
      int cross = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
      if (cross > 0) return 1;
      if (cross < 0) return -1;
      return 0;
    }
  };

  struct OnSegment
  {
    bool operator()(const ageev::Point& A, const ageev::Point& B, const ageev::Point& P) const
    {
      if (P.x >= std::min(A.x, B.x) && P.x <= std::max(A.x, B.x) && 
          P.y >= std::min(A.y, B.y) && P.y <= std::max(A.y, B.y))
      {
        PointOrientation orient;
        int cross = orient(A, B, P);
        return cross == 0;
      }
      return false;
    }
  };

  struct SegmentIntersectionChecker
  {
    bool operator()(const ageev::Point& A, const ageev::Point& B, 
                   const ageev::Point& C, const ageev::Point& D) const
    {
      PointOrientation orient;
      OnSegment on_segment;

      int ccw1 = orient(A, B, C);
      int ccw2 = orient(A, B, D);
      int ccw3 = orient(C, D, A);
      int ccw4 = orient(C, D, B);

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
  };

  struct Bounds
  {
    int min_x;
    int max_x;
    int min_y;
    int max_y;
  };

  struct BoundsReducer
  {
    Bounds operator()(Bounds acc, const ageev::Point& point) const
    {
      return {std::min(acc.min_x, point.x), std::max(acc.max_x, point.x),
              std::min(acc.min_y, point.y), std::max(acc.max_y, point.y)};
    }
  };

  Bounds calculate_bounds(const ageev::Polygon& poly)
  {
    if (poly.points.empty())
    {
      return {std::numeric_limits<int>::max(), std::numeric_limits<int>::min(),
              std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
    }
    Bounds initial{poly.points[0].x, poly.points[0].x, poly.points[0].y, poly.points[0].y};
    return std::accumulate(poly.points.begin(), poly.points.end(), initial, BoundsReducer());
  }

  struct BoundingBoxChecker
  {
    bool operator()(const ageev::Polygon& poly1, const ageev::Polygon& poly2) const
    {
      if (poly1.points.empty() || poly2.points.empty())
      {
        return false;
      }
      const Bounds b1 = calculate_bounds(poly1);
      const Bounds b2 = calculate_bounds(poly2);
      return !(b1.max_x < b2.min_x || b2.max_x < b1.min_x || 
               b1.max_y < b2.min_y || b2.max_y < b1.min_y);
    }
  };

  struct PointInPolygonChecker
  {
    explicit PointInPolygonChecker(const ageev::Point& pt): point(pt) {}
    
    struct State
    {
      bool inside = false;
      const ageev::Point* prev_point = nullptr;
    };

    struct RayCrossingChecker
    {
      explicit RayCrossingChecker(const ageev::Point& pt): point(pt) {}

      State operator()(State state, const ageev::Point& current_point) const
      {
        if (state.prev_point)
        {
          const ageev::Point& p1 = current_point;
          const ageev::Point& p2 = *state.prev_point;
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
      const ageev::Point& point;
    };

    bool operator()(const ageev::Polygon& polygon) const
    {
      if (polygon.points.size() < 3)
      {
        return false;
      }

      State init_state;
      init_state.prev_point = &polygon.points.back();
      RayCrossingChecker checker(point);
      State result = std::accumulate(polygon.points.begin(), polygon.points.end(), init_state, checker);
      return result.inside;
    }

    const ageev::Point& point;
  };

  struct PolygonIntersectionChecker
  {
    bool operator()(const ageev::Polygon& poly1, const ageev::Polygon& poly2) const
    {
      BoundingBoxChecker bbox_check;
      if (!bbox_check(poly1, poly2))
      {
        return false;
      }

      SegmentIntersectionChecker seg_check;
      const int n1 = poly1.points.size();
      const int n2 = poly2.points.size();

      for (int i = 0; i < n1; ++i)
      {
        const ageev::Point& A = poly1.points[i];
        const ageev::Point& B = poly1.points[(i + 1) % n1];

        for (int j = 0; j < n2; ++j)
        {
          const ageev::Point& C = poly2.points[j];
          const ageev::Point& D = poly2.points[(j + 1) % n2];

          if (seg_check(A, B, C, D))
          {
            return true;
          }
        }
      }

      PointInPolygonChecker point_checker{poly1.points[0]};
      return (!poly1.points.empty() && point_checker(poly2)) ||
             (!poly2.points.empty() && PointInPolygonChecker{poly2.points[0]}(poly1));
    }
  };

  struct CheckIntersect
  {
    explicit CheckIntersect(const ageev::Polygon& p): polygon(p) {}

    bool operator()(const ageev::Polygon& other) const
    {
      PolygonIntersectionChecker checker;
      return checker(polygon, other);
    }

    const ageev::Polygon& polygon;
  };
}

void ageev::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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
  ageev::StreamGuard stream(out);
  out << std::fixed << std::setprecision(1) << result << "\n";
}

void ageev::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

void ageev::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

void ageev::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

void ageev::rightshapes(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (in.peek() != '\n')
  {
    throw std::logic_error("ERROR: Do not need any parameters");
  }
  out << std::count_if(polygons.cbegin(), polygons.cend(), has_right_angle) << '\n';
}

void ageev::intersections(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  Polygon p;
  in >> p;
  if (!in || !(in.peek() == '\n'))
  {
    throw std::logic_error("ERROR: Not correct parameters");
  }
  CheckIntersect check{p};
  out << std::count_if(data.begin(), data.end(), check) << '\n';
}
