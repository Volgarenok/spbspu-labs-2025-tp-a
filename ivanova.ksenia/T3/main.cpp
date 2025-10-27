#include <algorithm>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <stdexcept>
#include <vector>
#include <utility>

#include "commands.hpp"
#include "polygon.hpp"

using ivanova::Polygon;

void readPolygons(std::istream& in, std::vector< Polygon >& data)
{
  using iIterator = std::istream_iterator< Polygon >;

  while (!in.eof())
  {
    std::copy(iIterator(in), iIterator(), std::back_inserter(data));
    if (!in)
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::fstream fin(argv[1]);
  if (!fin.is_open())
  {
    std::cerr << "can't open file: '" << argv[1] << "'\n";
    return 2;
  }
  std::vector< Polygon > data;
  readPolygons(fin, data);
  fin.close();

  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(ivanova::areaCommand, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  commands["MAX"] = std::bind(ivanova::maxCommand, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  commands["MIN"] = std::bind(ivanova::minCommand, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  commands["COUNT"] = std::bind(ivanova::countCommand, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  commands["ECHO"] = std::bind(ivanova::echoCommand, std::ref(std::cin), std::ref(std::cout), std::ref(data));
  commands["SAME"] = std::bind(ivanova::sameCommand, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  commands["PRINT"] = std::bind(ivanova::printCommand, std::ref(std::cout), std::cref(data));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      auto it = commands.find(command);
      if (it != commands.end())
      {
        it->second();
      }
      else
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
