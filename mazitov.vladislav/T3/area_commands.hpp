#ifndef AREA_COMMAND_HPP
#define AREA_COMMAND_HPP

#include <iostream>
#include <vector>
#include "polygon.hpp"

namespace mazitov
{
  double calcAreasSum(const std::vector< Polygon > &);
  double getEvenArea(const std::vector< Polygon > &);
  double getOddArea(const std::vector< Polygon > &);
  double getMeanArea(const std::vector< Polygon > &);
  double calcAreas(const std::vector< Polygon > &, std::istream &);
}

#endif
