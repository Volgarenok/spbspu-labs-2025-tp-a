#include "commands.hpp"
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <stream_guard.hpp>
#include "geom.hpp"

namespace
{
  using namespace abramov;
  using CommandDict = std::map< std::string, std::function< void() > >;

  void printAreaEven(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    std::vector< Polygon > even_polygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(even_polygons), isEven);
    std::vector< double > areas(polygons.size());
    std::transform(even_polygons.begin(), even_polygons.end(), areas.begin(), getArea);
    double area = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << area;
  }

  void printAreaOdd(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    std::vector< Polygon > odd_polygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(odd_polygons), isOdd);
    std::vector< double > areas(polygons.size());
    std::transform(odd_polygons.begin(), odd_polygons.end(), areas.begin(), getArea);
    double area = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << area;
  }

  void printAreaMean(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("Not enough shapes\n");
    }
    double res = areaMean(polygons);
    out << res;
  }

  double getAreaIfGoodNumOfVert(const Polygon &p, size_t vert)
  {
    VertexesCmp cmp{ vert };
    if (cmp(p))
    {
      return getArea(p);
    }
    return 0.0;
  }

  void printAreaVertexes(const std::vector< Polygon > &polygons, std::ostream &out, const std::string &s)
  {
    using namespace std::placeholders;

    size_t vert = std::stoull(s);
    if (vert < 3)
    {
      throw std::logic_error("Too less vertexes\n");
    }
    std::vector< double > areas(polygons.size());
    auto f = std::bind(getAreaIfGoodNumOfVert, _1, vert);
    std::transform(polygons.begin(), polygons.end(), areas.begin(), f);
    double area = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << area;
  }

  void getAreaCommands(CommandDict &commands, const std::vector< Polygon > &polygons, const std::string &s)
  {
    commands["EVEN"] = std::bind(printAreaEven, std::cref(polygons), std::ref(std::cout));
    commands["ODD"] = std::bind(printAreaOdd, std::cref(polygons), std::ref(std::cout));
    commands["MEAN"] = std::bind(printAreaMean, std::cref(polygons), std::ref(std::cout));
    commands["VERTEXES"] = std::bind(printAreaVertexes, std::cref(polygons), std::ref(std::cout), std::cref(s));
  }

  void printMaxArea(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    auto p = std::max_element(polygons.begin(), polygons.end(), maxArea);
    out << std::fixed << std::setprecision(1);
    out << getArea(*p);
  }

  void printMaxVertexes(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    auto p = std::max_element(polygons.begin(), polygons.end(), maxVertexes);
    out << (*p).points.size();
  }

  void getMaxCommands(CommandDict &commands, const std::vector< Polygon > &polygons)
  {
    commands["AREA"] = std::bind(printMaxArea, std::cref(polygons), std::ref(std::cout));
    commands["VERTEXES"] = std::bind(printMaxVertexes, std::cref(polygons), std::ref(std::cout));
  }

  void printMinArea(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    auto p = std::min_element(polygons.begin(), polygons.end(), maxArea);
    out << std::fixed << std::setprecision(1);
    out << getArea(*p);
  }

  void printMinVertexes(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    auto p = std::min_element(polygons.begin(), polygons.end(), maxVertexes);
    out << (*p).points.size();
  }

  void getMinCommands(CommandDict &commands, const std::vector< Polygon > &polygons)
  {
    commands["AREA"] = std::bind(printMinArea, std::cref(polygons), std::ref(std::cout));
    commands["VERTEXES"] = std::bind(printMinVertexes, std::cref(polygons), std::ref(std::cout));
  }

  void printCountEven(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    size_t res = countEven(polygons);
    out << res;
  }

  void printCountOdd(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    size_t res = countOdd(polygons);
    out << res;
  }

  void printCountVertexes(const std::vector< Polygon > &polygons, std::ostream &out, const std::string &s)
  {
    size_t vert = std::stoull(s);
    if (vert < 3)
    {
      throw std::logic_error("Too less vertexes\n");
    }
    size_t res = countVertexes(polygons, vert);
    out << res;
  }

  void getCountCommands(CommandDict &commands, const std::vector< Polygon > &polygons, const std::string &s)
  {
    commands["EVEN"] = std::bind(printCountEven, std::cref(polygons), std::ref(std::cout));
    commands["ODD"] = std::bind(printCountOdd, std::cref(polygons), std::ref(std::cout));
    commands["VERTEXES"] = std::bind(printCountVertexes, std::cref(polygons), std::ref(std::cout), std::cref(s));
  }
}

void abramov::getCommands(std::map< std::string, std::function< void() > > &commands, std::vector< Polygon > &polygons)
{
  commands["AREA"] = std::bind(doAreaComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["MAX"] = std::bind(doMaxComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["MIN"] = std::bind(doMinComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["COUNT"] = std::bind(doCountComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["RMECHO"] = std::bind(doRmechoComm, std::ref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["PERMS"] = std::bind(doPermsComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
}

void abramov::doAreaComm(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  getAreaCommands(commands, polygons, subcommand);
  out << std::fixed << std::setprecision(1);
  try
  {
    commands["VERTEXES"]();
  }
  catch (const std::exception &)
  {
    commands.at(subcommand)();
  }
}

void abramov::doMaxComm(const std::vector< Polygon> &polygons, std::ostream &out, std::istream &in)
{
  StreamGuard guard(out);
  if (polygons.size() < 1)
  {
    throw std::logic_error("Can not find max\n");
  }
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  getMaxCommands(commands, polygons);
  commands.at(subcommand)();
}

void abramov::doMinComm(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  StreamGuard guard(out);
  if (polygons.size() < 1)
  {
    throw std::logic_error("Can not find min\n");
  }
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  getMinCommands(commands, polygons);
  commands.at(subcommand)();
}

void abramov::doCountComm(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  getCountCommands(commands, polygons, subcommand);
  try
  {
    commands["VERTEXES"]();
  }
  catch (const std::exception &)
  {
    commands.at(subcommand)();
  }
}

void abramov::doRmechoComm(std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  using namespace std::placeholders;

  Polygon pattern;
  in >> pattern;
  if (!in)
  {
    throw std::logic_error("Fail to read\n");
  }
  auto f = std::bind(isPolygonsEqualToExample, _1, _2, pattern);
  auto del = std::unique(polygons.begin(), polygons.end(), f);
  size_t diff = polygons.size();
  polygons.erase(del, polygons.end());
  diff -= polygons.size();
  out << diff;
}

void abramov::doPermsComm(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  Polygon pattern;
  in >> pattern;
  if (!in)
  {
    throw std::logic_error("Fail to read\n");
  }

  struct permutationsComp
  {
    const Polygon &ex;

    permutationsComp(const Polygon &p):
      ex(p)
    {}
    bool operator()(const Polygon &p) const
    {
      return ex.points.size() == p.points.size() && std::is_permutation(p.points.begin(), p.points.end(), ex.points.begin());
    }
  };

  permutationsComp cmp(pattern);
  size_t count = std::count_if(polygons.begin(), polygons.end(), cmp);
  out << count;
}
