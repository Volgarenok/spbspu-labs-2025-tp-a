#include "commands.hpp"

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>

namespace {
  float calcAreaEven(const zholobov::Polygons& polygons)
  {
    return std::accumulate(polygons.begin(), polygons.end(), 0.0f,
        [](float acc, const zholobov::Polygon& polygon) {
          if (polygon.points.size() % 2 == 0) {
            acc += zholobov::calcArea(polygon);
          }
          return acc;
        });
  }

  float calcAreaOdd(const zholobov::Polygons& polygons)
  {
    return std::accumulate(polygons.begin(), polygons.end(), 0.0f,
        [](float acc, const zholobov::Polygon& polygon) {
          if (polygon.points.size() % 2 != 0) {
            acc += zholobov::calcArea(polygon);
          }
          return acc;
        });
  }

  float calcAreaMean(const zholobov::Polygons& polygons)
  {
    float area = std::accumulate(polygons.begin(), polygons.end(), 0.0f,
        [](float acc, const zholobov::Polygon& polygon) {
          return acc + zholobov::calcArea(polygon);
        });
    return area / polygons.size();
  }

  float calcAreaNumOfVertexes(const zholobov::Polygons& polygons, unsigned long numberOfVertexes)
  {
    return std::accumulate(polygons.begin(), polygons.end(), 0.0f,
        [numberOfVertexes](float acc, const zholobov::Polygon& polygon) {
          if (polygon.points.size() == numberOfVertexes) {
            acc += zholobov::calcArea(polygon);
          }
          return acc;
        });
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

  float calcMaxNumberOfVertexes(const zholobov::Polygons& polygons)
  {
    std::vector< float > numOfVertexes;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(numOfVertexes),
        [](const zholobov::Polygon& polygon) {
          return polygon.points.size();
        });

    return *std::max_element(numOfVertexes.begin(), numOfVertexes.end());
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

  float calcMinNumberOfVertexes(const zholobov::Polygons& polygons)
  {
    std::vector< float > numOfVertexes;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(numOfVertexes),
        [](const zholobov::Polygon& polygon) {
          return polygon.points.size();
        });

    return *std::min_element(numOfVertexes.begin(), numOfVertexes.end());
  }

  float calcCountEven(const zholobov::Polygons& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(),
        [](const zholobov::Polygon& polygon) {
          return polygon.points.size() % 2 == 0;
        });
  }

  float calcCountOdd(const zholobov::Polygons& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(),
        [](const zholobov::Polygon& polygon) {
          return polygon.points.size() % 2 != 0;
        });
  }

  float calcCountNumOfVertexes(const zholobov::Polygons& polygons, unsigned long numberOfVertexes)
  {
    return std::count_if(polygons.begin(), polygons.end(),
        [numberOfVertexes](const zholobov::Polygon& polygon) {
          return polygon.points.size() == numberOfVertexes;
        });
  }

  // bool hasIntersection(const zholobov::Point& pt1_1, const zholobov::Point& pt1_2,
  //     const zholobov::Point& pt2_1, const zholobov::Point& pt2_2)
  // {
  //   return false;
  // }

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
    unsigned long numberOfVertexes = std::stoul(param);
    result = calcAreaNumOfVertexes(polygons, numberOfVertexes);
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
  cmdParam["VERTEXES"] = std::bind(calcMaxNumberOfVertexes, std::cref(polygons));
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
  cmdParam["VERTEXES"] = std::bind(calcMinNumberOfVertexes, std::cref(polygons));
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
    unsigned long numberOfVertexes = std::stoul(param);
    result = calcCountNumOfVertexes(polygons, numberOfVertexes);
  }
  output << result;
}

void zholobov::cmdIntersections(std::istream& input, std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  Polygon param;
  if (!(input >> param)) {
    throw std::invalid_argument("INTERSECTIONS failed");
  }
  output << "Not implemented\n";
}

void zholobov::cmdRects(std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  output << "Not implemented\n";
}
