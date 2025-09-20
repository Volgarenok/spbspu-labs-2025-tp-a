#include "commands.hpp"
#include "inframe.hpp"
#include "lessarea.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>
#include <functional>
#include <numeric>
#include <fstream>
#include <sstream>
#include <cctype>

namespace
{
  struct IsEvenPolygon
  {
    bool operator()(const guseynov::Polygon& poly) const
    {
      return poly.points.size() % 2 == 0;
    }
  };

  struct IsOddPolygon
  {
    bool operator()(const guseynov::Polygon& poly) const
    {
      return poly.points.size() % 2 != 0;
    }
  };

  struct HasVertexCount
  {
    explicit HasVertexCount(size_t targetCount):
      target(targetCount)
    {}
    bool operator()(const guseynov::Polygon& poly) const
    {
      return poly.points.size() == target;
    }
    size_t target;
  };

  struct HasLessArea
  {
    explicit HasLessArea(double areaThreshold):
      threshold(areaThreshold)
    {}
    bool operator()(const guseynov::Polygon& poly) const
    {
      return guseynov::commands::calculateArea(poly) < threshold;
    }
    double threshold;
  };

  struct AreaAccumulator
  {
    double operator()(double sum, const guseynov::Polygon& poly) const
    {
      return sum + guseynov::commands::calculateArea(poly);
    }
  };

  struct ConditionalAreaAccumulatorEven
  {
    double operator()(double sum, const guseynov::Polygon& poly) const
    {
      return IsEvenPolygon()(poly) ? sum + guseynov::commands::calculateArea(poly) : sum;
    }
  };

  struct ConditionalAreaAccumulatorOdd
  {
    double operator()(double sum, const guseynov::Polygon& poly) const
    {
      return IsOddPolygon()(poly) ? sum + guseynov::commands::calculateArea(poly) : sum;
    }
  };

  struct ConditionalAreaAccumulatorVertexCount
  {
    explicit ConditionalAreaAccumulatorVertexCount(size_t target):
      target_(target)
    {}
    double operator()(double sum, const guseynov::Polygon& poly) const
    {
      return HasVertexCount(target_)(poly) ? sum + guseynov::commands::calculateArea(poly) : sum;
    }
    size_t target_;
  };

  struct MaxAreaFinder
  {
    double maxArea = 0.0;
    void operator()(const guseynov::Polygon& poly)
    {
      double area = guseynov::commands::calculateArea(poly);
      if (area > maxArea)
      {
        maxArea = area;
      }
    }
  };

  struct MinAreaFinder
  {
    double minArea = std::numeric_limits<double>::max();
    void operator()(const guseynov::Polygon& poly)
    {
      double area = guseynov::commands::calculateArea(poly);
      if (area < minArea)
      {
        minArea = area;
      }
    }
  };

  struct MaxVerticesFinder
  {
    size_t maxVertices = 0;
    void operator()(const guseynov::Polygon& poly)
    {
      size_t vertices = poly.points.size();
      if (vertices > maxVertices)
      {
        maxVertices = vertices;
      }
    }
  };

  struct MinVerticesFinder
  {
    size_t minVertices = std::numeric_limits<size_t>::max();
    void operator()(const guseynov::Polygon& poly)
    {
      size_t vertices = poly.points.size();
      if (vertices < minVertices)
      {
        minVertices = vertices;
      }
    }
  };

  struct FrameBoundsAccumulator
  {
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();
    void operator()(const guseynov::Polygon& poly)
    {
      for (const auto& point : poly.points)
      {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
      }
    }
  };

  struct PointOutsideFrame
  {
    int minX;
    int maxX;
    int minY;
    int maxY;
    PointOutsideFrame(int minX_, int maxX_, int minY_, int maxY_):
      minX(minX_),
      maxX(maxX_),
      minY(minY_),
      maxY(maxY_)
    {}
    bool operator()(const guseynov::Point& point) const
    {
      return point.x < minX || point.x > maxX || point.y < minY || point.y > maxY;
    }
  };
}

double guseynov::commands::calculateArea(const Polygon& poly)
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

void guseynov::commands::parsePolygon(std::istream& in, Polygon& poly)
{
  poly.points.clear();
  in >> poly;
}

