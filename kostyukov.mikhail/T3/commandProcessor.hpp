#ifndef COMMAND_PROCESSOR_HPP
#define COMMAND_PROCESSOR_HPP

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "geometry.hpp"

namespace kostyukov
{
  class CommandProcessor
  {
    public:
      CommandProcessor(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
      const std::map< std::string, std::function< void() > >& getCommands() const;
    private:
      std::map< std::string, std::function< void() > > commands_;
  };
}
#endif
