#include "commands.hpp"
#include <functional>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <cmath>
#include <map>
#include <streamguard.hpp>

namespace
{
  struct PredicateForVertices
  {
    size_t count;
    bool operator()(const kushekbaev::Polygon& polygon)
    {
      return polygon.points.size() == count;
    }
  };

  struct PointComparator
  {
    int dx = 0;
    int dy = 0;
    bool operator()(const kushekbaev::Point& a, const kushekbaev::Point& b)
    {
      return (a.x + dx) == b.x && (a.y + dy) == b.y;
    }
  };

  struct PolygonMatcher
  {
    const kushekbaev::Polygon& target;
    const kushekbaev::Polygon& other;
    bool operator()(const kushekbaev::Point& beforeShift)
    {
      int dx = beforeShift.x - target.points[0].x;
      int dy = beforeShift.y - target.points[0].y;
      return std::equal(target.points.begin(), target.points.end(), other.points.begin(), PointComparator{ dx, dy });
    }
  };

  struct AreaCalculator
  {
    double operator()(const kushekbaev::Point& a, const kushekbaev::Point& b)
    {
      return (a.x + b.x) * (a.y - b.y);
    }
  };

  struct RightAngleChecker
  {
    RightAngleChecker(const kushekbaev::Polygon& polygon):
      polygon(polygon)
    {}
    const kushekbaev::Polygon& polygon;
    bool operator()(const kushekbaev::Point& b)
    {
      size_t i = &b - &polygon.points[0];
      size_t n = polygon.points.size();
      const auto& a = polygon.points[(i + n - 1) % n];
      const auto& c = polygon.points[(i + 1) % n];
      double abx = a.x - b.x;
      double aby = a.y - b.y;
      double cbx = c.x - b.x;
      double cby = c.y - b.y;
      return (abx * cbx + aby * cby) == 0;
    }
  };

  struct PredicateForEvenPolygons
  {
    bool operator()(const kushekbaev::Polygon& polygon) const
    {
      return polygon.points.size() % 2 == 0;
    }
  };

  struct PredicateForOddPolygons
  {
    bool operator()(const kushekbaev::Polygon& polygon) const
    {
      return polygon.points.size() % 2 != 0;
    }
  };

