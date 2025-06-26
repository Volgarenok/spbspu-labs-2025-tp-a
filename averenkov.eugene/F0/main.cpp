#include <iostream>
#include <sstream>
#include "commands.hpp"
#include "solves.hpp"

int main(int argc, char* argv[])
{
  if (argc == 1 && std::string(argv[0]) == "--help")
  {
    averenkov::printHelp(std::cout);
  }
  std::map< std::string, std::function< void(averenkov::Base& , const std::vector< std::string >&) > > commands;
  commands["add"] = averenkov::addItem;
  commands["remove"] = averenkov::removeItem;
  commands["edit"] = averenkov::editItem;
  commands["add_kit"] = averenkov::addKit;
  commands["remove_kit"] = averenkov::removeKit;
  commands["add_to_kit"] = averenkov::addToKit;
  commands["remove_from_kit"] = averenkov::removeFromKit;
  commands["add_knapsack"] = averenkov::addKnapsack;
  commands["set_knapsack"] = averenkov::setKnapsack;
  commands["solve"] = averenkov::solve;
  commands["stats"] = averenkov::showStats;
  commands["reset"] = averenkov::reset;
  commands["bruteforce"] = averenkov::bruteforce;
  commands["dynamic_prog"] = averenkov::dynamicProgrammingSolve;
  commands["backtracking"] = averenkov::backtrackingSolve;
  commands["branch_and_bound"] = averenkov::branchAndBoundSolve;
  commands["save"] = averenkov::saveToFile;
  commands["load"] = averenkov::loadFromFile;

  averenkov::Base base;
  std::string line;

  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    std::vector< std::string > args;
    size_t start = 0;
    size_t end = line.find(' ');

    args.push_back(line.substr(0, end));

    while (end != std::string::npos)
    {
      start = end + 1;
      end = line.find(' ', start);
      args.push_back(line.substr(start, end - start));
    }

    try
    {
      auto it = commands.find(args[0]);
      if (it != commands.end())
      {
        it->second(base, args);
      }
      else
      {
        std::cerr << "Unknown command: " << args[0] << "\n";
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error: " << e.what() << "\n";
    }
  }

  return 0;
}
