#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "dictionary_manager.hpp"
#include "commands.hpp"

std::vector< std::string > splitString(const std::string& str)
{
  std::vector< std::string > tokens;
  std::string currentToken;
  size_t start = 0;
  size_t end = str.length();

  while (start < end && std::isspace(static_cast< unsigned char >(str[start])))
  {
    start++;
  }

  for (size_t i = start; i < end; ++i)
  {
    if (std::isspace(static_cast< unsigned char >(str[i])))
    {
      if (!currentToken.empty())
      {
        tokens.push_back(currentToken);
        currentToken.clear();
      }
    }
    else
    {
      currentToken += str[i];
    }
  }

  if (!currentToken.empty())
  {
    tokens.push_back(currentToken);
  }
  return tokens;
}

int main(int argc, char* argv[])
{
  DictionaryManager dm;
  std::map< std::string, CommandFunction > commandMap = createCommandMap();

  if (argc == 2)
  {
    std::string file = argv[1];
    if (!dm.createDict("main"))
    {
      std::cout << "DICT EXISTS\n";
    }
    else if (!dm.loadFromFile("main", file))
    {
      std::cout << "FILE NOT FOUND\n";
    }
  }

  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::vector< std::string > tokens = splitString(line);
    if (tokens.empty())
    {
      continue;
    }

    std::string commandName = tokens[0];
    std::vector< std::string > args(tokens.begin() + 1, tokens.end());

    auto it = commandMap.find(commandName);
    if (it != commandMap.end())
    {
      try
      {
        it->second(args, dm, std::cout);
      }
      catch (const std::exception& e)
      {
        std::cout << "ERROR Unhandled exception: " << e.what() << "\n";
      }
      catch (...)
      {
        std::cout << "ERROR Unknown unhandled exception occurred.\n";
      }
    }
    else
    {
      std::cout << "INVALID COMMAND\n";
    }
  }

  return 0;
}
