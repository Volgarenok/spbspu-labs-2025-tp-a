#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <stdexcept>

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
  if (!(iss >> numVertices))
  {
    return poly;
  }
  if (numVertices < 3)
  {
    return poly;
  }

  for (size_t i = 0; i < numVertices; ++i)
  {
    std::string pointStr;
    if (!(iss >> pointStr))
    {
      poly.points.clear();
      return poly;
    }
    if (pointStr.size() < 5 || pointStr[0] != '(' || pointStr.back() != ')')
    {
      poly.points.clear();
      return poly;
    }
    size_t semicolon = pointStr.find(';');
    if (semicolon == std::string::npos || semicolon < 2 || semicolon > pointStr.size() - 2)
    {
      poly.points.clear();
      return poly;
    }
    try
    {
      std::string xStr = pointStr.substr(1, semicolon - 1);
      std::string yStr = pointStr.substr(semicolon + 1, pointStr.size() - semicolon - 2);
      int x = std::stoi(xStr);
      int y = std::stoi(yStr);
      poly.points.push_back({x, y});
    }
    catch (const std::exception&)
    {
      poly.points.clear();
      return poly;
    }
  }
  return poly;
}

std::vector< guseynov::Polygon > guseynov::utils::readPolygonsFromFile(const std::string& filename)
{
  std::vector< Polygon > polygons;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file: " + filename);
  }
  std::string line;
  size_t lineNumber = 0;
  while (std::getline(file, line))
  {
    lineNumber++;
    if (line.empty()) continue;
    line.erase(0, line.find_first_not_of(" \t"));
    line.erase(line.find_last_not_of(" \t") + 1);
    if (line.empty()) continue;
    Polygon poly = parsePolygon(line);
    if (poly.points.size() >= 3)
    {
      polygons.push_back(poly);
    }
  }
  return polygons;
}
