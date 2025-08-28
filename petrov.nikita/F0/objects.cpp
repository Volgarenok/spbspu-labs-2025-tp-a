#include "objects.hpp"
#include <stdexcept>

void petrov::Map::addPlanet(const std::string & new_planet_name, const Planet & new_planet)
{
  auto it = planets_.find(new_planet_name);
  if (it == planets_.cend())
  {
    planets_.insert({ new_planet_name, new_planet });
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void petrov::Map::removePlanet(const std::string & todelete_planet_name)
{
  auto it = planets_.find(todelete_planet_name);
  if (it != planets_.cend())
  {
    planets_.erase(todelete_planet_name);
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void petrov::Map::removeAllPlanets()
{
  planets_.clear();
}


