#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>
#include <stdexcept>
#include "streamGuard.hpp"
#include "commands.hpp"

namespace ivanova
{
  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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
        throw std::invalid_argument("<INVALID COMMAND>");
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
      try
      {
        size_t numVertices = std::stoul(param);
        if (numVertices < 3)
        {
          throw std::invalid_argument("<INVALID COMMAND>");
        }
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
      catch (const std::invalid_argument&)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
    }
  }

  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string param;
    in >> param;

    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    StreamGuard guard(out);

    if (param == "AREA")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b)
        {
          return calculateArea(a) < calculateArea(b);
        }
      );
      out << std::fixed << std::setprecision(1) << calculateArea(*maxIt) << '\n';
    }
    else if (param == "VERTEXES")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b)
        {
          return a.points.size() < b.points.size();
        }
      );
      out << maxIt->points.size() << '\n';
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string param;
    in >> param;

    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "AREA")
    {
      auto minElem = std::min_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b)
        {
          return calculateArea(a) < calculateArea(b);
        }
      );
      out << calculateArea(*minElem) << '\n';
    }
    else if (param == "VERTEXES")
    {
      auto minElem = std::min_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b)
        {
          return a.points.size() < b.points.size();
        }
      );
      out << minElem->points.size() << '\n';
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string param;
    in >> param;

    auto isValidPolygon = [](const Polygon& poly)
    {
      return poly.points.size() >= 3;
    };

    if (param == "EVEN" || param == "ODD")
    {
      bool isEven = (param == "EVEN");
      size_t cnt = std::count_if(polygons.begin(), polygons.end(),
        [isEven, isValidPolygon](const Polygon& poly)
        {
          return isValidPolygon(poly) && (poly.points.size() % 2 == 0) == isEven;
        }
      );
      out << cnt << '\n';
    }
    else
    {
      try
      {
        size_t numVertices = std::stoul(param);
        if (numVertices < 3)
        {
          throw std::invalid_argument("<INVALID COMMAND>");
        }
        size_t cnt = std::count_if(polygons.begin(), polygons.end(),
          [numVertices, isValidPolygon](const Polygon& poly)
          {
            return isValidPolygon(poly) && poly.points.size() == numVertices;
          }
        );
        out << cnt << '\n';
      }
      catch (const std::invalid_argument&)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
    }
  }

  void echo(std::istream& in, std::ostream& out, std::vector< Polygon >& polygons)
  {
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    size_t count = 0;
    auto it = polygons.begin();

    while (it != polygons.end())
    {
      if (*it == target)
      {
        it = polygons.insert(it + 1, target);
        count++;
        it++;
      }
      else
      {
        it++;
      }
    }

    out << count << '\n';
  }

  void same(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    auto areCompatible = [](const Polygon& a, const Polygon& b)
    {
      if (a.points.size() != b.points.size())
      {
        return false;
      }
      return std::abs(calculateArea(a) - calculateArea(b)) < 1e-6;
    };

    size_t count = 0;
    for (const auto& poly : polygons)
    {
      if (areCompatible(poly, target))
      {
        count++;
      }
    }

    out << count << '\n';
  }
}
