#include "utils.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <numeric>
#include <functional>

namespace
{
  std::string extractFirstWord(const std::string& str)
  {
    auto isSpace = [](char c) { return std::isspace(static_cast<unsigned char>(c)); };
    auto start = std::find_if_not(str.begin(), str.end(), isSpace);
    if (start == str.end())
    {
      return "";
    }
    auto end = std::find_if(start, str.end(), isSpace);
    return std::string(start, end);
  }

  std::string extractRemainingString(const std::string& str, size_t startPos = 0)
  {
    auto isSpace = [](char c) { return std::isspace(static_cast<unsigned char>(c)); };
    auto remainingStart = std::find_if_not(str.begin() + startPos, str.end(), isSpace);
    if (remainingStart == str.end())
    {
      return "";
    }
    return std::string(remainingStart, str.end());
  }

  struct PointAccumulator
  {
    double operator()(double area, const std::pair<size_t, size_t>& indices) const
    {
      return area + indices.first * indices.second;
    }
  };
}

bool guseynov::utils::isNumber(const std::string& s)
{
  auto isDigit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };
  return !s.empty() && std::all_of(s.begin(), s.end(), isDigit);
}

double guseynov::utils::calculateArea(const Polygon& poly)
{
  if (poly.points.size() < 3)
  {
    return 0.0;
  }

  std::vector< std::pair<int, int> > products;
  products.reserve(poly.points.size());

  std::transform(poly.points.begin(), poly.points.end() - 1,
                 poly.points.begin() + 1,
                 std::back_inserter(products),
                 [](const Point& a, const Point& b) {
                   return std::make_pair(a.x, b.y);
                 });

  std::vector< std::pair<int, int> > negativeProducts;
  negativeProducts.reserve(poly.points.size());
  std::transform(poly.points.begin() + 1, poly.points.end(),
                 poly.points.begin(),
                 std::back_inserter(negativeProducts),
                 [](const Point& b, const Point& a) {
                   return std::make_pair(b.x, a.y);
                 });

  double positiveSum = std::accumulate(products.begin(), products.end(), 0.0, PointAccumulator());
  double negativeSum = std::accumulate(negativeProducts.begin(), negativeProducts.end(), 0.0, PointAccumulator());

  positiveSum += poly.points.back().x * poly.points.front().y;
  negativeSum += poly.points.front().x * poly.points.back().y;

  return std::abs(positiveSum - negativeSum) / 2.0;
}

guseynov::Polygon guseynov::utils::parsePolygon(const std::string& line)
{
  Polygon poly;
  std::string remaining = line;
  std::string numVerticesStr = extractFirstWord(remaining);
  remaining = extractRemainingString(remaining, numVerticesStr.size());
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

  auto parsePoint = [](const std::string& pointStr) -> Point {
    if (pointStr.size() < 5 || pointStr[0] != '(' || pointStr.back() != ')')
    {
      throw std::invalid_argument("Invalid point format");
    }
    size_t semicolon = pointStr.find(';');
    if (semicolon == std::string::npos || semicolon < 2 || semicolon > pointStr.size() - 2)
    {
      throw std::invalid_argument("Invalid point format");
    }
    std::string xStr = pointStr.substr(1, semicolon - 1);
    std::string yStr = pointStr.substr(semicolon + 1, pointStr.size() - semicolon - 2);
    auto isInvalidChar = [](char c) { return !std::isdigit(static_cast<unsigned char>(c)) && c != '-'; };
    if (xStr.empty() || yStr.empty() ||
        std::any_of(xStr.begin(), xStr.end(), isInvalidChar) ||
        std::any_of(yStr.begin(), yStr.end(), isInvalidChar))
    {
      throw std::invalid_argument("Invalid point coordinates");
    }
    return {std::stoi(xStr), std::stoi(yStr)};
  };

  try
  {
    for (size_t i = 0; i < numVertices; ++i)
    {
      std::string pointStr = extractFirstWord(remaining);
      if (pointStr.empty())
      {
        poly.points.clear();
        return poly;
      }
      remaining = extractRemainingString(remaining, pointStr.size());
      poly.points.push_back(parsePoint(pointStr));
    }
  }
  catch (const std::exception&)
  {
    poly.points.clear();
    return poly;
  }
  if (!remaining.empty())
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

  auto trim = [](std::string& str) {
    auto isSpace = [](char c) { return std::isspace(static_cast<unsigned char>(c)); };
    str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), isSpace));
    str.erase(std::find_if_not(str.rbegin(), str.rend(), isSpace).base(), str.end());
  };

  std::string line;
  while (std::getline(file, line))
  {
    trim(line);
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
