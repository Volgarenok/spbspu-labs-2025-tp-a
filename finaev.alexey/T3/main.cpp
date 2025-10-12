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
      file.clear(file.rdstate() ^ std::ios::failbit);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(std::istream_iterator< Polygon >(file), std::istream_iterator< Polygon >(), std::back_inserter(shapes));
  }
  auto commands = finaev::createCommandsHandler(std::cin, std::cout, shapes);
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)();
    }
    catch(...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>";
    }
    std::cout << "\n";
  }
}
