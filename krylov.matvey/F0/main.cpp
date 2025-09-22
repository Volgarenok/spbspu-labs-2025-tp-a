#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <limits>
#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "INVALID ARGUMENTS\n" << "\n";
    return 1;
  }

  krylov::CommandProcessor processor(std::cout);

  if (argc == 2)
  {
    std::string arg = argv[1];
    std::ifstream file(arg);
    if (!file)
    {
      std::cerr << "INVALID FILE\n" << "\n";
      return 1;
    }
  }

  std::map< std::string, std::function< void() > > commands;
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND>" << "\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}


