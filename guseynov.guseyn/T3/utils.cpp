#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>

bool guseynov::utils::isNumber(const std::string& s)
{
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

double guseynov::utils::calculateArea(const Polygon& poly)
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

guseynov::Polygon guseynov::utils::parsePolygon(const std::string& line)
{
  Polygon poly;
  std::istringstream iss(line);
  size_t numVertices;
  if (iss >> numVertices)
  {
    for (size_t i = 0; i < numVertices; ++i)
    {
      std::string pointStr;
      iss >> pointStr;
      if (pointStr.size() >= 5 && pointStr[0] == '(' && pointStr.back() == ')')
      {
        size_t semicolon = pointStr.find(';');
        if (semicolon != std::string::npos)
        {
          try
          {
            int x = std::stoi(pointStr.substr(1, semicolon - 1));
            int y = std::stoi(pointStr.substr(semicolon + 1, pointStr.size() - semicolon - 2));
            poly.points.push_back({x, y});
          }
          catch (...)
          {
            break;
          }
        }
      }
    }
  }
  return poly;
}

std::vector< guseynov::Polygon > guseynov::utils::readPolygonsFromFile(const std::string& filename)
{
  std::vector< Polygon > polygons;
  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line))
  {
    Polygon poly = parsePolygon(line);
    if (!poly.points.empty() && poly.points.size() >= 3)
    {
      polygons.push_back(poly);
    }
  }
  return polygons;
}
