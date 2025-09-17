#include "printCmd.hpp"
#include <map>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <streamGuard.hpp>

namespace shramko
{
  namespace details
  {
    bool isEven(const Polygon& poly)
    {
      return poly.points.size() % 2 == 0;
    }

    bool isOdd(const Polygon& poly)
    {
      return !isEven(poly);
    }

    bool hasVertexCount(const Polygon& poly, size_t count)
    {
      return poly.points.size() == count;
    }

    bool hasAreaLessThan(const Polygon& poly, const Polygon& ref)
    {
      return getPolygonArea(poly) < getPolygonArea(ref);
    }

    bool areaLess(const Polygon& lhs, const Polygon& rhs)
    {
      return getPolygonArea(lhs) < getPolygonArea(rhs);
    }

    bool vertexLess(const Polygon& lhs, const Polygon& rhs)
    {
      return lhs.points.size() < rhs.points.size();
    }

    struct LocalTriangle
    {
      shramko::Point prev;
      shramko::Point curr;
      shramko::Point next;
    };

    LocalTriangle makeLocalTriangle(const std::vector<Point>& points, size_t index)
    {
      size_t n = points.size();
      size_t prevIdx = (index + n - 1) % n;
      size_t nextIdx = (index + 1) % n;
      return { points[prevIdx], points[index], points[nextIdx] };
    }

    bool isRightAngle(const LocalTriangle& tri)
    {
      double vec1x = tri.prev.x - tri.curr.x;
      double vec1y = tri.prev.y - tri.curr.y;
      double vec2x = tri.next.x - tri.curr.x;
      double vec2y = tri.next.y - tri.curr.y;
      double dotProduct = vec1x * vec2x + vec1y * vec2y;
      return std::abs(dotProduct) < 1e-6;
    }

    bool hasRightAngle(const Polygon& poly)
    {
      if (poly.points.size() < 3)
      {
        return false;
      }
      for (size_t i = 0; i < poly.points.size(); ++i)
      {
        LocalTriangle tri = makeLocalTriangle(poly.points, i);
        if (isRightAngle(tri))
        {
          return true;
        }
      }
      return false;
    }

    double sumAreas(const std::vector< Polygon >& polys, bool (*pred)(const Polygon&))
    {
      std::vector< double > areas;
      std::transform(polys.begin(), polys.end(), std::back_inserter(areas),
        [](const Polygon& p) { return getPolygonArea(p); });
      std::vector< double > filteredAreas;
      std::copy_if(areas.begin(), areas.end(), std::back_inserter(filteredAreas), pred);
      return std::accumulate(filteredAreas.begin(), filteredAreas.end(), 0.0);
    }
  }

  void printArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("No polygons available");
    }
    std::string subcmd;
    in >> subcmd;
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);
    if (subcmd == "EVEN")
    {
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [](double acc, const Polygon& p) { return acc + (details::isEven(p) ? getPolygonArea(p) : 0.0); });
      out << sum;
    }
    else if (subcmd == "ODD")
    {
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [](double acc, const Polygon& p) { return acc + (details::isOdd(p) ? getPolygonArea(p) : 0.0); });
      out << sum;
    }
    else if (subcmd == "MEAN")
    {
      double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [](double acc, const Polygon& p) { return acc + getPolygonArea(p); });
      out << total / polygons.size();
    }
    else
    {
      size_t vertexCount = std::stoul(subcmd);
      if (vertexCount < 3)
      {
        throw std::invalid_argument("Invalid vertex count");
      }
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [vertexCount](double acc, const Polygon& p)
        {
          return acc + (details::hasVertexCount(p, vertexCount) ? getPolygonArea(p) : 0.0);
        });
      out << sum;
    }
  }

  void printMax(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("No polygons available");
    }
    std::string subcmd;
    in >> subcmd;
    if (subcmd == "AREA")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(), details::areaLess);
      StreamGuard guard(out);
      out << std::fixed << std::setprecision(1) << getPolygonArea(*maxIt);
    }
    else if (subcmd == "VERTEXES")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(), details::vertexLess);
      out << maxIt->points.size();
    }
    else
    {
      throw std::invalid_argument("Invalid subcommand for MAX");
    }
  }

  void printMin(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("No polygons available");
    }
    std::string subcmd;
    in >> subcmd;
    if (subcmd == "AREA")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(), details::areaLess);
      StreamGuard guard(out);
      out << std::fixed << std::setprecision(1) << getPolygonArea(*minIt);
    }
    else if (subcmd == "VERTEXES")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(), details::vertexLess);
      out << minIt->points.size();
    }
    else
    {
      throw std::invalid_argument("Invalid subcommand for MIN");
    }
  }

  void printCount(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    std::string subcmd;
    in >> subcmd;
    if (subcmd == "EVEN")
    {
      out << std::count_if(polygons.begin(), polygons.end(), details::isEven);
    }
    else if (subcmd == "ODD")
    {
      out << std::count_if(polygons.begin(), polygons.end(), details::isOdd);
    }
    else
    {
      size_t vertexCount = std::stoul(subcmd);
      if (vertexCount < 3)
      {
        throw std::invalid_argument("Invalid vertex count");
      }
      out << std::count_if(polygons.begin(), polygons.end(),
        [vertexCount](const Polygon& p) { return details::hasVertexCount(p, vertexCount); });
    }
  }

  void printLessArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    Polygon ref;
    in >> ref;
    if (!in || in.peek() != '\n')
    {
      throw std::invalid_argument("Invalid reference polygon");
    }
    out << std::count_if(polygons.begin(), polygons.end(),
      [&ref](const Polygon& p) { return details::hasAreaLessThan(p, ref); });
  }

  void printRightShapes(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    out << std::count_if(polygons.begin(), polygons.end(), details::hasRightAngle);
  }
}
