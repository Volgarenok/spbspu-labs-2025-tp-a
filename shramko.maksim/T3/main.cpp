#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <limits>
#include <ios>
#include "polygon.hpp"
#include "printCmd.hpp"

namespace shramko
{
  void readPolygons(std::ifstream& inputFile, std::vector< Polygon >& polygons)
  {
    if (inputFile.eof())
    {
      return;
    }
    Polygon temp;
    inputFile >> temp;
    if (inputFile.eof())
    {
      return;
    }
    if (inputFile.fail())
    {
      inputFile.clear();
      inputFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    else
    {
      polygons.push_back(temp);
    }
    readPolygons(inputFile, polygons);
  }

  void processCommands(const std::vector< Polygon >& polygons)
  {
    std::string cmd;
    if (!(std::cin >> cmd))
    {
      return;
    }
    using cmd_func = std::function<void()>;
    std::map<std::string, cmd_func> commands;
    commands["AREA"] = std::bind(&printArea, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    commands["MAX"] = std::bind(&printMax, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    commands["MIN"] = std::bind(&printMin, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    commands["COUNT"] = std::bind(&printCount, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    commands["LESSAREA"] = std::bind(&printLessArea, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    commands["RIGHTSHAPES"] = std::bind(&printRightShapes, std::cref(polygons), std::ref(std::cout));
    auto it = commands.find(cmd);
    if (it == commands.end())
    {
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
      processCommands(polygons);
      return;
    }
    try
    {
      it->second();
      std::cout << '\n';
    }
    catch (const std::exception& e)
    {
      std::cin.clear();
      std::cout << "<INVALID COMMAND>\n";
    }
    processCommands(polygons);
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: program <input_file>\n";
    return 1;
  }
  std::ifstream inputFile(argv[1]);
  if (!inputFile)
  {
    std::cerr << "Cannot open input file\n";
    return 1;
  }
  std::vector< shramko::Polygon > polygons;
  shramko::readPolygons(inputFile, polygons);
  shramko::processCommands(polygons);
  return 0;
}
