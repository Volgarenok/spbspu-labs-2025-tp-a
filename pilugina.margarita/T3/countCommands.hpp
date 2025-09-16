#ifndef COUNT_COMMANDS_HPP
#define COUNT_COMMANDS_HPP

#include <cstddef>
#include <vector>
#include "polygon.hpp"

namespace pilugina
{
  std::size_t getCountOdd(const std::vector< Polygon > &);
  std::size_t getCountEven(const std::vector< Polygon > &);
  std::size_t getCountVertices(const std::vector< Polygon > &);
  std::size_t getCount(const std::vector< Polygon > &, std::istream &);
}

#endif
