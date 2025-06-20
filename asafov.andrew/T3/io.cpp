#include "io.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>

std::vector< asafov::Polygon > asafov::readPolygonsFromFile(const std::string& filename)
{
  auto parsePoint = [](const std::string& s) ->Point
  {
    size_t p1 = s.find('('), p2 = s.find(';'), p3 = s.find(')');
    if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos)
      throw std::invalid_argument("Invalid point format");
    return {
      std::stoi(s.substr(p1 + 1, p2 - p1 - 1)),
      std::stoi(s.substr(p2 + 1, p3 - p2 - 1))
    };
  };

  std::ifstream file(filename);
  if (!file) throw std::runtime_error("File error");

  std::vector< Polygon > polygons;
  std::string line;

  while (std::getline(file, line))
  {
    size_t space_pos = line.find(' ');
    if (space_pos == std::string::npos) continue;

    size_t num_vertices;
    try
    {
      num_vertices = std::stoul(line.substr(0, space_pos));
    }
    catch (...)
    {
      continue;
    }

    std::vector< Point > points;
    auto start = line.begin() + space_pos + 1;
    auto end = line.end();

    std::vector< std::string > tokens;
    auto it = start;
    while (it != end)
    {
      auto next = std::find(it, end, ' ');
      tokens.emplace_back(it, next);
      it = next != end ? next + 1 : next;
    }

    if (tokens.size() < num_vertices) continue;

    std::transform(tokens.begin(), tokens.begin() + num_vertices, std::back_inserter(points), parsePoint);

    if (points.size() == num_vertices) polygons.push_back({points});
  }

  return polygons;
}