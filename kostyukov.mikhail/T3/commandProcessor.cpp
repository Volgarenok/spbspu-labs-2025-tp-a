#include "commandProcessor.hpp"

#include "commands.hpp"

kostyukov::CommandProcessor::CommandProcessor(PolygonContainer& polygons, std::istream& in, std::ostream& out):
  polygons_(polygons),
  in_(in),
  out_(out)
{
  add("AREA", area);
  add("MAX", max);
  add("MIN", min);
  add("COUNT", count);
  add("PERMS", permsCount);
  commands_["RIGHTSHAPES"] = std::bind(rightShapesCount, std::ref(out_), std::cref(polygons));
}

void kostyukov::CommandProcessor::add(const std::string& commandName, CommandFunc func)
{
  commands_[commandName] = std::bind(func, std::ref(in_), std::ref(out_), std::cref(polygons_));
  return;
}

void kostyukov::CommandProcessor::process(const std::string& commandName)
{
  commands_.at(commandName)();
  return;
}
