#include "printCmd.hpp"
#include <map>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <streamGuard.hpp>

namespace
{
  bool isOdd(const dribas::Polygon& plg)
  {
    return plg.points.size() % 2;
  }

  bool isEven(const dribas::Polygon& plg)
  {
    return !isOdd(plg);
  }

  bool isNumOfVertex(const dribas::Polygon& plg, size_t vertx)
  {
    return plg.points.size() == vertx;
  }

  template< typename Predicate >
  void printAreaPredicate(const std::vector< dribas::Polygon >& plg, std::ostream& out, Predicate predicate)
  {
    std::vector< dribas::Polygon > temp;
    std::copy_if(plg.cbegin(), plg.cend(), std::back_inserter(temp), predicate);

    std::vector< double > areas;
    std::transform(temp.begin(), temp.end(), std::back_inserter(areas), dribas::getPolygonArea);

    double total = std::accumulate(areas.begin(), areas.end(), 0.0);
    dribas::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << total;
  }

  bool areaCompare(const dribas::Polygon& lhs, const dribas::Polygon& rhs)
  {
    return dribas::getPolygonArea(lhs) < dribas::getPolygonArea(rhs);
  }

  bool vertexCompare(const dribas::Polygon& lhs, const dribas::Polygon& rhs)
  {
    return lhs.points.size() < rhs.points.size();
  }

  void printNumofVertex(const std::vector< dribas::Polygon >& plg, size_t vertex, std::ostream& out)
  {
    printAreaPredicate(plg, out, std::bind(isNumOfVertex, std::placeholders::_1, vertex));
  }

  template< typename Compare >
  void printVertexMax(const std::vector< dribas::Polygon >& plgs, std::ostream& out, Compare compare)
  {
    auto polygon = *std::max_element(plgs.begin(), plgs.end(), compare);
    out << polygon.points.size();
  }

  template< typename Compare >
  void printAreaMax(const std::vector< dribas::Polygon >& plgs, std::ostream& out, Compare compare)
  {
    auto polygon = *std::max_element(plgs.begin(), plgs.end(), compare);
    dribas::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << dribas::getPolygonArea(polygon);
  }

  template< typename Compare >
  void printVertexMin(const std::vector< dribas::Polygon >& plgs, std::ostream& out, Compare compare)
  {
    auto polygon = *std::min_element(plgs.begin(), plgs.end(), compare);
    dribas::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << dribas::getPolygonArea(polygon);
  }

  template< typename Compare >
  void printAreaMin(const std::vector< dribas::Polygon >& plgs, std::ostream& out, Compare compare)
  {
    auto polygon = *std::min_element(plgs.begin(), plgs.end(), compare);
    out << dribas::getPolygonArea(polygon);
  }

  template< typename Predicate >
  void printCountPredicate(const std::vector< dribas::Polygon >& plg, std::ostream& out, Predicate predicate)
  {
    out << std::count_if(plg.cbegin(), plg.cend(), predicate);
  }

  void printCountNumofVertex(const std::vector< dribas::Polygon >& plg, size_t vertex, std::ostream& out)
  {
    printCountPredicate(plg, out, std::bind(isNumOfVertex, std::placeholders::_1, vertex));
  }

  struct AngleTriangleCreator
  {
    const std::vector< dribas::Point >& polygonPoints;
    size_t numPoints;

    AngleTriangleCreator(const dribas::Polygon& plg):
      polygonPoints(plg.points),
      numPoints(plg.points.size())
    {}

    std::vector< dribas::Point > operator()(size_t i) const
    {
      size_t prev = (i + numPoints - 1) % numPoints;
      size_t curr = i;
      size_t next = (i + 1) % numPoints;
      return {polygonPoints[prev], polygonPoints[curr], polygonPoints[next]};
    }
  };

  struct IsRightAngleTriangle
  {
    bool operator()(const std::vector< dribas::Point >& anglePoints) const
    {
      if (anglePoints.size() != 3) {
        return false;
      }
      const dribas::Point& a = anglePoints[0];
      const dribas::Point& b = anglePoints[1];
      const dribas::Point& c = anglePoints[2];

      double ba_x = a.x - b.x;
      double ba_y = a.y - b.y;
      double bc_x = c.x - b.x;
      double bc_y = c.y - b.y;

      double dot = ba_x * bc_x + ba_y * bc_y;
      return std::abs(dot) < 0.0004;
    }
  };

