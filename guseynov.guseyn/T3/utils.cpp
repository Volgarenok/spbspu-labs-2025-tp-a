#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <regex>

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
    poly.points.clear();
    return poly;
  }
  if (numVertices < 3)
  {
    poly.points.clear();
    return poly;
  }

  std::regex pointRegex(R"(\((-?\d+);(-?\d+)\))");
  std::smatch match;
  std::string remaining;
  std::getline(iss, remaining);
  size_t pointsFound = 0;
  size_t pos = 0;
  while (pointsFound < numVertices)
  {
    if (pos >= remaining.size()) break;
    std::string substr = remaining.substr(pos);
    if (std::regex_search(substr, match, pointRegex))
    {
      if (match.position() != 0)
      {
        poly.points.clear();
        return poly;
      }
      try
      {
        int x = std::stoi(match[1].str());
        int y = std::stoi(match[2].str());
        poly.points.push_back({x, y});
        pointsFound++;
        pos += match[0].length();
      }
      catch (const std::exception&)
      {
        poly.points.clear();
        return poly;
      }
    }
    else
    {
      poly.points.clear();
      return poly;
    }
    while (pos < remaining.size() && remaining[pos] == ' ')
    {
      pos++;
    }
  }
  if (pointsFound != numVertices)
  {
    poly.points.clear();
    return poly;
  }
  if (pos < remaining.size())
  {
    poly.points.clear();
    return poly;
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
  while (std::getline(file, line))
  {
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
