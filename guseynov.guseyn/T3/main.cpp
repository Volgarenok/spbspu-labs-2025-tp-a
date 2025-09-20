#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "polygon.hpp"
#include "utils.hpp"
#include "commands.hpp"
#include "lessarea.hpp"
#include "inframe.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
    return 1;
  }

  try
  {
    std::vector< guseynov::Polygon > polygons = guseynov::utils::readPolygonsFromFile(argv[1]);
    std::string command;
    while (std::getline(std::cin, command))
    {
      std::istringstream iss(command);
      std::string cmd, param;
      iss >> cmd;
      if (cmd == "AREA")
      {
        iss >> param;
        guseynov::commands::handleAreaCommand(polygons, param);
      }
      else if (cmd == "MAX")
      {
        iss >> param;
        guseynov::commands::handleMaxCommand(polygons, param);
      }
      else if (cmd == "MIN")
      {
        iss >> param;
        guseynov::commands::handleMinCommand(polygons, param);
      }
      else if (cmd == "COUNT")
      {
        iss >> param;
        guseynov::commands::handleCountCommand(polygons, param);
      }
      else if (cmd == "LESSAREA")
      {
        std::getline(iss, param);
        param.erase(0, param.find_first_not_of(" "));
        guseynov::commands::handleLessAreaCommand(polygons, param);
      }
      else if (cmd == "INFRAME")
      {
        std::getline(iss, param);
        param.erase(0, param.find_first_not_of(" "));
        guseynov::commands::handleInFrameCommand(polygons, param);
      }
      else
      {
        std::cout << "<INVALID COMMAND>" << std::endl;
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
