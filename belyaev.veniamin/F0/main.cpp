#include "command-handlers.hpp"
#include "dictionary.hpp"
#include <algorithm>
#include <iterator>
#include <limits>

int main()
{
  using namespace belyaev;
  using namespace std::placeholders;

  Dictionaries data;
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
