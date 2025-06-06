#include "commands.hpp"
#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <map>
#include <streamGuard.hpp>
#include "polygonOperations.hpp"

namespace
{
  void getAreaEven(std::ostream & out, const std::vector< maslov::Polygon > & polygons)
  {
    maslov::StreamGuard guard(out);
    std::vector< maslov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), maslov::IsEven{});
    std::vector< double > areas;
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), maslov::AreaCalculator{});
    double result = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaOdd(std::ostream & out, const std::vector< maslov::Polygon > & polygons)
  {
    maslov::StreamGuard guard(out);
    std::vector< maslov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), maslov::IsOdd{});
    std::vector< double > areas;
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), maslov::AreaCalculator{});
    double result = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaMean(std::ostream & out, const std::vector< maslov::Polygon > & polygons)
  {
    if (polygons.empty())
    {
      throw std::runtime_error("ERROR: there are no polygons");
    }
    maslov::StreamGuard guard(out);
    std::vector< double > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), maslov::AreaCalculator{});
    double result = std::accumulate(areas.begin(), areas.end(), 0.0) / areas.size();
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaVertexes(std::ostream & out, const std::vector< maslov::Polygon > & polygons, size_t num)
  {
    maslov::StreamGuard guard(out);
    std::vector< maslov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), maslov::SameVertexes{num});
    std::vector< double > areas;
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), maslov::AreaCalculator{});
    double result = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxArea(std::ostream & out, const std::vector< maslov::Polygon > & polygons)
  {
    maslov::StreamGuard guard(out);
    std::vector< double > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), maslov::AreaCalculator{});
    double result = *std::max_element(areas.begin(), areas.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxVertexes(std::ostream & out, const std::vector< maslov::Polygon > & polygons)
  {
    maslov::StreamGuard guard(out);
    auto result = *std::max_element(polygons.begin(), polygons.end(), maslov::VertexesComparator{});
    out << std::fixed << std::setprecision(1) << result.points.size() << '\n';
  }

  void getMinArea(std::ostream & out, const std::vector< maslov::Polygon > & polygons)
  {
    maslov::StreamGuard guard(out);
    std::vector< double > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), maslov::AreaCalculator{});
    double result = *std::min_element(areas.begin(), areas.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMinVertexes(std::ostream & out, const std::vector< maslov::Polygon > & polygons)
  {
    maslov::StreamGuard guard(out);
    auto result = *std::min_element(polygons.begin(), polygons.end(), maslov::VertexesComparator{});
    out << std::fixed << std::setprecision(1) << result.points.size() << '\n';
  }
}

void maslov::getArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::string subcmd;
  in >> subcmd;
  if (subcmd == "EVEN")
  {
    getAreaEven(out, polygons);
  }
  else if (subcmd == "ODD")
  {
    getAreaOdd(out, polygons);
  }
  else if (subcmd == "MEAN")
  {
    getAreaMean(out, polygons);
  }
  else
  {
    size_t num = std::stoul(subcmd);
    if (num < 3)
    {
      throw std::logic_error("ERROR: the polygon consists of at least 3 vertices");
    }
    getAreaVertexes(out, polygons, num);
  }
}

void maslov::getMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("ERROR: there are no polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMaxArea, std::ref(out), std::cref(polygons));
  subcmds["VERTEXES"] = std::bind(getMaxVertexes, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  subcmds.at(subcmd)();
}

void maslov::getMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("ERROR: there are no polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMinArea, std::ref(out), std::cref(polygons));
  subcmds["VERTEXES"] = std::bind(getMinVertexes, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  subcmds.at(subcmd)();
}

void maslov::getCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::string subcmd;
  in >> subcmd;
  if (subcmd == "EVEN")
  {
    out << std::count_if(polygons.begin(), polygons.end(), IsEven{}) << '\n';
  }
  else if (subcmd == "ODD")
  {
    out << std::count_if(polygons.begin(), polygons.end(), IsOdd{}) << '\n';
  }
  else
  {
    size_t num = std::stoul(subcmd);
    if (num < 3)
    {
      throw std::logic_error("ERROR: the polygon consists of at least 3 vertices");
    }
    out << std::count_if(polygons.begin(), polygons.end(), SameVertexes{num}) << '\n';
  }
}

void maslov::getEcho(std::istream & in, std::ostream & out, std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("ERROR: there are no polygons");
  }
  Polygon inPolygon;
  in >> inPolygon;
  size_t count = std::count_if(polygons.begin(), polygons.end(), SamePolygon{inPolygon});
  std::vector< Polygon > result;
  std::vector< int > temp;
  std::transform(polygons.begin(), polygons.end(), temp.begin(), EchoInserter{result, inPolygon});
  polygons = std::move(result);
  out << count << '\n';
}

void maslov::getRects(std::ostream & out, const std::vector< Polygon > & polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), IsRectangle{}) << '\n';
}
