#include "countCommands.hpp"
#include <algorithm>
#include <functional>
#include <map>
#include "areaCommands.hpp"

std::size_t pilugina::getCountOdd(const std::vector< Polygon > &polys)
{
  return std::count_if(polys.begin(), polys.end(), isOddVertexNum);
}

std::size_t pilugina::getCountEven(const std::vector< Polygon > &polys)
{
  return std::count_if(polys.begin(), polys.end(), isEvenVertexNum);
}

std::size_t pilugina::getCount(const std::vector< Polygon > &polys, std::istream &in)
{
  std::map< std::string, std::function< std::size_t() > > subcommands
  {
    {"EVEN", std::bind(getCountEven, std::cref(polys))}, {"ODD", std::bind(getCountOdd, std::cref(polys))}
  };

  std::string subcommand;
  in >> subcommand;

  std::size_t result;
  try
  {
    result = subcommands.at(subcommand)();
  }
  catch (const std::out_of_range &)
  {
    std::size_t verticesNum = std::stoull(subcommand);
    if (verticesNum < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    auto predicate = std::bind(isEqualVertexNum, verticesNum, std::placeholders::_1);
    result = std::count_if(polys.begin(), polys.end(), predicate);
  }
  return result;
}