  double getArea(const kushekbaev::Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      return 0.0;
    }
    std::vector< double > partial_areas;
    const auto& points = polygon.points;
    auto first1 = points.begin();
    auto last1 = std::prev(points.end());
    auto first2 = std::next(points.begin());
    std::transform(first1, last1, first2, std::back_inserter(partial_areas), AreaCalculator{});
    double last_segment = AreaCalculator{}(points.back(), points.front());
    partial_areas.push_back(last_segment);
    double total = std::accumulate(partial_areas.begin(), partial_areas.end(), 0.0);
    return std::abs(total) / 2.0;
  }

  bool hasRightAngle(const kushekbaev::Polygon& polygon)
  {
    if (polygon.points.size() < kushekbaev::MIN_NUMBER_OF_VERTICES_IN_POLYGON)
    {
      return false;
    }
    return std::any_of(polygon.points.begin(), polygon.points.end(), RightAngleChecker{ polygon });
  }

  bool isCompabitebleByOverlay(const kushekbaev::Polygon& collectedPolygon, const kushekbaev::Polygon& inputedPolygon)
  {
    if (collectedPolygon.points.size() != inputedPolygon.points.size())
    {
      return false;
    }
    PolygonMatcher matcher{ collectedPolygon, inputedPolygon };
    return std::any_of(inputedPolygon.points.begin(), inputedPolygon.points.end(), matcher);
  }

  bool isEven(const kushekbaev::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const kushekbaev::Polygon& polygon)
  {
    return !isEven(polygon);
  }

  double calculateTotalArea(const std::vector< kushekbaev::Polygon >& polygons)
  {
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double countWithPredicate(const std::vector< kushekbaev::Polygon >& polygons, PredicateForVertices predicate)
  {
    return std::count_if(polygons.begin(), polygons.end(), predicate);
  }

  template< typename Predicate >
  double calculateTotalAreaWithPredicate(const std::vector< kushekbaev::Polygon >& polygons, Predicate predicate)
  {
    std::vector< kushekbaev::Polygon > predicated;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(predicated), predicate);
    return calculateTotalArea(predicated);
  }

  bool compareArea(const kushekbaev::Polygon& polygon1, const kushekbaev::Polygon& polygon2)
  {
    return getArea(polygon1) < getArea(polygon2);
  }

  bool compareNumOfVertices(const kushekbaev::Polygon& polygon1, const kushekbaev::Polygon& polygon2)
  {
    return polygon1.points.size() < polygon2.points.size();
  }

  double areaEven(const std::vector< kushekbaev::Polygon >& polygons)
  {
    std::vector< kushekbaev::Polygon > evenPolygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(evenPolygons), PredicateForEvenPolygons{});
    return calculateTotalArea(evenPolygons);
  }

  double areaOdd(const std::vector< kushekbaev::Polygon >& polygons)
  {
    std::vector< kushekbaev::Polygon > oddPolygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(oddPolygons), PredicateForOddPolygons{});
    return calculateTotalArea(oddPolygons);
  }

  double areaMean(const std::vector< kushekbaev::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("There are no polygons in vector!");
    }
    return calculateTotalArea(polygons) / polygons.size();
  }

  double areaNum(const std::vector< kushekbaev::Polygon >& polygons, size_t num_of_vertices)
  {
    std::vector< kushekbaev::Polygon > filteredPolygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), PredicateForVertices{num_of_vertices});
    return calculateTotalArea(filteredPolygons);
  }

  void maxArea(const std::vector< kushekbaev::Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons to process");
    }
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareArea));
    kushekbaev::StreamGuard s(out);
    out << std::fixed << std::setprecision(1) << getArea(max) << "\n";
  }

  void maxVertices(const std::vector< kushekbaev::Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons to process");
    }
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareNumOfVertices));
    out << max.points.size() << "\n";
  }

  void minArea(const std::vector< kushekbaev::Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons to process");
    }
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareArea));
    kushekbaev::StreamGuard s(out);
    out << std::fixed << std::setprecision(1) << getArea(min) << "\n";
  }

  void minVertices(const std::vector< kushekbaev::Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons to process");
    }
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareNumOfVertices));
    out << min.points.size() << "\n";
  }

  void countEven(const std::vector< kushekbaev::Polygon >& polygons, std::ostream& out)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEven) << '\n';
  }

  void countOdd(const std::vector< kushekbaev::Polygon >& polygons, std::ostream& out)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd) << '\n';
  }

  void countNum(const std::vector< kushekbaev::Polygon >& polygons, std::ostream& out, size_t num_of_vertices)
  {
    out << countWithPredicate(polygons, PredicateForVertices{ num_of_vertices }) << '\n';
  }
}

void kushekbaev::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  double result = 0;
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< double() > > subcommands;
  subcommands["EVEN"] = std::bind(areaEven, std::cref(polygons));
  subcommands["ODD"] = std::bind(areaOdd, std::cref(polygons));
  subcommands["MEAN"] = std::bind(areaMean, std::cref(polygons));
  try
  {
    result = subcommands.at(subcommand)();
  }
  catch (const std::out_of_range&)
  {
    size_t num_of_vertices = std::stoull(subcommand);
    if (num_of_vertices < MIN_NUMBER_OF_VERTICES_IN_POLYGON)
    {
      throw std::out_of_range("Too few vertices!");
    }
    result = areaNum(polygons, num_of_vertices);
  }
  kushekbaev::StreamGuard s(out);
  out << std::fixed << std::setprecision(1) << result << "\n";
}

void kushekbaev::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(maxArea, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(maxVertices, std::cref(polygons), std::ref(out));
  subcommands.at(subcommand)();
}

void kushekbaev::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(minArea, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(minVertices, std::cref(polygons), std::ref(out));
  subcommands.at(subcommand)();
}

void kushekbaev::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["EVEN"] = std::bind(countEven, std::cref(polygons), std::ref(out));
  subcommands["ODD"] = std::bind(countOdd, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (const std::out_of_range&)
  {
    size_t num_of_vertices = std::stoull(subcommand);
    if (num_of_vertices < MIN_NUMBER_OF_VERTICES_IN_POLYGON)
    {
      throw;
    }
    countNum(polygons, out, num_of_vertices);
  }
}

void kushekbaev::rightShapes(std::ostream& out, const std::vector< Polygon >& polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), hasRightAngle) << "\n";
}

void kushekbaev::same(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon polygon;
  in >> polygon;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto tmp = std::bind(isCompabitebleByOverlay, std::placeholders::_1, polygon);
  out << std::count_if(polygons.begin(), polygons.end(), tmp) << "\n";
}
