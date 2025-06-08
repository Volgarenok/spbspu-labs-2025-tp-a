#include "commands.hpp"

#include <iostream>
#include <limits>

void zholobov::cmdArea(std::istream& input, std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  output << "In cmdArea\n";
  input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}

void zholobov::cmdMax(std::istream& input, std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  output << "In cmdMax\n";
  input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}

void zholobov::cmdMin(std::istream& input, std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  output << "In cmdMin\n";
  input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}

void zholobov::cmdCount(std::istream& input, std::ostream& output, [[maybe_unused]] const Polygons& polygons)
{
  output << "In cmdCount\n";
  input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}
