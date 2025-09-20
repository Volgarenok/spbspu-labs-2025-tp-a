#include "extraPerimeterCommands.hpp"
#include <functional>
#include <map>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include "polygon.hpp"
#include "areaCommands.hpp"

namespace pilugina
{
  double getLineSize(const Point &p1, const Point &p2)
  {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
  }

  double getPolygonPerimeter(const Polygon &poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    double perimeter;
    auto first = poly.points.begin();
    auto last = poly.points.end() - 1;
    perimeter = std::inner_product(first, last, first + 1, 0.0, std::plus<>(), getLineSize);
    perimeter += getLineSize(poly.points.back(), poly.points.front());
    return perimeter;
  }

  double calcPerimetersSum(const std::vector< Polygon > &polys)
  {
    std::vector< double > perimeters(polys.size());
    std::transform(polys.cbegin(), polys.cend(), perimeters.begin(), getPolygonPerimeter);
    return std::accumulate(perimeters.cbegin(), perimeters.cend(), 0.0);
  }

  double getEvenPerimeter(const std::vector< Polygon > &polys)
  {
    std::vector< Polygon > filtered;
    std::copy_if(polys.cbegin(), polys.cend(), std::back_inserter(filtered), isEvenVertexNum);
    return calcPerimetersSum(filtered);
  }

  double getOddPerimeter(const std::vector< Polygon > &polys)
  {
    std::vector< Polygon > filtered;
    std::copy_if(polys.cbegin(), polys.cend(), std::back_inserter(filtered), isOddVertexNum);
    return calcPerimetersSum(filtered);
  }

  double calcPerimetersEqVert(const std::vector< Polygon > &polys, int vertex_num)
  {
    std::vector< Polygon > filtered;
    auto sameVerticesNum = std::bind(isEqualVertexNum, vertex_num, std::placeholders::_1);
    std::copy_if(polys.cbegin(), polys.cend(), std::back_inserter(filtered), sameVerticesNum);
    return calcPerimetersSum(filtered);
  }

  double calcPerimeters(const std::vector< Polygon > &polys, std::istream &in)
  {
    std::map< std::string, std::function< double() > > subcommands
    {
      {"EVEN", std::bind(getEvenPerimeter, std::cref(polys))},
      {"ODD", std::bind(getOddPerimeter, std::cref(polys))}
    };

    std::string subcommand;
    in >> subcommand;

    auto it = subcommands.find(subcommand);
    if (it != subcommands.end())
    {
      return it->second();
    }
    else
    {
      int vertexCount = std::stoi(subcommand);
      if (vertexCount < 3)
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
      return calcPerimetersEqVert(polys, vertexCount);
    }
  }
}
