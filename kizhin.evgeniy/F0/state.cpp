#include "state.hpp"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <iterator>

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
  State result{};
  std::string line{};
  while (std::getline(in, line)) {
    const auto comma = line.find(',');
    if (comma == std::string::npos) {
      result[line];
      continue;
    }
    auto& paths = result[line.substr(0, comma)];
    auto start = line.begin() + comma + 1;
    while (start != line.end()) {
      const auto next = std::find(start, line.end(), ',');
      paths.emplace_back(start, next);
      start = next == line.end() ? line.end() : next + 1;
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
