#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include <sstream>
#include "geometry.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using Polygon = geom::Polygon;

  if (argc != 2)
  {
    return 1;
  }

  std::ifstream inFile(argv[1]);
  if (!inFile)
  {
    std::cerr << "Could not open file\n";
    return 1;
  }

  std::vector< geom::Polygon > polyList;
  std::string line;
  while (std::getline(inFile, line))
  {
    if (line.empty())
    {
      continue;
    }
    std::istringstream iss(line);
    Polygon temp;
    if (iss >> temp)
    {
      polyList.push_back(std::move(temp));
    }
  }

  std::map< std::string, std::function< void(std::istream&) > > commandMap;
  commandMap["AREA"] = [&](std::istream& args) {
    smirnov::printAreaSum(args, std::cref(polyList), std::ref(std::cout));
    };
  commandMap["MAX"] = [&](std::istream& args) {
    smirnov::printMaxValueOf(args, std::cref(polyList), std::ref(std::cout));
    };
  commandMap["MIN"] = [&](std::istream& args) {
    smirnov::printMinValueOf(args, std::cref(polyList), std::ref(std::cout));
    };
  commandMap["COUNT"] = [&](std::istream& args) {
    smirnov::printCountOf(args, std::cref(polyList), std::ref(std::cout));
    };
  commandMap["LESSAREA"] = [&](std::istream& args) {
    smirnov::printLessAreaCnt(args, std::cref(polyList), std::ref(std::cout));
    };
  commandMap["INTERSECTIONS"] = [&](std::istream& args) {
    smirnov::printIntersectionsCnt(args, std::cref(polyList), std::ref(std::cout));
    };

  while (std::getline(std::cin, line))
  {
    std::istringstream iss(line);
    std::string cmd;

    if (!(iss >> cmd) || !commandMap.count(cmd))
    {
      std::cout << "<INVALID COMMAND>\n";
      continue;
    }
    try
    {
      commandMap[cmd](iss);
      std::cout << "\n";
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
