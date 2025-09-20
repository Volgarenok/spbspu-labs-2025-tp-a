#include "commands.hpp"
#include "inframe.hpp"
#include "lessarea.hpp"
#include "utils.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>
#include <functional>

void guseynov::commands::handleAreaCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (param == "EVEN")
  {
    double sum = 0.0;
    for (const auto& poly : polygons)
    {
      if (poly.points.size() % 2 == 0)
      {
        sum += guseynov::utils::calculateArea(poly);
      }
    }
    std::cout << std::fixed << std::setprecision(1) << sum << '\n';
  }
  else if (param == "ODD")
  {
    double sum = 0.0;
    for (const auto& poly : polygons)
    {
      if (poly.points.size() % 2 != 0)
      {
        sum += guseynov::utils::calculateArea(poly);
      }
    }
    std::cout << std::fixed << std::setprecision(1) << sum << '\n';
  }
  else if (param == "MEAN")
  {
    if (polygons.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    double total = 0.0;
    for (const auto& poly : polygons)
    {
      total += guseynov::utils::calculateArea(poly);
    }
    std::cout << std::fixed << std::setprecision(1) << total / polygons.size() << '\n';
  }
  else if (guseynov::utils::isNumber(param))
  {
    size_t target = std::stoul(param);
    if (target < 3)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    double sum = 0.0;
    for (const auto& poly : polygons)
    {
      if (poly.points.size() == target)
      {
        sum += guseynov::utils::calculateArea(poly);
      }
    }
    std::cout << std::fixed << std::setprecision(1) << sum << '\n';
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void guseynov::commands::handleMaxCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  if (param == "AREA")
  {
    double maxArea = 0.0;
    for (const auto& poly : polygons)
    {
      double area = guseynov::utils::calculateArea(poly);
      if (area > maxArea)
      {
        maxArea = area;
      }
    }
    std::cout << std::fixed << std::setprecision(1) << maxArea << '\n';
  }
  else if (param == "VERTEXES")
  {
    size_t maxVertices = 0;
    for (const auto& poly : polygons)
    {
      if (poly.points.size() > maxVertices)
      {
        maxVertices = poly.points.size();
      }
    }
    std::cout << maxVertices << '\n';
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void guseynov::commands::handleMinCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  if (param == "AREA")
  {
    double minArea = std::numeric_limits<double>::max();
    for (const auto& poly : polygons)
    {
      double area = guseynov::utils::calculateArea(poly);
      if (area < minArea)
      {
        minArea = area;
      }
    }
    std::cout << std::fixed << std::setprecision(1) << minArea << '\n';
  }
  else if (param == "VERTEXES")
  {
    size_t minVertices = std::numeric_limits<size_t>::max();
    for (const auto& poly : polygons)
    {
      if (poly.points.size() < minVertices)
      {
        minVertices = poly.points.size();
      }
    }
    std::cout << minVertices << '\n';
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void guseynov::commands::handleCountCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (param == "EVEN")
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly) { return poly.points.size() % 2 == 0; });
    std::cout << count << '\n';
  }
  else if (param == "ODD")
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly) { return poly.points.size() % 2 != 0; });
    std::cout << count << '\n';
  }
  else if (guseynov::utils::isNumber(param))
  {
    size_t target = std::stoul(param);
    if (target < 3)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [target](const Polygon& poly) { return poly.points.size() == target; });
    std::cout << count << '\n';
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void guseynov::commands::handleInFrameCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<FALSE>\n";
    return;
  }

  Polygon targetPoly = guseynov::utils::parsePolygon(param);
  if (targetPoly.points.size() < 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  int minX = std::numeric_limits<int>::max();
  int maxX = std::numeric_limits<int>::min();
  int minY = std::numeric_limits<int>::max();
  int maxY = std::numeric_limits<int>::min();

  for (const auto& poly : polygons)
  {
    for (const auto& point : poly.points)
    {
      minX = std::min(minX, point.x);
      maxX = std::max(maxX, point.x);
      minY = std::min(minY, point.y);
      maxY = std::max(maxY, point.y);
    }
  }

  bool allPointsInside = true;
  for (const auto& point : targetPoly.points)
  {
    if (point.x < minX || point.x > maxX || point.y < minY || point.y > maxY)
    {
      allPointsInside = false;
      break;
    }
  }

  std::cout << (allPointsInside ? "<TRUE>" : "<FALSE>") << '\n';
}

void guseynov::commands::handleLessAreaCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  Polygon targetPoly = guseynov::utils::parsePolygon(param);
  if (targetPoly.points.size() < 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  double targetArea = guseynov::utils::calculateArea(targetPoly);

  size_t count = std::count_if(polygons.begin(), polygons.end(),
    [targetArea](const Polygon& poly) {
      return guseynov::utils::calculateArea(poly) < targetArea;
    });

  std::cout << count << '\n';
}
