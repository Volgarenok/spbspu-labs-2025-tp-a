#include "printCmd.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cctype>
#include <limits>
#include "streamGuard.hpp"

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

    bool checkRightAngle(const std::vector<Point>& points, size_t i = 0)
    {
      if (i >= points.size())
      {
        return false;
      }
      size_t n = points.size();
      size_t prevIdx = (i + n - 1) % n;
      size_t nextIdx = (i + 1) % n;

      const Point vec1 = points[prevIdx] - points[i];
      const Point vec2 = points[nextIdx] - points[i];

      if (dot(vec1, vec2) == 0)
      {
        return true;
      }
      return checkRightAngle(points, i + 1);
    }

    bool hasRightAngle(const Polygon& poly)
    {
      if (poly.points.size() < 3)
      {
        return false;
      }
      return checkRightAngle(poly.points);
    }

    double sumAreaIfRecursive(const std::vector< Polygon >& polygons, size_t index, bool (*condition)(const Polygon&))
    {
      if (index >= polygons.size())
      {
        return 0.0;
      }
      double acc = condition(polygons[index]) ? getPolygonArea(polygons[index]) : 0.0;
      return acc + sumAreaIfRecursive(polygons, index + 1, condition);
    }

    double sumTotalAreaRecursive(const std::vector< Polygon >& polygons, size_t index)
    {
      if (index >= polygons.size())
      {
        return 0.0;
      }
      return getPolygonArea(polygons[index]) + sumTotalAreaRecursive(polygons, index + 1);
    }

    double sumVertexCountAreaRecursive(const std::vector< Polygon >& polygons, size_t index, size_t vertexCount)
    {
      if (index >= polygons.size())
      {
        return 0.0;
      }
      double acc = hasVertexCount(polygons[index], vertexCount) ? getPolygonArea(polygons[index]) : 0.0;
      return acc + sumVertexCountAreaRecursive(polygons, index + 1, vertexCount);
    }

    size_t countIfRecursive(const std::vector< Polygon >& polygons, size_t index, bool (*condition)(const Polygon&))
    {
      if (index >= polygons.size())
      {
        return 0;
      }
      size_t acc = condition(polygons[index]) ? 1 : 0;
      return acc + countIfRecursive(polygons, index + 1, condition);
    }

    size_t countVertexRecursive(const std::vector< Polygon >& polygons, size_t index, size_t vertexCount)
    {
      if (index >= polygons.size())
      {
        return 0;
      }
      size_t acc = hasVertexCount(polygons[index], vertexCount) ? 1 : 0;
      return acc + countVertexRecursive(polygons, index + 1, vertexCount);
    }

    size_t countLessAreaRecursive(const std::vector< Polygon >& polygons, size_t index, const Polygon& ref)
    {
      if (index >= polygons.size())
      {
        return 0;
      }
      size_t acc = hasAreaLessThan(polygons[index], ref) ? 1 : 0;
      return acc + countLessAreaRecursive(polygons, index + 1, ref);
    }

    const Polygon& maxAreaRecursive(const std::vector< Polygon >& polygons, size_t index, const Polygon& currentMax)
    {
      if (index >= polygons.size())
      {
        return currentMax;
      }
      const Polygon& newMax = areaLess(currentMax, polygons[index]) ? polygons[index] : currentMax;
      return maxAreaRecursive(polygons, index + 1, newMax);
    }

    const Polygon& minAreaRecursive(const std::vector< Polygon >& polygons, size_t index, const Polygon& currentMin)
    {
      if (index >= polygons.size())
      {
        return currentMin;
      }
      const Polygon& newMin = areaLess(polygons[index], currentMin) ? polygons[index] : currentMin;
      return minAreaRecursive(polygons, index + 1, newMin);
    }

    const Polygon& maxVertexRecursive(const std::vector< Polygon >& polygons, size_t index, const Polygon& currentMax)
    {
      if (index >= polygons.size())
      {
        return currentMax;
      }
      const Polygon& newMax = vertexLess(currentMax, polygons[index]) ? polygons[index] : currentMax;
      return maxVertexRecursive(polygons, index + 1, newMax);
    }

    const Polygon& minVertexRecursive(const std::vector< Polygon >& polygons, size_t index, const Polygon& currentMin)
    {
      if (index >= polygons.size())
      {
        return currentMin;
      }
      const Polygon& newMin = vertexLess(polygons[index], currentMin) ? polygons[index] : currentMin;
      return minVertexRecursive(polygons, index + 1, newMin);
    }

    bool hasNonSpace(const std::string& remaining, size_t index = 0)
    {
      if (index >= remaining.size())
      {
        return false;
      }
      if (!std::isspace(remaining[index]))
      {
        return true;
      }
      return hasNonSpace(remaining, index + 1);
    }
  }

  void printArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    std::string subcmd;
    in >> subcmd;
    shramko::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);
    if (subcmd == "EVEN")
    {
      out << details::sumAreaIfRecursive(polygons, 0, &details::isEven);
    }
    else if (subcmd == "ODD")
    {
      out << details::sumAreaIfRecursive(polygons, 0, &details::isOdd);
    }
    else if (subcmd == "MEAN")
    {
      if (polygons.empty())
      {
        throw std::invalid_argument("No polygons available");
      }
      out << details::sumTotalAreaRecursive(polygons, 0) / polygons.size();
    }
    else
    {
      try
      {
        size_t vertexCount = std::stoul(subcmd);
        if (vertexCount < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
        out << details::sumVertexCountAreaRecursive(polygons, 0, vertexCount);
      }
      catch (const std::exception&)
      {
        throw std::invalid_argument("Invalid subcommand for AREA");
      }
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
      shramko::StreamGuard guard(out);
      out << std::fixed << std::setprecision(1) << getPolygonArea(details::maxAreaRecursive(polygons, 1, polygons[0]));
    }
    else if (subcmd == "VERTEXES")
    {
      out << details::maxVertexRecursive(polygons, 1, polygons[0]).points.size();
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
      shramko::StreamGuard guard(out);
      out << std::fixed << std::setprecision(1) << getPolygonArea(details::minAreaRecursive(polygons, 1, polygons[0]));
    }
    else if (subcmd == "VERTEXES")
    {
      out << details::minVertexRecursive(polygons, 1, polygons[0]).points.size();
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
      out << details::countIfRecursive(polygons, 0, &details::isEven);
    }
    else if (subcmd == "ODD")
    {
      out << details::countIfRecursive(polygons, 0, &details::isOdd);
    }
    else
    {
      try
      {
        size_t vertexCount = std::stoul(subcmd);
        if (vertexCount < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
        out << details::countVertexRecursive(polygons, 0, vertexCount);
      }
      catch (const std::exception&)
      {
        throw std::invalid_argument("Invalid subcommand for COUNT");
      }
    }
  }

  void printLessArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    Polygon ref;
    if (!(in >> ref))
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      throw std::invalid_argument("Invalid reference polygon");
    }
    std::string remaining;
    std::getline(in, remaining);
    if (!remaining.empty())
    {
      if (details::hasNonSpace(remaining))
      {
        throw std::invalid_argument("Invalid reference polygon");
      }
    }
    out << details::countLessAreaRecursive(polygons, 0, ref);
  }

  void printRightShapes(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    out << details::countIfRecursive(polygons, 0, &details::hasRightAngle);
  }
}
