#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using namespace shchadilov;

  if (argc != 2)
  {
    std::cerr << "Error: missing filename argument\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    Polygon poly;
    if (file >> poly)
    {
      polygons.push_back(poly);
    }
    else
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > areaCmd;
  areaCmd["AREA"] = std::bind(printArea, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);

  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > maxCmd;
  maxCmd["MAX"] = std::bind(printMax, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);

  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > minCmd;
  minCmd["MIN"] = std::bind(printMin, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);

  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > countCmd;
  countCmd["COUNT"] = std::bind(printCount, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);

  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > rmechoCmd;
  rmechoCmd["RMECHO"] = std::bind(printRmEcho, std::ref(polygons), std::placeholders::_1, std::placeholders::_2);

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      if (command == "AREA")
      {
        areaCmd.at(command)(std::cin, std::cout);
      }
      else if (command == "MAX")
      {
        maxCmd.at(command)(std::cin, std::cout);
      }
      else if (command == "MIN")
      {
        minCmd.at(command)(std::cin, std::cout);
      }
      else if (command == "COUNT")
      {
        countCmd.at(command)(std::cin, std::cout);
      }
      else if (command == "RIGHTSHAPES")
      {
        printRights(polygons, std::cout);
      }
      else if (command == "RMECHO")
      {
        rmechoCmd.at(command)(std::cin, std::cout);
      }
      else
      {
        throw std::invalid_argument("Invalid command");
      }
      std::cout << "\n";
    }
    catch (const std::exception& e)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
