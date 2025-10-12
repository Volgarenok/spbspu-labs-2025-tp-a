#include <iostream>
#include <sstream>
#include "commands.hpp"

int main()
{
  using namespace mazitov;
  DataSetManager manager;

  std::map< std::string, std::function< void(DataSetManager&, std::istream&, std::ostream&) > > commands;
  getCmds(commands);
  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    try
    {
      auto it = commands.find(command);
      if (it != commands.end())
      {
        it->second(manager, iss, std::cout);
      }
      else
      {
        std::cerr << "<INVALID COMMAND>\n";
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << "\n";
    }
  }
}
