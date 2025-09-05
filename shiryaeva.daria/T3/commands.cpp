#include "commands.hpp"
#include "functors.hpp"
#include "format_guard.hpp"
#include <algorithm>
#include <iomanip>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <string>
#include <istream>
#include <ostream>

namespace shiryaeva
{
  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string subcmd;
    in >> subcmd;

    FormatGuard guard(out);
    out << std::fixed << std::setprecision(1);

    std::vector< Polygon > filteredPolygons;
    bool needMeanCalculation = false;

    if (subcmd == "EVEN")
    {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), IsEven{});
    }
    else if (subcmd == "ODD")
    {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), IsOdd{});
    }
    else if (subcmd == "MEAN")
    {
      if (polygons.empty())
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }

      filteredPolygons = polygons;
      needMeanCalculation = true;
    }
    else if (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
    {
      size_t num = std::stoul(subcmd);
      if (num < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }

      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), HasVertexCount{num});

    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::vector<double> areas;
    std::transform(filteredPolygons.begin(), filteredPolygons.end(), std::back_inserter(areas), GetPolygonArea{});
    double total = std::accumulate(areas.begin(), areas.end(), 0.0);

    if (needMeanCalculation)
    {
        out << total / areas.size();
    }
    else
    {
        out << total;
    }
  }

  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    FormatGuard guard(out);
    std::string subcmd;
    in >> subcmd;

    out << std::fixed << std::setprecision(1);

    if (polygons.empty())
    {
      out << "<INVALID COMMAND>";
      return;
    }

    if (subcmd == "AREA")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(), CompareByArea());
      out << getArea(*maxIt);
    }
    else if (subcmd == "VERTEXES")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(), CompareByVertexCount());
      out << maxIt->points.size();
    }
    else
    {
      out << "<INVALID COMMAND>";
    }
  }

  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    FormatGuard guard(out);
    std::string subcmd;
    in >> subcmd;

    out << std::fixed << std::setprecision(1);

    if (polygons.empty())
    {
      out << "<INVALID COMMAND>";
      return;
    }

    if (subcmd == "AREA")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(), CompareByArea());
      out << getArea(*minIt);
    }
    else if (subcmd == "VERTEXES")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(), CompareByVertexCount());
      out << minIt->points.size();
    }
    else
    {
      out << "<INVALID COMMAND>";
    }
  }

  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string subcmd;
    in >> subcmd;

    if (subcmd == "EVEN")
    {
      out << std::count_if(polygons.begin(), polygons.end(), IsEven{});
    }
    else if (subcmd == "ODD")
    {
      out << std::count_if(polygons.begin(), polygons.end(), IsOdd{});
    }
    else if (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
    {
      size_t num = std::stoul(subcmd);
      if (num < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      out << std::count_if(polygons.begin(), polygons.end(), HasVertexCount{num});
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void lessArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3 || (in.peek() != '\n' && !in.eof()))
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    double targetArea = getArea(target);

    struct LessArea
    {
      double area;
      bool operator()(const Polygon& p) const
      {
        return getArea(p) < area;
      }
    };

    out << std::count_if(polygons.begin(), polygons.end(), LessArea{targetArea});
  }

  void intersections(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    struct IntersectsWith
    {
      Polygon polygon;
      bool operator()(const Polygon& other) const
      {
        auto left = std::minmax_element(polygon.points.begin(), polygon.points.end());
        auto right = std::minmax_element(other.points.begin(), other.points.end());
        return !((*left.second < *right.first) || (*right.second < *left.first));
      }
    };

    out << std::count_if(polygons.begin(), polygons.end(), IntersectsWith{target});
  }
}
