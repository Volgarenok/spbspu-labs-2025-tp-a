#include "io.h"
#include "geometry.h"
#include <fstream>
#include <stdexcept>
#include <cctype>

namespace
{
  bool isValidPointChar(char c)
  {
    return isdigit(c) || c == '(' || c == ')' || c == ';' || c == '-' || c == ' ';
  }
}

namespace asafov
{
  std::vector<Polygon> readPolygonsFromFile(const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file)
    {
      throw std::runtime_error("Cannot open file");
    }

    std::vector<Polygon> polygons;
    std::string line;

    while (std::getline(file, line))
    {
      try
      {
        if (line.empty()) continue;

        for (char c : line)
        {
          if (!isValidPointChar(c) && !isspace(c))
          {
            throw std::invalid_argument("Invalid character in input");
          }
        }

        if (line.find("(;)") != std::string::npos)
        {
          throw std::invalid_argument("Empty point coordinates");
        }

        size_t space_pos = line.find(' ');
        if (space_pos == std::string::npos) continue;

        size_t num_vertices = 0;
        for (size_t i = 0; i < space_pos; ++i)
        {
          if (!isdigit(line[i]))
          {
            throw std::invalid_argument("Invalid vertex count");
          }
          num_vertices = num_vertices * 10 + (line[i] - '0');
        }

        if (num_vertices < 3) continue;

        Polygon poly;
        size_t pos = space_pos + 1;

        while (pos < line.size())
        {
          while (pos < line.size() && isspace(line[pos])) ++pos;
          if (pos >= line.size()) break;

          if (line[pos] != '(')
          {
            throw std::invalid_argument("Expected '('");
          }
          ++pos;

          int x = 0;
          bool negative = false;
          if (pos < line.size() && line[pos] == '-')
          {
            negative = true;
            ++pos;
          }
          while (pos < line.size() && isdigit(line[pos]))
          {
            x = x * 10 + (line[pos] - '0');
            ++pos;
          }
          if (negative) x = -x;

          if (pos >= line.size() || line[pos] != ';')
          {
            throw std::invalid_argument("Expected ';'");
          }
          ++pos;

          int y = 0;
          negative = false;
          if (pos < line.size() && line[pos] == '-')
          {
            negative = true;
            ++pos;
          }
          while (pos < line.size() && isdigit(line[pos]))
          {
            y = y * 10 + (line[pos] - '0');
            ++pos;
          }
          if (negative) y = -y;

          if (pos >= line.size() || line[pos] != ')')
          {
            throw std::invalid_argument("Expected ')'");
          }
          ++pos;

          poly.points.push_back({x, y});
        }

        if (poly.points.size() == num_vertices)
        {
          polygons.push_back(poly);
        }
      }
      catch (...)
      {
        continue;
      }
    }

    return polygons;
  }
}
