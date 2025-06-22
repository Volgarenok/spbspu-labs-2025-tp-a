#include "commands.hpp"

#include <algorithm>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>

#include "stream_guard.hpp"

namespace {

  using Predicate = std::function< bool(const zholobov::Polygon&) >;

  bool isEvenVertices(const zholobov::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOddVertices(const zholobov::Polygon& polygon)
  {
    return !isEvenVertices(polygon);
  }

  bool isRect(const zholobov::Polygon& polygon)
  {
    if (polygon.points.size() != 4) {
      return false;
    }
    zholobov::Point v0{polygon.points[0].x - polygon.points[3].x, polygon.points[0].y - polygon.points[3].y};
    for (size_t i = 0; i < 3; ++i) {
      zholobov::Point v{polygon.points[i + 1].x - polygon.points[i].x, polygon.points[i + 1].y - polygon.points[i].y};
      if ((v0.x * v.x + v0.y * v.y) != 0) {
        return false;
      }
      v0 = v;
    }
    return true;
  }

  struct NumOfVerticesEqualTo {
    const size_t num;
    bool operator()(const zholobov::Polygon& polygon) const
    {
      return polygon.points.size() == num;
    }
  };

  struct CheckIfIntersects {
    const zholobov::Polygon& poly;
    bool operator()(const zholobov::Polygon& polygon) const
    {
      return zholobov::hasIntersection(poly, polygon);
    }
  };

  float sumAreaIf(const zholobov::Polygons& polygons, const Predicate& pred)
  {
    return std::accumulate(polygons.begin(), polygons.end(), 0.0f,
        [&pred](float acc, const zholobov::Polygon& polygon) {
          if (pred(polygon)) {
            acc += zholobov::calcArea(polygon);
          }
          return acc;
        });
  }

  float calcAreaEven(const zholobov::Polygons& polygons)
  {
    return sumAreaIf(polygons, isEvenVertices);
  }

  float calcAreaOdd(const zholobov::Polygons& polygons)
  {
    return sumAreaIf(polygons, isOddVertices);
  }

  float calcAreaMean(const zholobov::Polygons& polygons)
  {
    if (polygons.size() == 0) {
      throw std::logic_error("There are no poligons");
    }
    float area = std::accumulate(polygons.begin(), polygons.end(), 0.0f,
        [](float acc, const zholobov::Polygon& polygon) {
          return acc + zholobov::calcArea(polygon);
        });
    return area / polygons.size();
  }

  float calcAreaNumOfVertices(const zholobov::Polygons& polygons, size_t numberOfVertices)
  {
    if (numberOfVertices < 3) {
      throw std::out_of_range("Few vertices");
    }
    return sumAreaIf(polygons, NumOfVerticesEqualTo{numberOfVertices});
  }

  void calcMaxArea(std::ostream& output, const zholobov::Polygons& polygons)
  {
    if (polygons.size() == 0) {
      throw std::logic_error("There are no poligons");
    }
    std::vector< float > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas),
        [](const zholobov::Polygon& polygon) {
          return zholobov::calcArea(polygon);
        });
    zholobov::StreamGuard guard(output);
    output << std::fixed << std::setprecision(1) << *std::max_element(areas.begin(), areas.end());
  }

  void calcMaxNumberOfVertices(std::ostream& output, const zholobov::Polygons& polygons)
  {
    if (polygons.size() == 0) {
      throw std::logic_error("There are no poligons");
    }
    std::vector< size_t > numOfVertices;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(numOfVertices),
        [](const zholobov::Polygon& polygon) {
          return polygon.points.size();
        });
    output << *std::max_element(numOfVertices.begin(), numOfVertices.end());
  }

  void calcMinArea(std::ostream& output, const zholobov::Polygons& polygons)
  {
    if (polygons.size() == 0) {
      throw std::logic_error("There are no poligons");
    }
    std::vector< float > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas),
        [](const zholobov::Polygon& polygon) {
          return zholobov::calcArea(polygon);
        });
    zholobov::StreamGuard guard(output);
    output << std::fixed << std::setprecision(1) << *std::min_element(areas.begin(), areas.end());
  }

  void calcMinNumberOfVertices(std::ostream& output, const zholobov::Polygons& polygons)
  {
    if (polygons.size() == 0) {
      throw std::logic_error("There are no poligons");
    }
    std::vector< float > numOfVertices;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(numOfVertices),
        [](const zholobov::Polygon& polygon) {
          return polygon.points.size();
        });
    output << *std::min_element(numOfVertices.begin(), numOfVertices.end());
  }

  size_t calcCountEven(const zholobov::Polygons& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), isEvenVertices);
  }

  size_t calcCountOdd(const zholobov::Polygons& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), isOddVertices);
  }

  size_t calcCountNumOfVertices(const zholobov::Polygons& polygons, size_t numberOfVertices)
  {
    if (numberOfVertices < 3) {
      throw std::out_of_range("Few vertices");
    }
    return std::count_if(polygons.begin(), polygons.end(), NumOfVerticesEqualTo{numberOfVertices});
  }
}

