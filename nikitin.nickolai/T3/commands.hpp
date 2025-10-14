#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <numeric>
#include <map>
#include <functional>
#include <iomanip>
#include "geometry.hpp"

namespace nikitin
{
  void areaPrint(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void maxPrint(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void minPrint(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void countPrint(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void inframePrint(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void rightshapesPrint(std::ostream &, const std::vector< Polygon > &);
}

#endif
