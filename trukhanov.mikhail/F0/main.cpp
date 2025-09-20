#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <limits>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc > 2)
  {
    std::cerr << "ERROR: wrong arguments" << '\n';
    return 1;
  }

  trukhanov::CommandProcessor processor(std::cout);

  if (argc == 2)
  {
    std::string arg = argv[1];
    if (arg == "--help")
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
      return 0;
    }

    std::ifstream test(arg);
    if (!test.is_open())
    {
      std::cerr << "ERROR: there is no such file" << '\n';
      return 1;
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

  std::string command;
  while (!(std::cin >> command).eof() && !std::cin.bad())
  {
    try
    {
      if (cmds.count(command))
      {
        cmds.at(command)();
      }
      else
      {
        std::string restOfLine;
        std::getline(std::cin, restOfLine);
        processor.execute(command + restOfLine);
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
