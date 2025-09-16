#include <algorithm>
#include <fstream>
#include <iterator>
#include "command-handlers.hpp"
#include "shapes.hpp"

int main(int argc, char* argv[])
{
  using namespace belyaev;
  using namespace std::placeholders;

  if (argc != 2)
  {
    std::cerr << "<INVALID NUMBER OF ARGUMENTS>\n";
    return 1;
  }
  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "<INVALID FILE>\n";
    return 1;
  }
  std::vector< Polygon > data;

  using PolyIStream = std::istream_iterator< Polygon >;
  while (!file.eof())
  {
    if (file.fail())
    {
      file.clear(file.rdstate() ^ std::ios_base::failbit);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(PolyIStream{file}, PolyIStream{}, std::back_inserter(data));
  }

  commandMap cmds = mapCommandHandlers(data);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)(std::cin, std::cout);
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
