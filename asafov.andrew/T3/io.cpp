#include "io.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <algorithm>

namespace asafov
{
  std::vector<Polygon> readPolygonsFromFile(const std::string& filename)
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
      if (line.empty()) continue;
      
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

  Polygon parsePolygonFromString(const std::string& str)
  {
    Polygon poly;
    std::istringstream iss(str);
    
    size_t vertexCount;
    iss >> vertexCount;
    
    if (vertexCount < 3)
    {
      throw std::invalid_argument("Invalid vertex count");
    }
    
    for (size_t i = 0; i < vertexCount; ++i)
    {
      char ch;
      iss >> ch;
      
      if (ch != '(')
      {
        throw std::invalid_argument("Expected '('");
      }
      
      int x, y;
      char semicolon;
      
      if (!(iss >> x >> semicolon >> y) || semicolon != ';')
      {
        throw std::invalid_argument("Invalid point format");
      }
      
      iss >> ch;
      if (ch != ')')
      {
        throw std::invalid_argument("Expected ')'");
      }
      
      poly.points.push_back({x, y});
    }
    
    return poly;
  }
}
