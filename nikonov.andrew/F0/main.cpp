#include <iostream>
#include <sstream>
#include <map>
#include <functional>
#include "Commands.hpp"
#include "DataStorage.hpp"
int main()
{
  nikonov::Storage storage;
  std::map< std::string, std::function< void(nikonov::Storage&, std::istream&, std::ostream&) > > commands;
  nikonov::getCommands(commands);
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    try {
      auto it = commands.find(command);
      if (it != commands.end()) {
        it->second(storage, iss, std::cout);
      } else {
        std::cerr << "Unknown command: " << command << std::endl;
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }
  return 0;
}
