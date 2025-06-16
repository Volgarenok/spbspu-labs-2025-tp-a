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
}