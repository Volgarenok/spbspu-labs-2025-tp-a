#include "count_commands.hpp"
#include <algorithm>
#include <functional>
#include <map>

namespace
{
  using mazitov::Polygon;
  std::size_t getCountOdd(const std::vector< Polygon > &polys)
  {
    return std::count_if(polys.begin(), polys.end(), mazitov::isOddVertexNum);
  }

  std::size_t getCountEven(const std::vector< Polygon > &polys)
  {
    return std::count_if(polys.begin(), polys.end(), mazitov::isEvenVertexNum);
  }
}

std::size_t mazitov::getCount(const std::vector< Polygon > &polys, std::istream &in)
{
  std::map< std::string, std::function< std::size_t() > > subcommands
  {
    {"EVEN", std::bind(getCountEven, std::cref(polys))},
    {"ODD", std::bind(getCountOdd, std::cref(polys))}
  };

  std::string subcommand;
  in >> subcommand;

  std::size_t result;
  try
  {
    result = subcommands.at(subcommand)();
  }
  catch (...)
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
