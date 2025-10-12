#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "objects.hpp"

namespace petrov
{
  void createMap(std::map< std::string, Map > & maps, std::istream & in);
  void removeMap(std::map< std::string, Map > & maps, std::istream & in);

  void addPlanet(std::map< std::string, Map > & maps, std::istream & in);
  void removePlanet(std::map< std::string, Map > & maps, std::istream & in);
  void clearMap(std::map< std::string, Map > & maps, std::istream & in);
}

#endif
