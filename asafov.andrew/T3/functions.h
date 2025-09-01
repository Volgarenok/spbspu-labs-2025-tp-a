#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include "geometry.h"

namespace asafov
{
  void processCommand(const std::vector<Polygon>& polygons, const std::string& cmd);
  void handleAreaCommands(const std::vector<Polygon>& polygons, const std::string& arg);
  void handleMaxCommands(const std::vector<Polygon>& polygons, const std::string& arg);
  void handleMinCommands(const std::vector<Polygon>& polygons, const std::string& arg);
  void handleCountCommands(const std::vector<Polygon>& polygons, const std::string& arg);
  void handlePermsCommand(const std::vector<Polygon>& polygons, const std::string& arg);
  void handleMaxSeqCommand(const std::vector<Polygon>& polygons, const std::string& arg);
  void handleRmechoCommand(std::vector<Polygon>& polygons, const std::string& arg);
  void handleEchoCommand(std::vector<Polygon>& polygons, const std::string& arg);
  void handleLessAreaCommand(const std::vector<Polygon>& polygons, const std::string& arg);
  void handleInFrameCommand(const std::vector<Polygon>& polygons, const std::string& arg);
  void handleIntersectionsCommand(const std::vector<Polygon>& polygons, const std::string& arg);
  void handleSameCommand(const std::vector<Polygon>& polygons, const std::string& arg);
  void handleRectsCommand(const std::vector<Polygon>& polygons);
  void handleRightShapesCommand(const std::vector<Polygon>& polygons);
}

#endif
