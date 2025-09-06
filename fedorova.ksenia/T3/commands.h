#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <vector>
#include <functional>
#include "polygon.h"

namespace fedorova
{
  void printAreaSum(std::istream& in, const std::vector<Polygon>& plgs, std::ostream& out);
  void printMaxValueOf(std::istream& in, const std::vector<Polygon>& plgs, std::ostream& out);
  void printMinValueOf(std::istream& in, const std::vector<Polygon>& plgs, std::ostream& out);
  void printCountOf(std::istream& in, const std::vector<Polygon>& plgs, std::ostream& out);
  void printPermsCnt(std::istream& in, const std::vector<Polygon>& plgs, std::ostream& out);
  void printRightsCnt(const std::vector<Polygon>& plgs, std::ostream& out);
}

#endif
