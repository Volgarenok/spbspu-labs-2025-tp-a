#include "functions.h"
#include "geometry.h"
#include <iomanip>
#include <stdexcept>
#include <iostream>

namespace
{
  bool isPolygonValid(const asafov::Polygon& poly)
  {
    if (poly.points.size() < 3)
    {
      return false;
    }

    for (size_t i = 0; i < poly.points.size(); ++i)
    {
      for (size_t j = i + 1; j < poly.points.size(); ++j)
      {
        if (poly.points[i] == poly.points[j])
        {
          return false;
        }
      }
    }

    return true;
}
}

namespace asafov
{
  void processCommand(const std::vector< Polygon >& polygons, const std::string& cmd)
  {
    auto printArea = [](double area) ->void
    {
      std::cout << std::fixed << std::setprecision(1) << area << '\n';
    };

    auto printCount = [](size_t count) ->void
    {
      std::cout << count << '\n';
    };

    try
    {
      if (cmd == "AREA MEAN")
      {
        if (polygons.empty())
        {
          throw std::invalid_argument("No polygons");
        }
        double total = 0.0;
        size_t valid_count = 0;
        for (const auto& poly: polygons)
        {
          if (isPolygonValid(poly))
          {
            total += computeArea(poly);
            valid_count++;
          }
        }
        if (valid_count == 0)
        {
          throw std::invalid_argument("No valid polygons");
        }
        printArea(total / static_cast< double >(valid_count));
      }
      else if (cmd == "MAX AREA")
      {
        if (polygons.empty())
        {
          throw std::invalid_argument("No polygons");
        }
        bool found = false;
        double max_area = 0;
        for (const auto& poly: polygons)
        {
          if (isPolygonValid(poly))
          {
            double area = computeArea(poly);
            if (!found || area > max_area)
            {
              max_area = area;
              found = true;
            }
          }
        }
        if (!found)
        {
          throw std::invalid_argument("No valid polygons");
        }
        printArea(max_area);
      }
      else if (cmd == "MAX VERTEXES")
      {
        if (polygons.empty())
        {
          throw std::invalid_argument("No polygons");
        }
        bool found = false;
        size_t max_vert = 0;
        for (const auto& poly: polygons)
        {
          if (isPolygonValid(poly))
          {
            if (!found || poly.points.size() > max_vert)
            {
              max_vert = poly.points.size();
              found = true;
            }
          }
        }
        if (!found)
        {
          throw std::invalid_argument("No valid polygons");
        }
        printCount(max_vert);
      }
      else if (cmd == "MIN AREA")
      {
        if (polygons.empty())
        {
          throw std::invalid_argument("No polygons");
        }
        bool found = false;
        double min_area = 0;
        for (const auto& poly: polygons)
        {
          if (isPolygonValid(poly))
          {
            double area = computeArea(poly);
            if (!found || area < min_area)
            {
              min_area = area;
              found = true;
            }
          }
        }
        if (!found)
        {
          throw std::invalid_argument("No valid polygons");
        }
        printArea(min_area);
      }
      else if (cmd == "MIN VERTEXES")
      {
        if (polygons.empty())
        {
          throw std::invalid_argument("No polygons");
        }
        bool found = false;
        size_t min_vert = 0;
        for (const auto& poly: polygons)
        {
          if (isPolygonValid(poly))
          {
            if (!found || poly.points.size() < min_vert)
            {
              min_vert = poly.points.size();
              found = true;
            }
          }
        }
        if (!found)
        {
          throw std::invalid_argument("No valid polygons");
        }
        printCount(min_vert);
      }
      else if (cmd == "AREA EVEN")
      {
        double sum = 0.0;
        for (const auto& poly: polygons)
        {
          if (isPolygonValid(poly) && poly.points.size() % 2 == 0)
          {
            sum += computeArea(poly);
          }
        }
        printArea(sum);
      }
      else if (cmd == "AREA ODD")
      {
        double sum = 0.0;
        for (const auto& poly: polygons)
        {
          if (isPolygonValid(poly) && poly.points.size() % 2 != 0)
          {
            sum += computeArea(poly);
          }
        }
        printArea(sum);
      }
      else if (cmd.find("AREA ") == 0)
      {
        std::string arg = cmd.substr(5);
        try
        {
          size_t num = std::stoul(arg);
          if (num < 3)
          {
            throw std::invalid_argument("Invalid vertex count");
          }
          double sum = 0.0;
          for (const auto& poly: polygons)
          {
            if (isPolygonValid(poly) && poly.points.size() == num)
            {
              sum += computeArea(poly);
            }
          }
          printArea(sum);
        }
        catch (...)
        {
          throw std::invalid_argument("Invalid AREA argument");
        }
      }
      else if (cmd == "COUNT EVEN")
      {
        size_t count = 0;
        for (const auto& poly: polygons)
        {
          if (isPolygonValid(poly) && poly.points.size() % 2 == 0)
          {
            count++;
          }
        }
        printCount(count);
      }
      else if (cmd == "COUNT ODD")
      {
        size_t count = 0;
        for (const auto& poly: polygons)
        {
          if (isPolygonValid(poly) && poly.points.size() % 2 != 0)
          {
            count++;
          }
        }
        printCount(count);
      }
      else if (cmd.find("COUNT ") == 0)
      {
        std::string arg = cmd.substr(6);
        try
        {
          size_t num = std::stoul(arg);
          if (num < 3)
          {
            throw std::invalid_argument("Invalid vertex count");
          }
          size_t count = 0;
          for (const auto& poly: polygons)
          {
            if (isPolygonValid(poly) && poly.points.size() == num)
            {
              count++;
            }
          }
          printCount(count);
        }
        catch (...)
        {
          throw std::invalid_argument("Invalid COUNT argument");
        }
      }
      else
      {
        throw std::invalid_argument("Unknown command");
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
