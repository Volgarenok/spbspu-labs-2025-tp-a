#include <algorithm>
#include <iomanip>
#include <string>
#include <cmath>
#include <sstream>
#include "streamGuard.hpp"
#include "commands.hpp"

namespace ivanova
{
  double calculateArea(const Polygon& poly)
  {
    double area = 0.0;
    size_t n = poly.points.size();
    for (size_t i = 0; i < n; ++i)
    {
      size_t j = (i + 1) % n;
      area += poly.points[i].x * poly.points[j].y;
      area -= poly.points[j].x * poly.points[i].y;
    }
    return std::abs(area) / 2.0;
  }

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
      auto minIt = std::min_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b)
        {
          return calculateArea(a) < calculateArea(b);
        }
      );
      out << calculateArea(*minIt) << '\n';
    }
    else if (param == "VERTEXES")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b)
        {
          return a.points.size() < b.points.size();
        }
      );
      out << minIt->points.size() << '\n';
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
    std::string line;
    std::getline(in, line);

    if (line.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::istringstream iss(line);
    Polygon target;
    iss >> target;

    if (!iss || target.points.size() < 3 || !iss.eof())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    size_t count = 0;
    std::vector< Polygon > result;

    for (const auto& poly : polygons)
    {
      result.push_back(poly);
      if (poly == target)
      {
        result.push_back(target);
        count++;
      }
    }

    polygons = std::move(result);
    out << count << '\n';
  }

  double distance(const Point& a, const Point& b)
  {
    return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
  }

  bool areCompatible(const Polygon& a, const Polygon& b)
  {
    if (a.points.size() != b.points.size())
    {
      return false;
    }

    size_t n = a.points.size();

    for (size_t shift = 0; shift < n; ++shift)
    {
      bool compatible = true;

      for (size_t i = 0; i < n && compatible; ++i)
      {
        size_t j = (i + 1) % n;
        size_t i_shift = (i + shift) % n;
        size_t j_shift = (j + shift) % n;

        double dist_a = distance(a.points[i], a.points[j]);
        double dist_b = distance(b.points[i_shift], b.points[j_shift]);

        if (std::abs(dist_a - dist_b) > 1e-6)
        {
          compatible = false;
        }
      }

      if (compatible)
      {
        return true;
      }
    }

    return false;
  }

  void same(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string line;
    std::getline(in, line);

    if (line.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::istringstream iss(line);
    Polygon target;
    iss >> target;

    if (!iss || target.points.size() < 3 || !iss.eof())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

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
