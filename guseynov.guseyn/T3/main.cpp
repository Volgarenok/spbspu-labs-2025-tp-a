#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <functional>
#include "polygon.hpp"
#include "utils.hpp"
#include "commands.hpp"

namespace
{
  struct IsSpace
  {
    bool operator()(char c) const
    {
      return std::isspace(static_cast<unsigned char>(c));
    }
  };

  struct TrimSpaces
  {
    void operator()(std::string& str) const
    {
      IsSpace isSpace;
      str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), isSpace));
      str.erase(std::find_if_not(str.rbegin(), str.rend(), isSpace).base(), str.end());
    }
  };

  std::string extractFirstWord(const std::string& str)
  {
    IsSpace isSpace;
    auto start = std::find_if_not(str.begin(), str.end(), isSpace);
    if (start == str.end())
    {
      return "";
    }
    auto end = std::find_if(start, str.end(), isSpace);
    return std::string(start, end);
  }

  std::string extractRemainingString(const std::string& str)
  {
    IsSpace isSpace;
    auto firstWordEnd = std::find_if(str.begin(), str.end(), isSpace);
    if (firstWordEnd == str.end())
    {
      return "";
    }
    auto remainingStart = std::find_if_not(firstWordEnd, str.end(), isSpace);
    if (remainingStart == str.end())
    {
      return "";
    }
    return std::string(remainingStart, str.end());
  }

  std::map<std::string, guseynov::commands::CommandHandler> createCommandMap()
  {
    std::map<std::string, guseynov::commands::CommandHandler> commands;
    commands["AREA"] = guseynov::commands::handleAreaCommand;
    commands["MAX"] = guseynov::commands::handleMaxCommand;
    commands["MIN"] = guseynov::commands::handleMinCommand;
    commands["COUNT"] = guseynov::commands::handleCountCommand;
    commands["LESSAREA"] = guseynov::commands::handleLessAreaCommand;
    commands["INFRAME"] = guseynov::commands::handleInFrameCommand;
    return commands;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " filename\n";
    return 1;
  }

  try
  {
    std::vector<guseynov::Polygon> polygons = guseynov::utils::readPolygonsFromFile(argv[1]);
    auto commandMap = createCommandMap();
    std::string command;
    while (std::getline(std::cin, command))
    {
      TrimSpaces trimSpaces;
      trimSpaces(command);
      if (command.empty())
      {
        continue;
      }

      std::string cmd = extractFirstWord(command);
      std::string param = extractRemainingString(command);

      auto it = commandMap.find(cmd);
      if (it != commandMap.end())
      {
        it->second(polygons, param);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
