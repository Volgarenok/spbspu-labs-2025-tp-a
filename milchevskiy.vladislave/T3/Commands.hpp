#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <cmath>
#include <functional>
#include <iomanip>
#include <limits>
#include <map>
#include <numeric>
#include <string>
#include "Shapes.hpp"

namespace milchevskiy
{
  void areaPrint(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void maxPrint(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void minPrint(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void countPrint(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void lessAreaPrint(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void maxSeqPrint(std::istream&, std::ostream&, const std::vector< Polygon >&);
}

#endif
