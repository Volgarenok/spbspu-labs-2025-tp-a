#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <vector>
#include <string>
#include <ostream>
#include "polygon.hpp"

namespace kazak {
  namespace commands {

    void commandArea(const std::vector<Polygon> & polygons, const std::string & param, std::ostream & os);
    void commandMax(const std::vector<Polygon> & polygons, const std::string & param, std::ostream & os);
    void commandMin(const std::vector<Polygon> & polygons, const std::string & param, std::ostream & os);
    void commandCount(const std::vector<Polygon> & polygons, const std::string & param, std::ostream & os);
    void commandEcho(std::vector<Polygon> & polygons, const std::string & line, std::ostream & os);
    void commandInframe(const std::vector<Polygon> & polygons, const std::string & line, std::ostream & os);

  }
}

#endif
