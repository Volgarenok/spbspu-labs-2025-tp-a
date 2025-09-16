#include "commands.hpp"
#include <algorithm>
#include <format_guard.hpp>
#include <iomanip>
#include "areaCommands.hpp"
#include "countCommands.hpp"
#include "maxCommands.hpp"
#include "minCommands.hpp"
#include "extraCommands.hpp"
#include "extraPerimeterCommands.hpp"

void pilugina::areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  FormatGuard g(out);
  out << std::fixed << std::setprecision(1) << calcAreas(polys, in);
}

void pilugina::maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  printMax(polys, in, out);
}

void pilugina::minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  printMin(polys, in, out);
}

void pilugina::countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  out << getCount(polys, in);
}

void pilugina::echoCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polys)
{
  Polygon ref;
  in >> ref;
  if (in)
  {
    out << echo(polys, ref);
  }
  else
  {
    throw std::invalid_argument("wrong polygon");
  }
}

void pilugina::intersectsCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  Polygon ref;
  in >> ref;
  if (in)
  {
    out << std::count_if(polys.cbegin(), polys.cend(), IntersectsWith(ref));
  }
  else
  {
    throw std::invalid_argument("wrong polygon");
  }
}

void pilugina::permsCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  Polygon ref;
  in >> ref;
  if (in)
  {
    out << std::count_if(polys.cbegin(), polys.cend(), PermutationPredicate(ref));
  }
  else
  {
    throw std::invalid_argument("wrong polygon");
  }
}

void pilugina::perimeterCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  FormatGuard g(out);
  out << std::fixed << std::setprecision(1) << calcPerimeters(polys, in);
}
