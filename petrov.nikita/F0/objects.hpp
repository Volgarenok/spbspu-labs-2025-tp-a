#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <string>
#include <map>
#include <vector>

namespace petrov
{
  struct Planet
  {
    double x, y;
    Planet(const double & x_coord, const double & y_coord):
      x(x_coord),
      y(y_coord)
    {}
  };

  struct Map
  {
    Map() = default;
    void addPlanet(const std::string & new_planet_name, const Planet & new_planet);
    void removePlanet(const std::string & todelete_planet_name);
    void removeAllPlanets();
    double findOptimalWay(const std::string & first_planet_name, const std::string & second_planet_name);
  private:
    std::map< std::string, Planet > planets_;
    std::vector< std::vector< double > > adjency_matrix_;
  };
}

#endif