void zholobov::cmdArea(std::istream& input, std::ostream& output, const Polygons& polygons)
{
  std::string param;
  if (!(input >> param)) {
    throw std::invalid_argument("AREA failed");
  }

  std::map< std::string, std::function< float() > > cmdParam;
  cmdParam["EVEN"] = std::bind(calcAreaEven, std::cref(polygons));
  cmdParam["ODD"] = std::bind(calcAreaOdd, std::cref(polygons));
  cmdParam["MEAN"] = std::bind(calcAreaMean, std::cref(polygons));

  float result = 0.0f;
  try {
    result = cmdParam.at(param)();
  } catch (std::out_of_range&) {
    size_t numberOfVertices = std::stoul(param);
    result = calcAreaNumOfVertices(polygons, numberOfVertices);
  }
  zholobov::StreamGuard guard(output);
  output << std::fixed << std::setprecision(1) << result;
}

void zholobov::cmdMax(std::istream& input, std::ostream& output, const Polygons& polygons)
{
  std::string param;
  if (!(input >> param)) {
    throw std::invalid_argument("MAX failed");
  }

  std::map< std::string, std::function< void() > > cmdParam;
  cmdParam["AREA"] = std::bind(calcMaxArea, std::ref(output), std::cref(polygons));
  cmdParam["VERTEXES"] = std::bind(calcMaxNumberOfVertices, std::ref(output), std::cref(polygons));
  cmdParam.at(param)();
}

void zholobov::cmdMin(std::istream& input, std::ostream& output, const Polygons& polygons)
{
  std::string param;
  if (!(input >> param)) {
    throw std::invalid_argument("MIN failed");
  }

  std::map< std::string, std::function< void() > > cmdParam;
  cmdParam["AREA"] = std::bind(calcMinArea, std::ref(output), std::cref(polygons));
  cmdParam["VERTEXES"] = std::bind(calcMinNumberOfVertices, std::ref(output), std::cref(polygons));
  cmdParam.at(param)();
}

void zholobov::cmdCount(std::istream& input, std::ostream& output, const Polygons& polygons)
{
  std::string param;
  if (!(input >> param)) {
    throw std::invalid_argument("COUNT failed");
  }

  std::map< std::string, std::function< size_t() > > cmdParam;
  cmdParam["EVEN"] = std::bind(calcCountEven, std::cref(polygons));
  cmdParam["ODD"] = std::bind(calcCountOdd, std::cref(polygons));

  size_t result = 0;
  try {
    result = cmdParam.at(param)();
  } catch (std::out_of_range&) {
    size_t numberOfVertices = std::stoul(param);
    result = calcCountNumOfVertices(polygons, numberOfVertices);
  }
  output << result;
}

void zholobov::cmdIntersections(std::istream& input, std::ostream& output, const Polygons& polygons)
{
  Polygon param;
  if (!(input >> param)) {
    throw std::invalid_argument("INTERSECTIONS failed");
  }
  output << std::count_if(polygons.begin(), polygons.end(), CheckIfIntersects{param});
}

void zholobov::cmdRects(std::ostream& output, const Polygons& polygons)
{
  output << std::count_if(polygons.begin(), polygons.end(), isRect);
}
