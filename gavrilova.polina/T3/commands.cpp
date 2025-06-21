#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <map>
#include <numeric>
#include <sstream>
#include "utils.hpp"

namespace {
  double calcArea(const gavrilova::Polygon& polygon)
  {
    return polygon.area();
  }

  double calcAreaOdd(const gavrilova::Polygon& polygon)
  {
    return polygon.isOdd() ? calcArea(polygon) : 0.0;
  }

  double calcAreaEven(const gavrilova::Polygon& polygon)
  {
    return polygon.isEven() ? calcArea(polygon) : 0.0;
  }

  double calcAreaNum(const gavrilova::Polygon& polygon, size_t num)
  {
    return (polygon.points.size() == num) ? calcArea(polygon) : 0.0;
  }

  bool minAreaComp(const gavrilova::Polygon& polygon_1, const gavrilova::Polygon& polygon_2)
  {
    return polygon_1.area() < polygon_2.area();
  }

  bool minVertexesComp(const gavrilova::Polygon& polygon_1, const gavrilova::Polygon& polygon_2)
  {
    return polygon_1.points.size() < polygon_2.points.size();
  }

  const gavrilova::Polygon& findMinPolygon(const std::vector< gavrilova::Polygon >& polygons,
      const std::function< bool(const gavrilova::Polygon&, const gavrilova::Polygon&) >& comp)
  {
    if (polygons.empty()) {
      throw std::runtime_error("");
    }
    return *std::min_element(polygons.begin(), polygons.end(), comp);
  }

  const gavrilova::Polygon& findMaxPolygon(const std::vector< gavrilova::Polygon >& polygons,
      const std::function< bool(const gavrilova::Polygon&, const gavrilova::Polygon&) >& comp)
  {
    if (polygons.empty()) {
      throw std::runtime_error("");
    }
    return *std::max_element(polygons.begin(), polygons.end(), comp);
  }

  size_t calcCountOdd(const gavrilova::Polygon& polygon)
  {
    return polygon.isOdd() ? 1 : 0;
  }

  size_t calcCountEven(const gavrilova::Polygon& polygon)
  {
    return polygon.isEven() ? 1 : 0;
  }

  double calcCountNum(const gavrilova::Polygon& polygon, size_t num)
  {
    return (polygon.points.size() == num) ? 1 : 0.0;
  }
}

