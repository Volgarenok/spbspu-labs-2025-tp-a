#include <iostream>
#include "Polygon.hpp"
#include "utils.hpp"

int main(int argc, char* argv[])
{
  std::string filename = "";

  if (argc >= 2) {
    filename = argv[1];
  }

  try {
    gavrilova::startCommandInterface(filename, std::cin, std::cout);
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  return 0;
}
