#include "lessarea.hpp"
#include "utils.hpp"
#include <iostream>
#include <algorithm>

void guseynov::commands::handleLessAreaCommand(const std::vector< Polygon >& polygons, const std::string& param)
{
  Polygon targetPoly = guseynov::utils::parsePolygon(param);
  if (targetPoly.points.size() < 3)
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  double targetArea = guseynov::utils::calculateArea(targetPoly);
  size_t count = std::count_if(polygons.begin(), polygons.end(),
    [targetArea](const Polygon& poly) {
      return guseynov::utils::calculateArea(poly) < targetArea;
    });

  std::cout << count << std::endl;
}
