#include <fstream>
#include <iostream>
#include "command-processor.hpp"

int main()
{
  try {
    using namespace kizhin;
    const std::string stateFile = getStateFile();
    std::ifstream fin(stateFile);
    auto state = loadState(fin);
    CommandProcessor cmd(state, std::cin, std::cout, std::cerr);
    cmd.processCommands();
    std::ofstream fout(stateFile);
    saveState(fout, state);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}

