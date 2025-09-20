#include "commands.hpp"
#include "utils.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>

void guseynov::commands::handleAreaCommand(const std::vector< Polygon >& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

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
    std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
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
    std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
  }
  else if (param == "MEAN")
  {
    double total = 0.0;
    for (const auto& poly : polygons)
    {
      total += guseynov::utils::calculateArea(poly);
    }
    std::cout << std::fixed << std::setprecision(1) << total / polygons.size() << std::endl;
  }
  else if (guseynov::utils::isNumber(param))
  {
    size_t target = std::stoul(param);
    double sum = 0.0;
    for (const auto& poly : polygons)
    {
      if (poly.points.size() == target)
      {
        sum += guseynov::utils::calculateArea(poly);
      }
    }
    std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
  }
  else
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
  }
}

void guseynov::commands::handleMaxCommand(const std::vector< Polygon >& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  if (param == "AREA")
  {
    double maxArea = 0.0;
    for (const auto& poly : polygons)
    {
      double area = guseynov::utils::calculateArea(poly);
      if (area > maxArea) maxArea = area;
    }
    std::cout << std::fixed << std::setprecision(1) << maxArea << std::endl;
  }
  else if (param == "VERTEXES")
  {
    size_t maxVertices = 0;
    for (const auto& poly : polygons)
    {
      if (poly.points.size() > maxVertices) maxVertices = poly.points.size();
    }
    std::cout << maxVertices << std::endl;
  }
  else
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
  }
}

void guseynov::commands::handleMinCommand(const std::vector< Polygon >& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  if (param == "AREA")
  {
    double minArea = std::numeric_limits<double>::max();
    for (const auto& poly : polygons)
    {
      double area = guseynov::utils::calculateArea(poly);
      if (area < minArea) minArea = area;
    }
    std::cout << std::fixed << std::setprecision(1) << minArea << std::endl;
  }
  else if (param == "VERTEXES")
  {
    size_t minVertices = std::numeric_limits<size_t>::max();
    for (const auto& poly : polygons)
    {
      if (poly.points.size() < minVertices) minVertices = poly.points.size();
    }
    std::cout << minVertices << std::endl;
  }
  else
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
  }
}

void guseynov::commands::handleCountCommand(const std::vector< Polygon >& polygons, const std::string& param)
{
  if (param == "EVEN")
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly) { return poly.points.size() % 2 == 0; });
    std::cout << count << std::endl;
  }
  else if (param == "ODD")
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly) { return poly.points.size() % 2 != 0; });
    std::cout << count << std::endl;
  }
  else if (guseynov::utils::isNumber(param))
  {
    size_t target = std::stoul(param);
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [target](const Polygon& poly) { return poly.points.size() == target; });
    std::cout << count << std::endl;
  }
  else
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
  }
}
