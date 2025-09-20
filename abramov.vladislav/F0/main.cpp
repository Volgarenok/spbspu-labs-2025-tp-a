#include <limits>
#include <iostream>
#include "commands.hpp"
#include "dictionary_collection.hpp"

int main()
{
  using namespace abramov;

  DictionaryCollection collection{};
  std::map< std::string, std::function< void() > > commands;
  getCommands(commands, collection);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::exception &e)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
      std::cout << e.what();
    }
  }
}
