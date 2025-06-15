#include <iostream>
#include <sstream>
#include <iterator>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>
#include <functional>
#include "commands.h"

using namespace orlova;

void getPolygons(std::istream& in, std::vector< Polygon >& polygons)
{
  while (!in.eof())
  {
    std::copy(std::istream_iterator< Polygon >(in), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
    if (!in)
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

void area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< double(const std::vector<Polygon>&) > > subcmds;
  subcmds["even"] = std::bind(areaEven, std::cref(polygons));
  subcmds["odd"] = std::bind(areaOdd, std::cref(polygons));
  subcmds["mean"] = std::bind(areaMean, std::cref(polygons));
  auto it = subcmds.find(subcommand);
  if (it != subcmds.end())
  {
    out << it->second(polygons);
  }
  else
  {
    std::stringstream iss(subcommand);
    size_t numOfVertexes = 0;

    if (!(iss >> numOfVertexes) || numOfVertexes < 3)
    {
      throw std::logic_error("<WRONG SUBCOMMAND>");
    }

    out << areaNum(polygons, numOfVertexes);
  }
}

void max(const std::vector< Polygon >&, std::istream&, std::ostream&);
double maxArea(const std::vector< Polygon >&);
size_t maxVertexes(const std::vector< Polygon >&);

void min(const std::vector< Polygon >&, std::istream&, std::ostream&);
double minArea(const std::vector< Polygon >&);
size_t minVertexes(const std::vector< Polygon >&);

void count(const std::vector< Polygon >&, std::istream&, std::ostream&);
size_t countEven(const std::vector< Polygon >&);
size_t countOdd(const std::vector< Polygon >&);
size_t countNum(const std::vector< Polygon >&, size_t);
