#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <functional>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  trukhanov::CommandProcessor processor(std::cout);

  std::string line;
  while (std::getline(std::cin, line))
  {
    try
    {
      processor.execute(line);
    }
    catch (const std::exception& e)
    {
      std::cout << "INVALID COMMAND\n";
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["--help"] = [&processor]()
    {
      std::cout <<
        "Available commands:\n"
        "createindex <index> <filename>\n"
        "searchword <index> <word>\n"
        "showindex <index>\n"
        "clearindex <index>\n"
        "showfrequency <index>\n"
        "saveindex <index> <filename>\n"
        "exportword <index> <word> <filename>\n"
        "filterlines <index> <from> <to>\n"
        "replaceword <index> <old> <new>\n"
        "listindexes\n"
        "compareindexes <index1> <index2>\n"
        "longestwords <index> <count>\n"
        "shortestwords <index> <count>\n"
        "mergeindexes <index1> <index2> <new_index>\n"
        "reconstructtext <index> <filename>\n"
        "mergebylines <index1> <index2> <new_index>\n";
    };

  if (argc > 2)
  {
    std::cerr << "ERROR: wrong arguments\n";
    return 1;
  }

  if (argc == 2)
  {
    std::string arg = argv[1];
    if (arg == "--help")
    {
      cmds.at("--help")();
      return 0;
    }

    std::ifstream test(arg);
    if (!test.is_open())
    {
      std::cerr << "ERROR: there is no such file\n";
      return 1;
    }
    test.close();

    processor.execute("createindex default " + arg);
  }

  try
  {
    std::string commandLine;
    while (!(std::cin >> std::ws).eof() && std::getline(std::cin, commandLine))
    {
      if (commandLine.empty())
      {
        continue;
      }

      std::istringstream iss(commandLine);
      std::string command;
      iss >> command;

      if (cmds.count(command))
      {
        cmds.at(command)();
      }
      else
      {
        processor.execute(commandLine);
      }

      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
    }
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>\n";
  }

  return 0;
}
