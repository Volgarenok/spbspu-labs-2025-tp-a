#include <iostream>
#include <limits>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <map>
#include <functional>
#include "printCmd.hpp"
#include "polygon.hpp"

int main(int argc, char* argv[])
{
  using namespace dribas;
  if (argc != 2) {
    std::cerr << "No parametrs found" << '\n';
    return 1;
  }

  std::ifstream file(argv[1]);
  std::vector< dribas::Polygon > plgs;
  while (!file.eof()) {
    std::copy(std::istream_iterator< Polygon >(file), std::istream_iterator< Polygon >(), std::back_inserter(plgs));
    if (!file) {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(printArea, std::cref(plgs), std::ref(std::cin), std::ref(std::cout));
  cmds["MAX"] = std::bind(printMax, std::cref(plgs), std::ref(std::cin), std::ref(std::cout));
  cmds["MIN"] = std::bind(printMin, std::cref(plgs), std::ref(std::cin), std::ref(std::cout));
  cmds["COUNT"] = std::bind(printCount, std::cref(plgs), std::ref(std::cin), std::ref(std::cout));
  cmds["LESSAREA"] = std::bind(printLessArea, std::cref(plgs), std::ref(std::cin), std::ref(std::cout));
  cmds["RIGHTSHAPES"] = std::bind(printRightShapes, std::cref(plgs), std::ref(std::cout));

  std::string command;
  while (!(std::cin >> command).eof()) {
    try {
      cmds.at(command)();
      std::cout << '\n';
    } catch (const std::exception& e) {
      if (std::cin.fail()) {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
