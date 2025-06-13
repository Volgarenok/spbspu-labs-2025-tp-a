#ifndef COMMAND_HANDLERS_HPP
#define COMMAND_HANDLERS_HPP
#include <iomanip>
#include <iostream>
#include <map>
#include "command-helpers.hpp"
#include "shapes.hpp"
#include <stream-guard.hpp>

namespace belyaev
{
  double areaOdd(double value, const Polygon& src);
  double areaEven(double value, const Polygon& src);
  double areaMean(double value, const Polygon& src, size_t size);
  double areaVertices(double value, const Polygon& src, size_t vertices);
  void area(const std::vector<Polygon>& data, std::istream& in, std::ostream& out);
  void minMaxArea(const std::vector<Polygon>& data, std::ostream& out, const std::string& command);
  void minMaxVertices(const std::vector<Polygon>& data, std::ostream& out, const std::string& command);
  void minMax(const std::vector<Polygon>& data, std::istream& in, std::ostream& out, const std::string& command);
  void countEven(const std::vector<Polygon>& data, size_t& result);
  void countOdd(const std::vector<Polygon>& data, size_t& result);
  void countVertices(const std::vector<Polygon>& data, size_t& result, size_t givenSize);
  void count(const std::vector<Polygon>& data, std::istream& in, std::ostream& out);
  void rmecho(std::vector<Polygon>& data, std::istream& in, std::ostream& out);
  void inframe(const std::vector<Polygon>& data, std::istream& in, std::ostream& out);
}

#endif
