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

    if (subcmd == "EVEN")
    {
      std::vector<Polygon> filteredPolygons;
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), IsEven{});

      std::vector< double > areas;
      std::transform(filteredPolygons.begin(), filteredPolygons.end(), std::back_inserter(areas), GetPolygonArea{});
      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      out << total;
    }
    else if (subcmd == "ODD")
    {
      std::vector< Polygon > filteredPolygons;
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), IsOdd{});

      std::vector< double > areas;
      std::transform(filteredPolygons.begin(), filteredPolygons.end(), std::back_inserter(areas), GetPolygonArea{});
      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      out << total;
    }
    else if (subcmd == "MEAN")
    {
      if (polygons.empty())
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }

      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), GetPolygonArea{});
      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      out << total / areas.size();
    }
    else if (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
    {
      size_t num = std::stoul(subcmd);
      if (num < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }

      struct HasSize
      {
        size_t size;
        bool operator()(const Polygon& p) const { return p.points.size() == size; }
      };

      std::vector< Polygon > filteredPolygons;
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), HasSize{num});

      std::vector< double > areas;
      std::transform(filteredPolygons.begin(), filteredPolygons.end(), std::back_inserter(areas), GetPolygonArea{});
      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      out << total;
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::string subcmd;
    in >> subcmd;

    if (subcmd == "AREA")
    {
      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), GetPolygonArea{});
      double maxArea = *std::max_element(areas.begin(), areas.end());
      FormatGuard guard(out);
      out << std::fixed << std::setprecision(1) << maxArea;
    }
    else if (subcmd == "VERTEXES")
    {
      std::vector< size_t > vertexCounts;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(vertexCounts), GetPolygonVertexCount{});
      size_t maxVertexes = *std::max_element(vertexCounts.begin(), vertexCounts.end());
      out << maxVertexes;
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::string subcmd;
    in >> subcmd;

    if (subcmd == "AREA")
    {
      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), GetPolygonArea{});
      double minArea = *std::min_element(areas.begin(), areas.end());
      FormatGuard guard(out);
      out << std::fixed << std::setprecision(1) << minArea;
    }
    else if (subcmd == "VERTEXES")
    {
      std::vector< size_t > vertexCounts;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(vertexCounts), GetPolygonVertexCount{});
      size_t minVertexes = *std::min_element(vertexCounts.begin(), vertexCounts.end());
      out << minVertexes;
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
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

  void lessarea(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    struct HasDuplicates
    {
      bool operator()(const Polygon& p) const
      {
        for (size_t i = 0; i < p.points.size(); ++i)
        {
          for (size_t j = i + 1; j < p.points.size(); ++j)
          {
            if (p.points[i] == p.points[j])
            {
              return true;
            }
          }
        }
          return false;
      }
    };

    if (HasDuplicates{}(target))
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
    in >> target;

    if (!in || target.points.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    struct HasDuplicates
    {
      bool operator()(const Polygon& p) const
      {
        for (size_t i = 0; i < p.points.size(); ++i)
        {
          for (size_t j = i + 1; j < p.points.size(); ++j)
          {
            if (p.points[i] == p.points[j])
            {
              return true;
            }
          }
        }
        return false;
      }
    };

    if (HasDuplicates{}(target))
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
