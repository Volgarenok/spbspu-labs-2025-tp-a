#include <cstring>
#include <iostream>

#include "commands.hpp"

int main(int argc, char* argv[])
{
  using namespace zholobov;
  if (argc > 2) {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }
  if (argc == 2) {
    if (std::strcmp(argv[1], "--help") == 0) {
      printHelp(std::cout);
      std::cout << '\n';
      return 0;
    }
  }

  return 0;
}
