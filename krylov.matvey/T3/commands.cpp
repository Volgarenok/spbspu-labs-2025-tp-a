#include "commands.hpp"
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <streamGuard.hpp>
#include "geometry.hpp"

namespace
{
  using namespace krylov;
  void printAreaEven(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::vector< Polygon > even_polygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(even_polygons), isEven);
    std::vector< double > areas(polygons.size());
    std::transform(even_polygons.begin(), even_polygons.end(), areas.begin(), getArea);
    double area = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << area;
  }

  void printAreaOdd(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::vector< Polygon > odd_polygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(odd_polygons), isOdd);
    std::vector< double > areas(polygons.size());
    std::transform(odd_polygons.begin(), odd_polygons.end(), areas.begin(), getArea);
    double area = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << area;
  }

  void printAreaMean(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No shapes!\n");
    }
    double res = areaMean(polygons);
    out << res;
  }

  void printAreaVertexes(const std::vector< Polygon >& polygons, std::ostream& out, const std::string& s)
  {
    using namespace std::placeholders;

    size_t vert = std::stoull(s);
    if (vert < 3)
    {
      throw std::logic_error("Not enough vertexes\n");
    }
    VertexesCmp cmp{vert};
    std::vector< Polygon > filtered(polygons.size());
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), cmp);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    double totalArea = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << totalArea;
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

  size_t getMaxseq(const Polygon& polygon, const Polygon& pattern)
  {
    size_t currentSeq = 0;
    size_t maxSeq = 0;
    if (isPolygonsEqual(polygon, pattern))
    {
      currentSeq++;
      maxSeq = std::max(maxSeq, currentSeq);
    }
    else
    {
      currentSeq = 0;
    }
    return maxSeq;
  }

  struct PointInPolygon
  {
    const Polygon& polygon;

    PointInPolygon(const Polygon& p):
      polygon(p)
    {}

    bool operator()(const Point& pt) const
    {
      return std::find(polygon.points.begin(), polygon.points.end(), pt) != polygon.points.end();
    }
  };

  struct TriangleInPolygonChecker
  {
    const Polygon& triangle;

    TriangleInPolygonChecker(const Polygon& t):
      triangle(t)
    {}

    bool operator()(const Polygon& poly) const
    {
      PointInPolygon check(poly);
      return std::all_of(triangle.points.begin(), triangle.points.end(), check);
    }
  };
}

void krylov::doIsCuttedComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
{
  Polygon triangle;
  in >> triangle;
  if (triangle.points.size() != 3)
  {
    std::cout << triangle.points.size();
    throw std::logic_error("Input must be a triangle");
  }
  TriangleInPolygonChecker checker(triangle);
  size_t count = std::count_if(polygons.begin(), polygons.end(), checker);
  out << count;
}


void krylov::doAreaComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  commands["EVEN"] = std::bind(printAreaEven, std::cref(polygons), std::ref(std::cout));
  commands["ODD"] = std::bind(printAreaOdd, std::cref(polygons), std::ref(std::cout));
  commands["MEAN"] = std::bind(printAreaMean, std::cref(polygons), std::ref(std::cout));
  commands["VERTEXES"] = std::bind(printAreaVertexes, std::cref(polygons), std::ref(std::cout), std::cref(subcommand));
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

void krylov::doMaxComm(const std::vector< Polygon >& polygons, std::ostream &out, std::istream& in)
{
  StreamGuard guard(out);
  if (polygons.size() < 1)
  {
    throw std::logic_error("No maximum\n");
  }
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(printMaxArea, std::cref(polygons), std::ref(std::cout));
  commands["VERTEXES"] = std::bind(printMaxVertexes, std::cref(polygons), std::ref(std::cout));
  commands.at(subcommand)();
}

void krylov::doMinComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
{
  StreamGuard guard(out);
  if (polygons.size() < 1)
  {
    throw std::logic_error("No minimum!\n");
  }
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(printMinArea, std::cref(polygons), std::ref(std::cout));
  commands["VERTEXES"] = std::bind(printMinVertexes, std::cref(polygons), std::ref(std::cout));
  commands.at(subcommand)();
}

void krylov::doCountComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  commands["EVEN"] = std::bind(printCountEven, std::cref(polygons), std::ref(std::cout));
  commands["ODD"] = std::bind(printCountOdd, std::cref(polygons), std::ref(std::cout));
  commands["VERTEXES"] = std::bind(printCountVertexes, std::cref(polygons), std::ref(std::cout), std::cref(subcommand));
  try
  {
    commands["VERTEXES"]();
  }
  catch (const std::exception&)
  {
    commands.at(subcommand)();
  }
}

void krylov::doMaxseqComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
{
  using namespace std::placeholders;
  Polygon pattern;
  in >> pattern;
  if (std::count(polygons.begin(), polygons.end(), pattern) == 0)
  {
    throw std::logic_error("No similar polygons");
  }
  std::vector< size_t > seqOfPolygons(polygons.size());
  std::transform(polygons.begin(), polygons.end(), seqOfPolygons.begin(), std::bind(getMaxseq, _1, std::cref(pattern)));
  out << *std::max_element(seqOfPolygons.begin(), seqOfPolygons.end());
}

void krylov::doIntersectComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
{
  Polygon target;
  in >> target;
  out << std::count_if(polygons.begin(), polygons.end(), IntersectCmp{ target });
}
