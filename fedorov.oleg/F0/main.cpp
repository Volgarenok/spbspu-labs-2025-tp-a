#include <iostream>
#include <map>
#include <functional>
#include <vector>

#include "TextProcessor.hpp"
#include "CommandHandler.hpp"
#include "Tokenizer.hpp"

int main()
{
  crossref::TextProcessor processor;
  auto commandMap = crossref::CommandHandler::createCommandMap();
  std::string input;

  std::cout << "Enter 'help' for list of commands\n";

  while (true)
  {
    std::cout << "> ";
    if (!std::getline(std::cin, input) || input == "exit")
    {
      break;
    }
    if (input.empty())
    {
      continue;
    }

    std::vector< std::string > tokens = crossref::Tokenizer::tokenize(input);
    if (tokens.empty())
    {
      continue;
    }

    std::string command = tokens[0];
    std::vector< std::string > args(tokens.begin() + 1, tokens.end());

    try
    {
      auto it = commandMap.find(command);
      if (it != commandMap.end())
      {
        it->second(processor, args);
      }
      else
      {
        std::cout << "<UNKNOWN COMMAND>\n";
      }
    }
    catch (const std::exception &e)
    {
      std::cout << "Error: " << e.what() << '\n';
    }
  }
}
