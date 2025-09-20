#include "utils.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <numeric>

namespace
{
  struct IsSpace
  {
    bool operator()(char c) const
    {
      return std::isspace(static_cast<unsigned char>(c));
    }
  };

  struct IsDigit
  {
    bool operator()(char c) const
    {
      return std::isdigit(static_cast<unsigned char>(c));
    }
  };

  struct IsInvalidChar
  {
    bool operator()(char c) const
    {
      return !std::isdigit(static_cast<unsigned char>(c)) && c != '-';
    }
  };

  struct TrimSpaces
  {
    void operator()(std::string& str) const
    {
      IsSpace isSpace;
      str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), isSpace));
      str.erase(std::find_if_not(str.rbegin(), str.rend(), isSpace).base(), str.end());
    }
  };
}

bool guseynov::utils::isNumber(const std::string& s)
{
  return !s.empty() && std::all_of(s.begin(), s.end(), IsDigit());
}

double guseynov::utils::calculateArea(const Polygon& poly)
{
  if (poly.points.size() < 3)
  {
    return 0.0;
  }

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
  std::string remaining = line;
  IsSpace isSpace;
  auto start = std::find_if_not(remaining.begin(), remaining.end(), isSpace);
  auto end = std::find_if(start, remaining.end(), isSpace);
  std::string numVerticesStr(start, end);
  if (numVerticesStr.empty())
  {
    poly.points.clear();
    return poly;
  }
  size_t numVertices = 0;
  try
  {
    numVertices = std::stoul(numVerticesStr);
  }
  catch (const std::exception&)
  {
    poly.points.clear();
    return poly;
  }
  if (numVertices < 3)
  {
    poly.points.clear();
    return poly;
  }

  remaining = std::string(end, remaining.end());
  for (size_t i = 0; i < numVertices; ++i)
  {
    start = std::find_if_not(remaining.begin(), remaining.end(), isSpace);
    if (start == remaining.end())
    {
      poly.points.clear();
      return poly;
    }
    end = std::find_if(start, remaining.end(), isSpace);
    std::string pointStr(start, end);
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
    std::string xStr = pointStr.substr(1, semicolon - 1);
    std::string yStr = pointStr.substr(semicolon + 1, pointStr.size() - semicolon - 2);
    IsInvalidChar isInvalidChar;
    if (xStr.empty() || yStr.empty() ||
        std::any_of(xStr.begin(), xStr.end(), isInvalidChar) ||
        std::any_of(yStr.begin(), yStr.end(), isInvalidChar))
    {
      poly.points.clear();
      return poly;
    }
    try
    {
      int x = std::stoi(xStr);
      int y = std::stoi(yStr);
      poly.points.push_back({x, y});
    }
    catch (const std::exception&)
    {
      poly.points.clear();
      return poly;
    }
    remaining = std::string(end, remaining.end());
  }
  if (std::find_if_not(remaining.begin(), remaining.end(), isSpace) != remaining.end())
  {
    poly.points.clear();
    return poly;
  }
  return poly;
}

std::vector<guseynov::Polygon> guseynov::utils::readPolygonsFromFile(const std::string& filename)
{
  std::vector<Polygon> polygons;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file: " + filename);
  }
  std::string line;
  TrimSpaces trimSpaces;
  while (std::getline(file, line))
  {
    trimSpaces(line);
    if (line.empty())
    {
      continue;
    }
    Polygon poly = parsePolygon(line);
    if (poly.points.size() >= 3)
    {
      polygons.push_back(poly);
    }
  }
  return polygons;
}
