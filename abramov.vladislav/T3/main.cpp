#include <map>
#include <limits>
#include <vector>
#include <fstream>
#include <iterator>
#include <functional>
#include "geom.hpp"
#include "commands.hpp"

int main(int argc, char **argv)
{
  using namespace abramov;

  if (argc != 2)
  {
    std::cerr << "Wrong filename\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Wrong input\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  while (!input.eof())
  {
    try
    {
      std::copy(std::istream_iterator< Polygon >(input), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
    }
    catch (std::logic_error &)
    {
      input.clear();
      input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void() > > commands;
  getCommands(commands, polygons);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
      std::cout << "\n";
    }
    catch (...)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
