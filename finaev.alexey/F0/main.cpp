#include <iostream>
#include <fstream>
#include <unordered_map>
#include <limits>
#include "commands.hpp"
#include "openningInfo.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Uncorrect input\n";
    return 1;
  }
  std::unordered_map< std::string, finaev::OpenningInfo > globalOpenings;
  std::unordered_map< std::string, std::unordered_map< std::string, bool > > bases;
  if (argc > 1)
  {
    std::string mode = argv[1];
    if (mode == "--load")
    {
      try
      {
        finaev::loadDebutsFromFile(argv[2], globalOpenings, std::cout);
        std::cout << "Successfully loaded " << globalOpenings.size() << " debuts\n";
      }
      catch (const std::exception& e)
      {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
      }
    }
    else
    {
      std::cerr << "Unknown parameter\n";
      return 1;
    }
  }
  auto commands = finaev::createCommandsHandler(std::cin, std::cout, globalOpenings, bases);
  std::string cmd;
  while (std::cin >> cmd)
  {
    auto it = commands.find(cmd);
    if (it == commands.end())
    {
      std::cout << ("<INVALID COMMAND>\n");
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      continue;
    }
    try
    {
      it->second();
    }
    catch (const std::exception& e)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cout << e.what();
    }
    std::cout << "\n";
  }
}
