#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <map>
#include <string>
#include <functional>
#include <fstream>
#include <iostream>
#include "graph.hpp"

namespace klimova
{
  using GraphType = Graph< std::string >;
  using GraphManager = std::unordered_map< std::string, GraphType >;
  using CommandHandler = std::map< std::string, std::function< void(std::istream&, std::ostream&) > >;
  using namespace std::placeholders;

  void loadGraphFromFile(GraphManager& graphs, const std::string& filename);
  bool checkGraphFile(const std::string& filename);
  void showHelp(std::ostream& out);
  CommandHandler createCommandHandler(GraphManager& graphs);
}
#endif
