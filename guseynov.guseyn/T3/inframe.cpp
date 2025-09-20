#include "inframe.hpp"
#include "utils.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

void guseynov::commands::handleInFrameCommand(const std::vector< Polygon >& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<FALSE>" << std::endl;
    return;
  }

  Polygon targetPoly = guseynov::utils::parsePolygon(param);
  if (targetPoly.points.empty())
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
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

  bool inside = true;
  for (const auto& point : targetPoly.points)
  {
    if (point.x < minX || point.x > maxX || point.y < minY || point.y > maxY)
    {
      inside = false;
      break;
    }
  }

  std::cout << (inside ? "<TRUE>" : "<FALSE>") << std::endl;
}
