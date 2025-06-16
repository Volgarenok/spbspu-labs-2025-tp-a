#include <algorithm>
#include <iomanip>
#include <string>
#include "streamGuard.hpp"
#include "commands.hpp"

namespace ivanova
{
  void area(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
  {
    std::string param;
    in >> param;

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "EVEN" || param == "ODD")
    {
      bool isEven = (param == "EVEN");
      double sum = 0.0;
      for (const auto& poly : polygons)
      {
        if ((poly.points.size() % 2 == 0) == isEven)
        {
          sum += calculateArea(poly);
        }
      }
      out << sum << '\n';
    }
    else if (param == "MEAN")
    {
      if (polygons.empty())
      {
        throw std::invalid_argument("At least one polygon required");
      }
      double sum = 0.0;
      for (const auto& poly : polygons)
      {
        sum += calculateArea(poly);
      }
      out << (sum / polygons.size()) << '\n';
    }
    else
    {
      size_t numVertices = std::stoul(param);
      double sum = 0.0;
      for (const auto& poly : polygons)
      {
        if (poly.points.size() == numVertices)
        {
          sum += calculateArea(poly);
        }
      }
      out << sum << '\n';
    }
  }

  void max(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("At least one polygon required");
    }

    std::string param;
    in >> param;

    if (param == "AREA")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(),
              [](const Polygon& a, const Polygon& b){ return calculateArea(a) < calculateArea(b);} );
      out << std::fixed << std::setprecision(1) << calculateArea(*maxIt) << '\n';
    }
    else if (param == "VERTEXES")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(),
              [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size();} );
      out << maxIt->points.size() << '\n';
    }
    else
    {
      throw std::invalid_argument("Invalid max parameter");
    }
  }

  void min(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("At least one polygon required");
    }
     
    std::string param;
    in >> param;

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "AREA")
    {
      auto minElem = std::min_element(polygons.begin(), polygons.end(),
               [](const Polygon& a, const Polygon& b) { return calculateArea(a) < calculateArea(b);} );
      out << calculateArea(*minElem) << '\n';
    }
    else if (param == "VERTEXES")
    {
      auto minElem = std::min_element(polygons.begin(), polygons.end(),
               [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size();} );
      out << minElem->points.size() << '\n';
    }
    else
    {
      throw std::invalid_argument("Invalid MIN parameter: " + param);
    }
  }
}