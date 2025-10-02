#include <fstream>
#include <iterator>
#include <limits>
#include "commands.hpp"

int main(int argc, const char* argv[])
{
  using namespace shak;
  if (argc != 2)
  {
    std::cerr << "Incorrect nuber arguments\n";
    return 1;
  }
  std::ifstream inputFile(argv[1]);
  if (!inputFile.is_open())
  {
    std::cerr << "Can not open file\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  while (!inputFile.eof())
  {
    std::copy(std::istream_iterator< Polygon >{ inputFile }, std::istream_iterator< Polygon >{}, std::back_inserter(polygons));
    if (inputFile.fail())
    {
      inputFile.clear(inputFile.rdstate() ^ std::ios::failbit);
      inputFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void() > > cmds;
  createCommandHandler(cmds, polygons);
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