  bool hasPlgRightAngle(const dribas::Polygon& plg)
  {
    if (plg.points.size() < 3) {
      return false;
    }
    AngleTriangleCreator creator(plg);
    std::vector< size_t > indices(plg.points.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::vector< std::vector< dribas::Point > > angleTriangles;
    angleTriangles.reserve(indices.size());
    std::transform(indices.begin(), indices.end(), std::back_inserter(angleTriangles), creator);

    IsRightAngleTriangle isRightAngle;
    return std::any_of(angleTriangles.begin(), angleTriangles.end(), isRightAngle);
  }

  void printAreaMean(const std::vector< dribas::Polygon >& plg, std::ostream& out)
  {
    if (!plg.size()) {
      throw std::invalid_argument("No polygon found");
    }
    std::vector< double > areas;
    std::transform(plg.begin(), plg.end(), std::back_inserter(areas), dribas::getPolygonArea);

    dribas::StreamGuard guard(out);
    double total = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << total / plg.size();
  }
}

void dribas::printArea(const std::vector< Polygon >& plg, std::istream& in, std::ostream& out)
{
  std::map< std::string, std::function< void() > > cmds;
  cmds["EVEN"] = std::bind(printAreaPredicate< decltype(isEven) >, std::cref(plg), std::ref(out), std::cref(isEven));
  cmds["ODD"] = std::bind(printAreaPredicate< decltype(isOdd) >, std::cref(plg), std::ref(out), std::cref(isOdd));
  cmds["MEAN"] = std::bind(printAreaMean, std::cref(plg), std::ref(out));

  std::string command;
  in >> command;
  try {
    int num = std::stoi(command);
    if (num < 3) {
      throw std::invalid_argument("");
    }
    printNumofVertex(plg, num, out);
  } catch (std::exception&) {
    cmds.at(command)();
  }
}

void dribas::printMax(const std::vector< Polygon >& plg, std::istream& in, std::ostream& out)
{
  if (!plg.size()) {
    throw std::invalid_argument("no figure found");
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(printAreaMax< decltype(areaCompare) >, std::cref(plg), std::ref(out), areaCompare);
  cmds["VERTEXES"] = std::bind(printVertexMax< decltype(vertexCompare) >, std::cref(plg), std::ref(out), vertexCompare);

  std::string command;
  in >> command;
  cmds.at(command)();
}

void dribas::printMin(const std::vector< Polygon >& plg, std::istream& in, std::ostream& out)
{
  if (!plg.size()) {
    throw std::invalid_argument("no figure found");
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(printAreaMin< decltype(areaCompare) >, std::cref(plg), std::ref(out), areaCompare);
  cmds["VERTEXES"] = std::bind(printVertexMin< decltype(vertexCompare) >, std::cref(plg), std::ref(out), vertexCompare);
  std::string command;
  in >> command;
  cmds.at(command)();
}

void dribas::printCount(const std::vector< Polygon >& plg, std::istream& in , std::ostream& out)
{
  std::map< std::string, std::function< void() > > cmds;
  cmds["EVEN"] = std::bind(printCountPredicate< decltype(isEven) >, std::cref(plg), std::ref(out), std::cref(isEven));
  cmds["ODD"] = std::bind(printCountPredicate< decltype(isOdd) >, std::cref(plg), std::ref(out), std::cref(isOdd));
  std::string command;
  in >> command;
  try {
    int num = std::stoi(command);
    if (num < 3) {
      throw std::invalid_argument("");
    }
    printCountNumofVertex(plg, num, out);
  } catch (std::exception&) {
    cmds.at(command)();
  }
}

void dribas::printLessArea(const std::vector< Polygon >& plg, std::istream& in , std::ostream& out)
{
  Polygon temp;
  in >> temp;
  if (!in || in.peek() != '\n') {
    throw std::invalid_argument("Invalid argument for less are polygon");
  }
  printCountPredicate(plg, out, std::bind(areaCompare, std::placeholders::_1, temp));
}

void dribas::printRightShapes(const std::vector< Polygon >& plg, std::ostream& out)
{
  out << std::count_if(plg.begin(), plg.end(), hasPlgRightAngle);
}
