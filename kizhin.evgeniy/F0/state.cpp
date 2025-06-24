#include "state.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream> // TODO: Remove sstream

namespace kizhin {
  std::string getStateDir()
  {
    const char* xdgStateHome = std::getenv("XDG_STATE_HOME");
    if (xdgStateHome) {
      return xdgStateHome;
    }
    const char* home = std::getenv("HOME");
    if (home) {
      return std::string(home) + "/.local/state";
    }
    return "/tmp";
  }
}

std::string kizhin::getStateFile()
{
  return getStateDir() + "/freq_dict.state";
}

kizhin::State kizhin::loadState(std::istream& in)
{
  // TODO: Refactor
  State result{};
  std::string currLine;
  while (std::getline(in, currLine)) {
    std::istringstream sin(currLine);
    std::string currName;
    std::string currFile;
    if (std::getline(sin, currName, ',')) {
      auto& currPaths = result[currName];
      while (std::getline(sin, currFile, ',')) {
        currPaths.push_back(currFile);
      }
    }
  }
  return result;
}

void kizhin::saveState(std::ostream& out, const State& state)
{
  for (const auto& dict: state) {
    const auto& dictName = dict.first;
    const auto& files = dict.second;
    out << dictName;
    for (const auto& filePath: files) {
      out << ',' << filePath;
    }
    out << '\n';
  }
}

