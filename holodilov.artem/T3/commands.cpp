#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <map>
#include <numeric>
#include "polygon.hpp"
#include "subcommands.hpp"

namespace
{
  using holodilov::VecPolygons;

  bool compareAreaWithTarget(const holodilov::Polygon& polygon, double targetArea)
  {
    return polygon.getArea() < targetArea;
  }

  VecPolygons accumulateEcho(VecPolygons vecPolygons, const holodilov::Polygon& polygon, const holodilov::Polygon& targetPolygon)
  {
    vecPolygons.push_back(polygon);
    if (polygon == targetPolygon)
    {
      vecPolygons.push_back(polygon);
    }
    return vecPolygons;
  }
}

void holodilov::commands::area(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  std::string subcommand;
  is >> subcommand;
  try
  {
    int amountVertexes = std::stoi(subcommand);
    if (amountVertexes < 3)
    {
      throw std::logic_error("Error: vertexes amount must be >= 3.");
    }
    subcommands::areaAmount(os, vecPolygons, amountVertexes);
  }
  catch (const std::invalid_argument&)
  {
    std::map< std::string, std::function< void() > > cmds;
    cmds["EVEN"] = std::bind(subcommands::areaEven, std::ref(os), std::cref(vecPolygons));
    cmds["ODD"] = std::bind(subcommands::areaOdd, std::ref(os), std::cref(vecPolygons));
    cmds["MEAN"] = std::bind(subcommands::areaMean, std::ref(os), std::cref(vecPolygons));
    cmds.at(subcommand)();
  }
}

void holodilov::commands::min(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  if (vecPolygons.empty())
  {
    throw std::logic_error("Error: no polygons to handle.");
  }

  std::string subcommand;
  is >> subcommand;

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(subcommands::minArea, std::ref(os), std::cref(vecPolygons));
  cmds["VERTEXES"] = std::bind(subcommands::minVertexes, std::ref(os), std::cref(vecPolygons));
  cmds.at(subcommand)();
}

void holodilov::commands::max(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  if (vecPolygons.empty())
  {
    throw std::logic_error("Error: no polygons to handle.");
  }

  std::string subcommand;
  is >> subcommand;

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(subcommands::maxArea, std::ref(os), std::cref(vecPolygons));
  cmds["VERTEXES"] = std::bind(subcommands::maxVertexes, std::ref(os), std::cref(vecPolygons));
  cmds.at(subcommand)();
}

void holodilov::commands::count(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  std::string subcommand;
  is >> subcommand;
  try
  {
    int amountVertexes = std::stoi(subcommand);
    if (amountVertexes < 3)
    {
      throw std::logic_error("Error: vertexes amount must be >= 3.");
    }
    subcommands::countAmount(os, vecPolygons, amountVertexes);
  }
  catch (const std::invalid_argument&)
  {
    std::map< std::string, std::function< void() > > cmds;
    cmds["EVEN"] = std::bind(subcommands::countEven, std::ref(os), std::cref(vecPolygons));
    cmds["ODD"] = std::bind(subcommands::countOdd, std::ref(os), std::cref(vecPolygons));
    cmds.at(subcommand)();
  }
}

void holodilov::commands::echo(std::istream& is, std::ostream& os, VecPolygons& vecPolygons)
{
  Polygon targetPolygon;
  is >> targetPolygon;
  if (!is || (is.peek() != '\n'))
  {
    throw std::logic_error("Error: invalid polygon argument.");
  }

  const size_t amountDuplicates = std::count(vecPolygons.begin(), vecPolygons.end(), targetPolygon);

  auto accumulateBound = std::bind(accumulateEcho, std::placeholders::_1, std::placeholders::_2, std::cref(targetPolygon));
  vecPolygons = std::accumulate(vecPolygons.begin(), vecPolygons.end(), VecPolygons(0), accumulateBound);
  os << amountDuplicates;
}

void holodilov::commands::lessArea(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  Polygon targetPolygon;
  is >> targetPolygon;
  if (!is || (is.peek() != '\n'))
  {
    throw std::logic_error("Error: invalid polygon argument.");
  }

  auto areaComparator = std::bind(compareAreaWithTarget, std::placeholders::_1, targetPolygon.getArea());
  size_t result = std::count_if(vecPolygons.begin(), vecPolygons.end(), areaComparator);
  os << result;
}
