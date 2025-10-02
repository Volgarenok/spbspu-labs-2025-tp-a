#include "commands.hpp"
#include <string>

void petrov::createMap(std::map< std::string, Map > & maps, std::istream & in)
{
  std::string map_name;
  if (in >> map_name)
  {
    auto ret_val = maps.insert({ map_name, Map() });
    if (!ret_val.second)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void petrov::removeMap(std::map< std::string, Map > & maps, std::istream & in)
{
  std::string map_name;
  if (in >> map_name)
  {
    try
    {
      maps.erase(map_name);
    }
    catch (...)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void petrov::addPlanet(std::map< std::string, Map > & maps, std::istream & in)
{
  std::string map_name;
  std::string new_planet_name;
  double x = 0.0, y = 0.0;
  if (in >> map_name >> new_planet_name >> x >> y)
  {
    if (x <= 25.0 && y <= 25.0)
    {
      throw std::logic_error("<TOO FAR FROM ORIGIN>");
    }
    auto it = maps.find(map_name);
    if (it != maps.cend())
    {
      Planet new_planet{ x, y };
      try
      {
        it->second.addPlanet(new_planet_name, new_planet);
      }
      catch(...)
      {
        throw;
      }
    }
    else
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void petrov::removePlanet(std::map< std::string, Map > & maps, std::istream & in)
{
  std::string map_name;
  std::string todelete_planet_name;
  if (in >> map_name >> todelete_planet_name)
  {
    auto it = maps.find(map_name);
    if (it != maps.cend())
    {
      try
      {
        it->second.removePlanet(todelete_planet_name);
      }
      catch(...)
      {
        throw;
      }
      }
    else
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void petrov::clearMap(std::map< std::string, Map > & maps, std::istream & in)
{
  std::string map_name;
  if (in >> map_name)
  {
    auto it = maps.find(map_name);
    if (it != maps.cend())
    {
      try
      {
        it->second.removeAllPlanets();
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << '\n';
      }
      }
    else
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}
