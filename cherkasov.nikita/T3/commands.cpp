#include "commands.hpp"
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <streamGuard.hpp>
#include <map>
#include <functional>

namespace
{
  using namespace cherkasov;
  void printAreaEven(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::vector< Polygon > even_polygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(even_polygons), isEven);
    std::vector< double > areas(even_polygons.size());
    std::transform(even_polygons.begin(), even_polygons.end(), areas.begin(), getArea);
    double area = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << area;
  }

  void printAreaOdd(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::vector< Polygon > odd_polygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(odd_polygons), isOdd);
    std::vector< double > areas(odd_polygons.size());
    std::transform(odd_polygons.begin(), odd_polygons.end(), areas.begin(), getArea);
    double area = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << area;
  }

  void printAreaMean(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No shapes!\n");
    }
    double res = areaMean(polygons);
    out << std::fixed << std::setprecision(1) << res;
  }

  void printAreaVertexes(const std::vector< Polygon >& polygons, std::ostream& out, const std::string& s)
  {
    size_t vert = std::stoull(s);
    if (vert < 3)
    {
      throw std::logic_error("Not enough vertexes\n");
    }
    VertexesCmp cmp{vert};
    std::vector< Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), cmp);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    double totalArea = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << totalArea;
  }

  void printMaxArea(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    auto p = std::max_element(polygons.begin(), polygons.end(), maxArea);
    out << std::fixed << std::setprecision(1);
    out << getArea(*p);
  }

  void printMaxVertexes(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    auto p = std::max_element(polygons.begin(), polygons.end(), maxVertexes);
    out << (*p).points.size();
  }

  void printMinArea(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    auto p = std::min_element(polygons.begin(), polygons.end(), maxArea);
    out << std::fixed << std::setprecision(1);
    out << getArea(*p);
  }

  void printMinVertexes(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    auto p = std::min_element(polygons.begin(), polygons.end(), maxVertexes);
    out << (*p).points.size();
  }

  void printCountEven(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    size_t res = countEven(polygons);
    out << res;
  }

  void printCountOdd(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    size_t res = countOdd(polygons);
    out << res;
  }

  void printCountVertexes(const std::vector< Polygon >& polygons, std::ostream& out, const std::string& s)
  {
    size_t vert = std::stoull(s);
    if (vert < 3)
    {
      throw std::logic_error("Not enough vertexes\n");
    }
    size_t res = countVertexes(polygons, vert);
    out << res;
  }
}

namespace cherkasov
{
  void doAreaComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
  {
    StreamGuard guard(out);
    std::string subcommand;
    in >> subcommand;
    if (!in)
    {
      throw std::logic_error("Invalid input");
    }
    std::map< std::string, std::function< void() > > commands;
    commands["EVEN"] = std::bind(printAreaEven, std::cref(polygons), std::ref(std::cout));
    commands["ODD"] = std::bind(printAreaOdd, std::cref(polygons), std::ref(std::cout));
    commands["MEAN"] = std::bind(printAreaMean, std::cref(polygons), std::ref(std::cout));
    commands["VERTEXES"] = std::bind(printAreaVertexes, std::cref(polygons), std::ref(std::cout), std::cref(subcommand));
    try
    {
      commands.at("VERTEXES")();
    }
    catch (const std::exception &)
    {
      commands.at(subcommand)();
    }
  }

  void doMaxComm(const std::vector< Polygon >& polygons, std::ostream &out, std::istream& in)
  {
    StreamGuard guard(out);
    if (polygons.size() < 1)
    {
      throw std::logic_error("No maximum\n");
    }
    std::string subcommand;
    in >> subcommand;
    if (!in)
    {
      throw std::logic_error("Invalid input");
    }
    std::map< std::string, std::function< void() > > commands;
    commands["AREA"] = std::bind(printMaxArea, std::cref(polygons), std::ref(std::cout));
    commands["VERTEXES"] = std::bind(printMaxVertexes, std::cref(polygons), std::ref(std::cout));
    commands.at(subcommand)();
  }

  void doMinComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
  {
    StreamGuard guard(out);
    if (polygons.size() < 1)
    {
      throw std::logic_error("No minimum!\n");
    }
    std::string subcommand;
    in >> subcommand;
    if (!in)
    {
      throw std::logic_error("Invalid input");
    }
    std::map< std::string, std::function< void() > > commands;
    commands["AREA"] = std::bind(printMinArea, std::cref(polygons), std::ref(std::cout));
    commands["VERTEXES"] = std::bind(printMinVertexes, std::cref(polygons), std::ref(std::cout));
    commands.at(subcommand)();
  }

  void doCountComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
  {
    StreamGuard guard(out);
    std::string subcommand;
    in >> subcommand;
    if (!in)
    {
      throw std::logic_error("Invalid input");
    }
    std::map< std::string, std::function< void() > > commands;
    commands["EVEN"] = std::bind(printCountEven, std::cref(polygons), std::ref(std::cout));
    commands["ODD"] = std::bind(printCountOdd, std::cref(polygons), std::ref(std::cout));
    commands["VERTEXES"] = std::bind(printCountVertexes, std::cref(polygons), std::ref(std::cout), std::cref(subcommand));
    try
    {
      commands.at("VERTEXES")();
    }
    catch (const std::exception&)
    {
      commands.at(subcommand)();
    }
  }

  void doIntersectComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
  {
    Polygon target;
    in >> target;
    if (!in)
    {
      throw std::logic_error("Invalid input");
    }
    out << std::count_if(polygons.begin(), polygons.end(), IntersectCmp{ target });
  }

  void doRightShapesComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
  {
    (void)in;
    size_t count = std::count_if(polygons.begin(), polygons.end(), RightAngleCmp{});
    out << count;
  }
}
