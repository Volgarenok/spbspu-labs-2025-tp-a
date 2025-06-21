#include <iostream>
#include <string>
#include <limits>
#include <sstream>  
#include "commands.hpp"

using namespace smirnov;

int main(int argc, char* argv[])
{
  DictionaryManager dm;
  auto commandMap = createCommandMap();
  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::stringstream ss(line);
    std::string commandName;
    ss >> commandName;

    auto it = commandMap.find(commandName);
    if (it != commandMap.end())
    {
      try
      {
        it->second(ss, dm, std::cout);
      }
      catch (const std::exception& e)
      {
        std::cout << "<ERROR> Unhandled exception: " << e.what() << "\n";
      }
      catch (...)
      {
        std::cout << "<ERROR> Unknown unhandled exception occurred.\n";
      }
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
