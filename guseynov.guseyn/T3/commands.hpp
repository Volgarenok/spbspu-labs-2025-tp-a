#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include <functional>
#include "polygon.hpp"

namespace guseynov
{
  namespace commands
  {
    using CommandHandler = std::function<void(const std::vector<Polygon>&, const std::string&)>;

    void handleAreaCommand(const std::vector<Polygon>& polygons, const std::string& param);
    void handleMaxCommand(const std::vector<Polygon>& polygons, const std::string& param);
    void handleMinCommand(const std::vector<Polygon>& polygons, const std::string& param);
    void handleCountCommand(const std::vector<Polygon>& polygons, const std::string& param);
    void handleInFrameCommand(const std::vector<Polygon>& polygons, const std::string& param);
    void handleLessAreaCommand(const std::vector<Polygon>& polygons, const std::string& param);
  }
}

#endif
