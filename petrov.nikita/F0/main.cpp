#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <limits>
#include "cmd_line_args.hpp"
#include "commands.hpp"
#include "objects.hpp"

namespace
{
  int checkArguments(const int argc)
  {
    if (argc > 1)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

int main(const int argc, const char * const * argv)
{
  using namespace petrov;
  if (checkArguments(argc))
  {
    processCmdLineAgruments(argc, argv);
  }
  std::map< std::string, Map > maps;
  std::map< std::string, std::function< void() > > cmds;
  cmds["create_map"] = std::bind(createMap, std::ref(maps), std::ref(std::cin));
  cmds["remove_map"] = std::bind(removeMap, std::ref(maps), std::ref(std::cin));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>";
      std::cout << "\n";
    }
  }
}
