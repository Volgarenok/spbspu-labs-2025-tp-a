#include "commands.hpp"

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>

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
    float area = std::accumulate(polygons.begin(), polygons.end(), 0.0f,
        [](float acc, const zholobov::Polygon& polygon) {
          return acc + zholobov::calcArea(polygon);
        });
    return area / polygons.size();
  }

  float calcAreaNumOfVertices(const zholobov::Polygons& polygons, size_t numberOfVertices)
  {
    return sumAreaIf(polygons, NumOfVerticesEqualTo{numberOfVertices});
  }

  float calcMaxArea(const zholobov::Polygons& polygons)
  {
    std::vector< float > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas),
        [](const zholobov::Polygon& polygon) {
          return zholobov::calcArea(polygon);
        });
    return *std::max_element(areas.begin(), areas.end());
  }

  float calcMaxNumberOfVertices(const zholobov::Polygons& polygons)
  {
    std::vector< float > numOfVertices;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(numOfVertices),
        [](const zholobov::Polygon& polygon) {
          return polygon.points.size();
        });
    return *std::max_element(numOfVertices.begin(), numOfVertices.end());
  }

  float calcMinArea(const zholobov::Polygons& polygons)
  {
    std::vector< float > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas),
        [](const zholobov::Polygon& polygon) {
          return zholobov::calcArea(polygon);
        });

    return *std::min_element(areas.begin(), areas.end());
  }

  float calcMinNumberOfVertices(const zholobov::Polygons& polygons)
  {
    std::vector< float > numOfVertices;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(numOfVertices),
        [](const zholobov::Polygon& polygon) {
          return polygon.points.size();
        });

    return *std::min_element(numOfVertices.begin(), numOfVertices.end());
  }

  float calcCountEven(const zholobov::Polygons& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), isEvenVertices);
  }

  float calcCountOdd(const zholobov::Polygons& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), isOddVertices);
  }

  float calcCountNumOfVertices(const zholobov::Polygons& polygons, size_t numberOfVertices)
  {
    return std::count_if(polygons.begin(), polygons.end(), NumOfVerticesEqualTo{numberOfVertices});
  }

}

void zholobov::cmdArea(std::istream& input, std::ostream& output, [[maybe_unused]] const Polygons& polygons)
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
  output << result;
}

void zholobov::cmdMax(std::istream& input, std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  std::string param;
  if (!(input >> param)) {
    throw std::invalid_argument("MAX failed");
  }

  std::map< std::string, std::function< float() > > cmdParam;
  cmdParam["AREA"] = std::bind(calcMaxArea, std::cref(polygons));
  cmdParam["VERTEXES"] = std::bind(calcMaxNumberOfVertices, std::cref(polygons));
  float result = cmdParam.at(param)();
  output << result;
}

void zholobov::cmdMin(std::istream& input, std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  std::string param;
  if (!(input >> param)) {
    throw std::invalid_argument("MIN failed");
  }

  std::map< std::string, std::function< float() > > cmdParam;
  cmdParam["AREA"] = std::bind(calcMinArea, std::cref(polygons));
  cmdParam["VERTEXES"] = std::bind(calcMinNumberOfVertices, std::cref(polygons));
  float result = cmdParam.at(param)();
  output << result;
}

void zholobov::cmdCount(std::istream& input, std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  std::string param;
  if (!(input >> param)) {
    throw std::invalid_argument("COUNT failed");
  }

  std::map< std::string, std::function< float() > > cmdParam;
  cmdParam["EVEN"] = std::bind(calcCountEven, std::cref(polygons));
  cmdParam["ODD"] = std::bind(calcCountOdd, std::cref(polygons));

  float result = 0.0f;
  try {
    result = cmdParam.at(param)();
  } catch (std::out_of_range&) {
    size_t numberOfVertices = std::stoul(param);
    result = calcCountNumOfVertices(polygons, numberOfVertices);
  }
  output << result;
}

void zholobov::cmdIntersections(std::istream& input, std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  Polygon param;
  if (!(input >> param)) {
    throw std::invalid_argument("INTERSECTIONS failed");
  }
  output << std::count_if(polygons.begin(), polygons.end(), CheckIfIntersects{param});
}

void zholobov::cmdRects(std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  output << "Not implemented\n";
}
