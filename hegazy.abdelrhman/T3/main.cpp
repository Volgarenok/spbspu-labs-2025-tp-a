#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include "geometry.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using Polygon = geom::Polygon;
  using it = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    return 1;
  }

  std::vector< Polygon > polyList;
  std::ifstream inFile(argv[1]);

  while (!inFile.eof())
  {
    std::copy(it(inFile), it(), std::back_inserter(polyList));
    if (!inFile)
    {
      inFile.clear();
      inFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > commandMap;
  commandMap["AREA"] = std::bind(bob::printAreaSum, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MAX"] = std::bind(bob::printMaxValueOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MIN"] = std::bind(bob::printMinValueOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["COUNT"] = std::bind(bob::printCountOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["LESSAREA"] = std::bind(bob::printLessAreaCnt, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["INTERSECTIONS"] = std::bind(bob::printIntersectionsCnt, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));

  std::string line;
  while (!(std::cin >> line).eof())
  {
    try
    {
      commandMap.at(line)();
      std::cout << "\n";
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
  return 0;
}
