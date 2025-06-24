#include "state.hpp"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream> // TODO: Remove sstream

namespace kizhin {
  std::string getStateDir();
  void saveDict(std::ostream&, State::const_reference);
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
  using std::placeholders::_1;
  const auto saver = std::bind(std::addressof(saveDict), std::ref(out), _1);
  std::for_each(state.begin(), state.end(), saver);
}

std::string kizhin::getStateDir()
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

void kizhin::saveDict(std::ostream& out, State::const_reference dict)
{
  const auto& dictName = dict.first;
  const auto& files = dict.second;
  out << dictName;
  if (files.empty()) {
    out << '\n';
    return;
  }
  using OutIt = std::ostream_iterator< std::string >;
  std::copy(files.begin(), std::prev(files.end()), OutIt{ out << ',', "," });
  out << files.back() << '\n';
}
