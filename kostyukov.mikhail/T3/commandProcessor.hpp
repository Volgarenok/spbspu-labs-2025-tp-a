#ifndef COMMAND_PROCESSOR_HPP
#define COMMAND_PROCESSOR_HPP

#include <functional>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

#include "geometry.hpp"

namespace kostyukov
{
  using PolygonContainer = const std::vector< Polygon >;
  using CommandFunc = void (*)(std::istream&, std::ostream&, PolygonContainer&);

  class CommandProcessor
  {
    public:
      CommandProcessor(PolygonContainer& polygons, std::istream& in, std::ostream& out);
      void process(const std::string& commandName);
    private:
      void add(const std::string& commandName, CommandFunc func);
      std::map< std::string, std::function< void() > > commands_;
      PolygonContainer& polygons_;
      std::istream& in_;
      std::ostream& out_;
  };
}
#endif
