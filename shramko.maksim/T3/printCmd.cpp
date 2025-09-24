#include "printCmd.hpp"
#include <map>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
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

    LocalTriangle makeLocalTriangle(const std::vector< Point >& points, size_t index)
    {
      size_t n = points.size();
      size_t prevIdx = (index + n - 1) % n;
      size_t nextIdx = (index + 1) % n;
      return { points[prevIdx], points[index], points[nextIdx] };
    }

    bool isRightAngle(const LocalTriangle& tri)
    {
      Point vec1 = tri.prev - tri.curr;
      Point vec2 = tri.next - tri.curr;
      return dot(vec1, vec2) == 0;
    }

    struct CheckRightAngle
    {
      const std::vector< Point >& points;
      CheckRightAngle(const std::vector< Point >& p):
        points(p)
      {}

      bool operator()(size_t index) const
      {
        LocalTriangle tri = makeLocalTriangle(points, index);
        return isRightAngle(tri);
      }
    };

    bool hasRightAngle(const Polygon& poly)
    {
      if (poly.points.size() < 3)
      {
        return false;
      }
      std::vector< size_t > indices(poly.points.size());
      std::iota(indices.begin(), indices.end(), 0UL);
      return std::any_of(indices.begin(), indices.end(), CheckRightAngle(poly.points));
    }

    double sumAreaIf(double acc, const Polygon& p, bool (*condition)(const Polygon&))
    {
      return acc + (condition(p) ? getPolygonArea(p) : 0.0);
    }

    double sumTotalArea(double acc, const Polygon& p)
    {
      return acc + getPolygonArea(p);
    }

    double sumVertexCountArea(double acc, const Polygon& p, size_t vertexCount)
    {
      return acc + (hasVertexCount(p, vertexCount) ? getPolygonArea(p) : 0.0);
    }
  }

  void printArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    std::string subcmd;
    in >> subcmd;
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);
    if (subcmd == "EVEN" || subcmd == "ODD")
    {
      double sum = 0.0;
      if (!polygons.empty())
      {
        if (subcmd == "EVEN")
        {
          sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            std::bind(details::sumAreaIf, std::placeholders::_1, std::placeholders::_2, &details::isEven));
        }
        else if (subcmd == "ODD")
        {
          sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            std::bind(details::sumAreaIf, std::placeholders::_1, std::placeholders::_2, &details::isOdd));
        }
      }
      out << sum;
    }
    else if (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
    {
      size_t vertexCount = std::stoul(subcmd);
      if (vertexCount < 3)
      {
        throw std::invalid_argument("Invalid vertex count");
      }
      double sum = 0.0;
      if (!polygons.empty())
      {
        sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
          std::bind(details::sumVertexCountArea, std::placeholders::_1, std::placeholders::_2, vertexCount));
      }
      out << sum;
    }
    else if (subcmd == "MEAN")
    {
      if (polygons.empty())
      {
        throw std::invalid_argument("No polygons available");
      }
      double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, details::sumTotalArea);
      out << total / polygons.size();
    }
    else
    {
      throw std::invalid_argument("Invalid subcommand for AREA");
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
      auto maxIt = std::max_element(polygons.begin(), polygons.end(),
        std::bind(&details::areaLess, std::placeholders::_1, std::placeholders::_2));
      StreamGuard guard(out);
      out << std::fixed << std::setprecision(1) << getPolygonArea(*maxIt);
    }
    else if (subcmd == "VERTEXES")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(),
        std::bind(&details::vertexLess, std::placeholders::_1, std::placeholders::_2));
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
      auto minIt = std::min_element(polygons.begin(), polygons.end(),
        std::bind(&details::areaLess, std::placeholders::_1, std::placeholders::_2));
      StreamGuard guard(out);
      out << std::fixed << std::setprecision(1) << getPolygonArea(*minIt);
    }
    else if (subcmd == "VERTEXES")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(),
        std::bind(&details::vertexLess, std::placeholders::_1, std::placeholders::_2));
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
      out << std::count_if(polygons.begin(), polygons.end(),
        std::bind(&details::isEven, std::placeholders::_1));
    }
    else if (subcmd == "ODD")
    {
      out << std::count_if(polygons.begin(), polygons.end(),
        std::bind(&details::isOdd, std::placeholders::_1));
    }
    else
    {
      size_t vertexCount = std::stoul(subcmd);
      if (vertexCount < 3)
      {
        throw std::invalid_argument("Invalid vertex count");
      }
      out << std::count_if(polygons.begin(), polygons.end(),
        std::bind(&details::hasVertexCount, std::placeholders::_1, vertexCount));
    }
  }

  void printLessArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      throw std::invalid_argument("Invalid input stream");
    }
    std::string line;
    std::getline(in, line);
    std::istringstream iss(line);
    Polygon ref;
    if (!(iss >> ref))
    {
      throw std::invalid_argument("Invalid reference polygon");
    }
    std::string extra;
    if (iss >> extra)
    {
      throw std::invalid_argument("Invalid reference polygon");
    }
    out << std::count_if(polygons.begin(), polygons.end(),
      std::bind(&details::hasAreaLessThan, std::placeholders::_1, std::cref(ref)));
  }

  void printRightShapes(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    out << std::count_if(polygons.begin(), polygons.end(), details::hasRightAngle);
  }
}
