#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include "polygon.hpp"

namespace amine
{

  class CommandProcessor
  {
  public:
    CommandProcessor(const std::vector< Polygon >& polygons);
    void operator()(const std::string& line) const;

  private:
    const std::vector< Polygon >& polygons_;

    void command_area(const std::string& rest) const;
    void command_max(const std::string& rest) const;
    void command_min(const std::string& rest) const;
    void command_count(const std::string& rest) const;
    void command_intersections(const std::string& rest) const;
    void command_rmecho(const std::string& rest) const;
  };

}

#endif
