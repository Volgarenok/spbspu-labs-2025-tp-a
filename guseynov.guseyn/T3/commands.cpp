#include "commands.hpp"
#include "inframe.hpp"
#include "lessarea.hpp"
#include "utils.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>
#include <functional>
#include <numeric>

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
    explicit HasVertexCount(size_t targetCount) : target(targetCount) {}
    bool operator()(const guseynov::Polygon& poly) const
    {
      return poly.points.size() == target;
    }
    size_t target;
  };

  struct HasLessArea
  {
    explicit HasLessArea(double areaThreshold) : threshold(areaThreshold) {}
    bool operator()(const guseynov::Polygon& poly) const
    {
      return guseynov::utils::calculateArea(poly) < threshold;
    }
    double threshold;
  };

  struct AreaAccumulator
  {
    double operator()(double sum, const guseynov::Polygon& poly) const
    {
      return sum + guseynov::utils::calculateArea(poly);
    }
  };

  struct ConditionalAreaAccumulator
  {
    explicit ConditionalAreaAccumulator(const std::function<bool(const guseynov::Polygon&)>& pred) : predicate(pred) {}
    double operator()(double sum, const guseynov::Polygon& poly) const
    {
      if (predicate(poly))
      {
        return sum + guseynov::utils::calculateArea(poly);
      }
      return sum;
    }
    std::function<bool(const guseynov::Polygon&)> predicate;
  };

  struct ExtremeArea
  {
    explicit ExtremeArea(bool findMax) : isMax(findMax), extremeValue(isMax ? 0.0 : std::numeric_limits<double>::max()) {}
    void operator()(const guseynov::Polygon& poly)
    {
      double area = guseynov::utils::calculateArea(poly);
      if ((isMax && area > extremeValue) || (!isMax && area < extremeValue))
      {
        extremeValue = area;
      }
    }
    bool isMax;
    double extremeValue;
  };

  struct ExtremeVertices
  {
    explicit ExtremeVertices(bool findMax) : isMax(findMax), extremeValue(isMax ? 0 : std::numeric_limits<size_t>::max()) {}
    void operator()(const guseynov::Polygon& poly)
    {
      size_t vertices = poly.points.size();
      if ((isMax && vertices > extremeValue) || (!isMax && vertices < extremeValue))
      {
        extremeValue = vertices;
      }
    }
    bool isMax;
    size_t extremeValue;
  };

  struct FrameBounds
  {
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();

    void operator()(const guseynov::Polygon& poly)
    {
      std::for_each(poly.points.begin(), poly.points.end(), [this](const guseynov::Point& point) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
      });
    }
  };
}

void guseynov::commands::handleAreaCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (param == "EVEN")
  {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, ConditionalAreaAccumulator(IsEvenPolygon()));
    std::cout << std::fixed << std::setprecision(1) << sum << '\n';
  }
  else if (param == "ODD")
  {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, ConditionalAreaAccumulator(IsOddPolygon()));
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
  else if (guseynov::utils::isNumber(param))
  {
    size_t target = std::stoul(param);
    if (target < 3)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                                ConditionalAreaAccumulator(HasVertexCount(target)));
    std::cout << std::fixed << std::setprecision(1) << sum << '\n';
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
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
    ExtremeArea finder(true);
    std::for_each(polygons.begin(), polygons.end(), std::ref(finder));
    std::cout << std::fixed << std::setprecision(1) << finder.extremeValue << '\n';
  }
  else if (param == "VERTEXES")
  {
    ExtremeVertices finder(true);
    std::for_each(polygons.begin(), polygons.end(), std::ref(finder));
    std::cout << finder.extremeValue << '\n';
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
    ExtremeArea finder(false);
    std::for_each(polygons.begin(), polygons.end(), std::ref(finder));
    std::cout << std::fixed << std::setprecision(1) << finder.extremeValue << '\n';
  }
  else if (param == "VERTEXES")
  {
    ExtremeVertices finder(false);
    std::for_each(polygons.begin(), polygons.end(), std::ref(finder));
    std::cout << finder.extremeValue << '\n';
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
  else if (guseynov::utils::isNumber(param))
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

void guseynov::commands::handleInFrameCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  if (polygons.empty())
  {
    std::cout << "<FALSE>\n";
    return;
  }

  Polygon targetPoly = guseynov::utils::parsePolygon(param);
  if (targetPoly.points.size() < 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  FrameBounds bounds;
  std::for_each(polygons.begin(), polygons.end(), std::ref(bounds));

  auto isOutside = [&bounds](const Point& point) {
    return point.x < bounds.minX || point.x > bounds.maxX || point.y < bounds.minY || point.y > bounds.maxY;
  };

  bool allPointsInside = std::none_of(targetPoly.points.begin(), targetPoly.points.end(), isOutside);
  std::cout << (allPointsInside ? "<TRUE>" : "<FALSE>") << '\n';
}

void guseynov::commands::handleLessAreaCommand(const std::vector<Polygon>& polygons, const std::string& param)
{
  Polygon targetPoly = guseynov::utils::parsePolygon(param);
  if (targetPoly.points.size() < 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  double targetArea = guseynov::utils::calculateArea(targetPoly);
  size_t count = std::count_if(polygons.begin(), polygons.end(), HasLessArea(targetArea));
  std::cout << count << '\n';
}
