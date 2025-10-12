#include "commands.hpp"
#include <iostream>
#include <string>

int main()
{
  CommandHandler handler;
  std::string input;
  while (true)
  {
    std::getline(std::cin, input);
    handler.handleCommand(input);
  }
  return 0;
}
