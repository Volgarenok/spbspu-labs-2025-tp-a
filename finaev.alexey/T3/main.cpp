#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <limits>
#include "shapes.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Uncorrect input\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error openning\n";
    return 1;
  }
  using finaev::Polygon;
  std::vector< Polygon > shapes;
  while (!file.eof())
  {
    if (file.fail())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(std::istream_iterator< Polygon >(file), std::istream_iterator< Polygon >(), std::back_inserter(shapes));
  }
  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(finaev::area, std::ref(std::cin), std::ref(std::cout), std::cref(shapes));
  commands["COUNT"] = std::bind(finaev::count, std::ref(std::cin), std::ref(std::cout), std::cref(shapes));
  commands["MAX"] = std::bind(finaev::max, std::ref(std::cin), std::ref(std::cout), std::cref(shapes));
  commands["MIN"] = std::bind(finaev::min, std::ref(std::cin), std::ref(std::cout), std::cref(shapes));
  commands["SAME"] = std::bind(finaev::same, std::ref(std::cin), std::ref(std::cout), std::cref(shapes));
  commands["ECHO"] = std::bind(finaev::echo, std::ref(std::cin), std::ref(std::cout), std::ref(shapes));
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      auto func = commands.find(cmd);
      if (func != commands.end())
      {
        func->second();
      }
      else
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
    }
    catch(const std::exception& e)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << e.what();
    }
    std::cout << "\n";
  }
}