void gavrilova::processArea(const std::vector< Polygon >& polygons,
    const std::vector< std::string >& commands, std::ostream& out)
{
  if (commands.size() < 2) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto& subcommand = commands.at(1);
  double area = 0.0;

  static auto apply_command = std::bind(
      gavrilova::applyCommand< std::vector< Polygon >, double, std::function< double(const Polygon&) >, std::plus<> >,
      std::placeholders::_1,
      0.0,
      std::placeholders::_2,
      std::plus<>());

  if (subcommand == "MEAN") {
    if (polygons.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    area = apply_command(polygons, calcArea);
    area = area / static_cast< double >(polygons.size());
  } else if (subcommand == "ODD") {
    area = apply_command(polygons, calcAreaOdd);
  } else if (subcommand == "EVEN") {
    area = apply_command(polygons, calcAreaEven);
  } else {
    size_t num = 0;
    try {
      num = std::stoll(subcommand);
      if (num < 3) {
        out << "<INVALID COMMAND>\n";
        return;
      }
      area = apply_command(
          polygons,
          std::bind(
              calcAreaNum,
              std::placeholders::_1,
              num));
    } catch (const std::exception&) {
      throw std::runtime_error("Invalid subcommand for AREA");
    }
  }
  out << std::fixed << std::setprecision(1) << area << "\n";
}

void gavrilova::processMinMax(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands, std::ostream& out)
{
  if (commands.size() < 2) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  static std::map< std::string, std::function< const Polygon&(const std::vector< Polygon >&,
                                    const std::function< bool(const Polygon&, const Polygon&) >&) > >
      min_max_map = {
          {"MIN", findMinPolygon},
          {"MAX", findMaxPolygon}};

  auto& command = commands.at(0);
  auto& subcommand = commands.at(1);

  try {
    if (subcommand == "AREA") {
      auto val = min_max_map.at(command)(polygons, minAreaComp);
      out << std::fixed << std::setprecision(1) << val.area() << "\n";
    } else if (subcommand == "VERTEXES") {
      auto val = min_max_map.at(command)(polygons, minVertexesComp);
      out << val.points.size() << "\n";
    } else {
      out << "<INVALID COMMAND>\n";
      return;
    }
  } catch (const std::exception&) {
    out << "<INVALID COMMAND>\n";
    return;
  }
}

void gavrilova::processCount(const std::vector< Polygon >& polygons,
    const std::vector< std::string >& commands, std::ostream& out)
{
  if (commands.size() < 2) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto& subcommand = commands.at(1);
  static auto apply_command = std::bind(
      gavrilova::applyCommand< std::vector< Polygon >, size_t, std::function< size_t(const Polygon&) >, std::plus<> >,
      std::placeholders::_1,
      static_cast< size_t >(0),
      std::placeholders::_2,
      std::plus<>());

  size_t count = 0;

  if (subcommand == "ODD") {
    count = apply_command(polygons, calcCountOdd);
  } else if (subcommand == "EVEN") {
    count = apply_command(polygons, calcCountEven);
  } else {
    size_t num = 0;
    try {
      num = std::stoll(subcommand);
      if (num < 3) {
        out << "<INVALID COMMAND>\n";
        return;
      }
      count = apply_command(
          polygons,
          std::bind(
              calcCountNum,
              std::placeholders::_1,
              num));
    } catch (const std::exception&) {
      throw std::runtime_error("Invalid subcommand for COUNT");
    }
  }

  out << count << "\n";
}

void gavrilova::processPerms(const std::vector< Polygon >& polygons,
    const std::vector< std::string >& commands, std::ostream& out)
{
  if (commands[1] < "3") {
    out << "<INVALID COMMAND>\n";
    return;
  }

  struct AccumulateJoin {
    std::string operator()(const std::string& str_1, const std::string& str_2) const
    {
      return str_1 + " " + str_2;
    }
  };

  auto polygon_str = std::accumulate(
      commands.begin() + 1,
      commands.end(),
      std::string(""),
      AccumulateJoin());

  std::istringstream iss(polygon_str);
  Polygon ref_polygon;

  if (!(iss >> ref_polygon && iss.eof())) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::sort(ref_polygon.points.begin(), ref_polygon.points.end());

  struct PointsEqual {
    const Polygon& ref_polygon;
    PointsEqual(const Polygon& polygon):
      ref_polygon(polygon)
    {}
    bool operator()(const Polygon& polygon) const
    {
      auto points = polygon.points;
      std::sort(points.begin(), points.end());
      return points == ref_polygon.points;
    }
  };

  size_t count = std::count_if(
      polygons.begin(),
      polygons.end(),
      PointsEqual(ref_polygon));

  out << count << "\n";
}

void gavrilova::processLessArea(const std::vector< Polygon >& polygons,
    const std::vector< std::string >& commands, std::ostream& out)
{
  if (commands.size() < 3) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  struct AccumulateJoin {
    std::string operator()(const std::string& str_1, const std::string& str_2) const
    {
      return str_1 + " " + str_2;
    }
  };

  auto polygon_str = std::accumulate(
      commands.begin() + 1,
      commands.end(),
      std::string(""),
      AccumulateJoin());

  std::istringstream iss(polygon_str);
  Polygon ref_polygon;

  if (!(iss >> ref_polygon && iss.eof())) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (ref_polygon.area() == 0.0) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  struct AreaLess {
    const Polygon& ref_polygon;
    AreaLess(const Polygon& polygon):
      ref_polygon(polygon)
    {}
    bool operator()(const Polygon& polygon) const
    {
      return polygon.area() < ref_polygon.area();
    }
  };

  size_t count = std::count_if(
      polygons.begin(),
      polygons.end(),
      AreaLess(ref_polygon));

  out << count << "\n";
}
