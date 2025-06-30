#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <limits>
#include "polygon.h"
#include "commands.h"

int main(int argc, char* argv[])
{
  using namespace fedorova;

  if (argc != 2)
  {
    std::cerr << "Error: filename not provided" << std::endl;
    return 1;
  }

  std::vector<Polygon> polygons;
  try
  {
    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
      throw std::runtime_error("Failed to open file");
    }

    Polygon poly;
    while (file >> poly)
    {
      polygons.push_back(poly);
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  std::map<std::string, std::function<void()>> cmds;
  cmds["AREA"] = std::bind(printAreaSum, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["MAX"] = std::bind(printMaxValueOf, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["MIN"] = std::bind(printMinValueOf, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["COUNT"] = std::bind(printCountOf, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["PERMS"] = std::bind(printPermsCnt, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["RIGHTSHAPES"] = std::bind(printRightsCnt, std::cref(polygons), std::ref(std::cout));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      auto it = cmds.find(command);
      if (it != cmds.end())
      {
        it->second();
        std::cout << std::endl;
      }
      else
      {
        std::cout << "<INVALID COMMAND>" << std::endl;
      }
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND>" << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return 0;
}

