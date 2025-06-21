#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include "commands.hpp"

int main(int argc, char** argv)
{
  using namespace maslevtsov;

  if (argc < 2 || argc > 4) {
    std::cerr << "Error: invalid parameters number\n";
    return 1;
  }
  std::ifstream fin(argv[argc == 4 ? 3 : (argc == 3 ? 2 : 1)]);
  if (!fin) {
    std::cerr << "Error: invalid data file\n";
    return 1;
  }
  if (std::strcmp(argv[1], "--help") == 0 || (argc > 3 && std::strcmp(argv[2], "--help") == 0)) {
    print_help_manual(std::cout);
    std::cout << '\n';
  }
  if (std::strcmp(argv[1], "--check") == 0 || (argc > 3 && std::strcmp(argv[2], "--check") == 0)) {
    return 0;
  }
}
