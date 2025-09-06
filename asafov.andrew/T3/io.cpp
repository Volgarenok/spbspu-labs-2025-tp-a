#include "io.h"
#include <fstream>
#include <stdexcept>
#include <cctype>

namespace
{
  bool isWhitespace(char c)
  {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
  }
}

void asafov::skipWhitespace(const std::string& str, size_t& pos)
{
  while (pos < str.size() && isWhitespace(str[pos]))
  {
    ++pos;
  }
}

bool asafov::parseNumber(const std::string& str, size_t& pos, int& number)
{
  number = 0;
  bool negative = false;
  bool foundDigit = false;

  skipWhitespace(str, pos);

  if (pos < str.size() && str[pos] == '-')
  {
    negative = true;
    ++pos;
  }

  while (pos < str.size() && std::isdigit(str[pos]))
  {
    number = number * 10 + (str[pos] - '0');
    ++pos;
    foundDigit = true;
  }

  if (negative)
  {
    number = -number;
  }

  return foundDigit;
}

bool asafov::parsePoint(const std::string& str, size_t& pos, Point& point)
{
  skipWhitespace(str, pos);

  if (pos >= str.size() || str[pos] != '(')
  {
    return false;
  }
  ++pos;

  int x = 0;
  if (!parseNumber(str, pos, x))
  {
    return false;
  }

  skipWhitespace(str, pos);
  if (pos >= str.size() || str[pos] != ';')
  {
    return false;
  }
  ++pos;

  int y = 0;
  if (!parseNumber(str, pos, y))
  {
    return false;
  }

  skipWhitespace(str, pos);
  if (pos >= str.size() || str[pos] != ')')
  {
    return false;
  }
  ++pos;

  point = {x, y};
  return true;
}

std::vector<Polygon> asafov::readPolygonsFromFile(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  std::vector<Polygon> polygons;
  std::string line;

  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    try
    {
      Polygon poly = parsePolygonFromString(line);
      if (poly.points.size() >= 3)
      {
        polygons.push_back(poly);
      }
    }
    catch (const std::exception&)
    {
      continue;
    }
  }

  return polygons;
}

Polygon asafov::parsePolygonFromString(const std::string& str)
{
  Polygon poly;
  size_t pos = 0;

  skipWhitespace(str, pos);

  size_t vertexCount = 0;
  bool foundDigit = false;

  while (pos < str.size() && std::isdigit(str[pos]))
  {
    vertexCount = vertexCount * 10 + (str[pos] - '0');
    ++pos;
    foundDigit = true;
  }

  if (!foundDigit || vertexCount < 3)
  {
    throw std::invalid_argument("Invalid vertex count");
  }

  for (size_t i = 0; i < vertexCount; ++i)
  {
    Point point;
    if (!parsePoint(str, pos, point))
    {
      throw std::invalid_argument("Invalid point format");
    }
    poly.points.push_back(point);
  }

  skipWhitespace(str, pos);
  if (pos < str.size())
  {
    throw std::invalid_argument("Extra characters after polygon definition");
  }

  return poly;
}
