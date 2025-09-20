#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include "geometry.hpp"

namespace cherkasov
{
  void processCommand(const std::vector<Polygon>& polys, const std::string& cmd);
  void handleArea(const std::vector<Polygon>& polys, const std::string& arg);
  void handleMax(const std::vector<Polygon>& polys, const std::string& arg);
  void handleMin(const std::vector<Polygon>& polys, const std::string& arg);
  void handleCount(const std::vector<Polygon>& polys, const std::string& arg);
  void handleIntersections(const std::vector<Polygon>& polys, const std::string& arg);
  void handleRightShapes(const std::vector<Polygon>& polys);
}

#endif
