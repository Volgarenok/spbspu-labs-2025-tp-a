#ifndef COUNT_COMMANDS_HPP
#define COUNT_COMMANDS_HPP

#include <cstddef>
#include <vector>
#include "polygon.hpp"

namespace mazitov
{
  std::size_t getCount(const std::vector< Polygon > &, std::istream &);
}

#endif
