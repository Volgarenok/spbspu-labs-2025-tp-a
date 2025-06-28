#include <fstream>
#include <iostream>
#include "command-processor.hpp"

namespace kizhin {
  void outHelp(std::ostream&, const char*);
}

int main(int argc, char** argv)
{
  using namespace kizhin;
  if (argc != 1) {
    outHelp(std::cout, argv[0]);
    return 0;
  }
  try {
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

void kizhin::outHelp(std::ostream& out, const char* exec)
{
  out << "Usage: " << exec << " [OPTIONS]" << '\n';
  out << "A program for processing and analyzing frequency dictionaries." << '\n';
  out << '\n';
  out << "Options:" << '\n';
  out << "  -h, --help    Show this help message and exit" << '\n';
  out << '\n';
  out << "Available Commands (interactive mode):" << '\n';
  out << "  ls [dict...]             - List all dictionaries or specified ones" << '\n';
  out << "  add <dict> [file...]     - Add files to dictionary" << '\n';
  out << "  rm <dict>                - Remove a dictionary" << '\n';
  out << "  mv <old> <new>           - Rename a dictionary" << '\n';
  out << "  clear                    - Remove all dictionaries" << '\n';
  out << "  merge <d1> <d2> <new>    - Merge two dictionaries into a new one" << '\n';
  out << "  inter <d1> <d2>          - Show intersection of two dictionaries" << '\n';
  out << "  diff <d1> <d2>           - Show words unique to first dictionary" << '\n';
  out << "  stat <dict>              - Show statistics about a dictionary" << '\n';
  out << "  top <dict> [N=5]         - Show N (default 5) most frequent words" << '\n';
  out << "  bot <dict> [N=5]         - Show N (default 5) least frequent words" << '\n';
  out << "  range <dict> [min] [max] - Show words in frequency range (0.0-1.0)" << '\n';
  out << "  find <dict> <regexp>     - Search for words\n";
  out << '\n';
  out << "State Information:" << '\n';
  out << "  Program state is automatically saved to: " << getStateFile() << '\n';
  out << "  Default state directory locations:" << '\n';
  out << "    1. $XDG_STATE_HOME/freq_dict.state" << '\n';
  out << "    2. $HOME/.local/state/freq_dict.state" << '\n';
  out << "    3. /tmp/freq_dict.state (if neither of above is available)" << '\n';
}