void guseynov::commands::readPolygonsFromFile(std::istream& in, std::vector<Polygon>& polygons)
{
  polygons.clear();
  Polygon poly;
  while (in >> poly)
  {
    if (poly.points.size() >= 3)
    {
      polygons.push_back(poly);
    }
    poly.points.clear();
    in.clear();
    std::string line;
    while (std::getline(in, line))
    {
      std::istringstream lineStream(line);
      if (lineStream >> poly)
      {
        if (poly.points.size() >= 3)
        {
          polygons.push_back(poly);
        }
        poly.points.clear();
        break;
      }
    }
  }
}

void guseynov::commands::handleAreaCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (param == "EVEN")
  {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, ConditionalAreaAccumulatorEven());
    std::cout << std::fixed << std::setprecision(1) << sum << '\n';
  }
  else if (param == "ODD")
  {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, ConditionalAreaAccumulatorOdd());
    std::cout << std::fixed << std::setprecision(1) << sum << '\n';
  }
  else if (param == "MEAN")
  {
    if (polygons.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaAccumulator());
    std::cout << std::fixed << std::setprecision(1) << total / polygons.size() << '\n';
  }
  else
  {
    bool isNumber = true;
    for (char c : param)
    {
      if (!std::isdigit(static_cast<unsigned char>(c)))
      {
        isNumber = false;
        break;
      }
    }
    if (isNumber)
    {
      size_t target = std::stoul(param);
      if (target < 3)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, ConditionalAreaAccumulatorVertexCount(target));
      std::cout << std::fixed << std::setprecision(1) << sum << '\n';
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

void guseynov::commands::handleMaxCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  if (param == "AREA")
  {
    MaxAreaFinder finder;
    for (const auto& poly : polygons)
    {
      finder(poly);
    }
    std::cout << std::fixed << std::setprecision(1) << finder.maxArea << '\n';
  }
  else if (param == "VERTEXES")
  {
    MaxVerticesFinder finder;
    for (const auto& poly : polygons)
    {
      finder(poly);
    }
    std::cout << finder.maxVertices << '\n';
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void guseynov::commands::handleMinCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  if (param == "AREA")
  {
    MinAreaFinder finder;
    for (const auto& poly : polygons)
    {
      finder(poly);
    }
    std::cout << std::fixed << std::setprecision(1) << finder.minArea << '\n';
  }
  else if (param == "VERTEXES")
  {
    MinVerticesFinder finder;
    for (const auto& poly : polygons)
    {
      finder(poly);
    }
    std::cout << finder.minVertices << '\n';
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void guseynov::commands::handleCountCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (param == "EVEN")
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(), IsEvenPolygon());
    std::cout << count << '\n';
  }
  else if (param == "ODD")
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(), IsOddPolygon());
    std::cout << count << '\n';
  }
  else
  {
    bool isNumber = true;
    for (char c : param)
    {
      if (!std::isdigit(static_cast<unsigned char>(c)))
      {
        isNumber = false;
        break;
      }
    }
    if (isNumber)
    {
      size_t target = std::stoul(param);
      if (target < 3)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      size_t count = std::count_if(polygons.begin(), polygons.end(), HasVertexCount(target));
      std::cout << count << '\n';
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

void guseynov::commands::handleInFrameCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<FALSE>\n";
    return;
  }

  std::istringstream iss(param);
  Polygon targetPoly;
  parsePolygon(iss, targetPoly);
  if (targetPoly.points.size() < 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  FrameBoundsAccumulator bounds;
  for (const auto& poly : polygons)
  {
    bounds(poly);
  }

  bool allPointsInside = std::none_of(targetPoly.points.begin(), targetPoly.points.end(),
    PointOutsideFrame(bounds.minX, bounds.maxX, bounds.minY, bounds.maxY));
  std::cout << (allPointsInside ? "<TRUE>" : "<FALSE>") << '\n';
}

void guseynov::commands::handleLessAreaCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  std::istringstream iss(param);
  Polygon targetPoly;
  parsePolygon(iss, targetPoly);
  if (targetPoly.points.size() < 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  double targetArea = calculateArea(targetPoly);
  size_t count = std::count_if(polygons.begin(), polygons.end(), HasLessArea(targetArea));
  std::cout << count << '\n';
}
