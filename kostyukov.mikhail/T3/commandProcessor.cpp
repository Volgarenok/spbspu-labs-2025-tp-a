#include "commandProcessor.hpp"

#include <functional>

#include "commands.hpp"

namespace kostyukov
{
  CommandProcessor::CommandProcessor(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    commands_["AREA"] = std::bind(area, std::ref(in), std::ref(out), std::cref(polygons));
    commands_["MAX"] = std::bind(max, std::ref(in), std::ref(out), std::cref(polygons));
    commands_["MIN"] = std::bind(min, std::ref(in), std::ref(out), std::cref(polygons));
    commands_["COUNT"] = std::bind(count, std::ref(in), std::ref(out), std::cref(polygons));
    commands_["PERMS"] = std::bind(permsCount, std::ref(in), std::ref(out), std::cref(polygons));
    commands_["RIGHTSHAPES"] = std::bind(rightShapesCount, std::ref(out), std::cref(polygons));
  }

  const std::map< std::string, std::function< void() > >& CommandProcessor::getCommands() const
  {
    return commands_;
  }
}
