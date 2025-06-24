#include "command-processor.hpp"
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <set>
#include <sstream> // TODO: Remove sstream
#include "freq-dict.hpp"

kizhin::CommandProcessor::CommandProcessor(State& state, std::istream& in,
    std::ostream& out, std::ostream& err) noexcept:
  state_(state),
  in_(in),
  out_(out),
  err_(err)
{}

void kizhin::CommandProcessor::processCommands()
{
  using CmdContainer = std::map< std::string, std::function< void(const CmdArgs&) > >;
  const CmdContainer commands = {
    { "ls", std::bind(&CommandProcessor::handleLs, this, std::placeholders::_1) },
    { "add", std::bind(&CommandProcessor::handleAdd, this, std::placeholders::_1) },
    { "rm", std::bind(&CommandProcessor::handleRm, this, std::placeholders::_1) },
    { "mv", std::bind(&CommandProcessor::handleMv, this, std::placeholders::_1) },
    { "clear", std::bind(&CommandProcessor::handleClear, this, std::placeholders::_1) },
    { "merge", std::bind(&CommandProcessor::handleMerge, this, std::placeholders::_1) },
    { "inter", std::bind(&CommandProcessor::handleInter, this, std::placeholders::_1) },
    { "diff", std::bind(&CommandProcessor::handleDiff, this, std::placeholders::_1) },
    { "stat", std::bind(&CommandProcessor::handleStat, this, std::placeholders::_1) },
    { "top", std::bind(&CommandProcessor::handleTop, this, std::placeholders::_1) },
    { "bot", std::bind(&CommandProcessor::handleBot, this, std::placeholders::_1) },
    { "range", std::bind(&CommandProcessor::handleRange, this, std::placeholders::_1) },
    { "find", std::bind(&CommandProcessor::handleFind, this, std::placeholders::_1) },
  };
  printPrompt();
  for (CmdContainer::key_type currLine{}; std::getline(in_, currLine); printPrompt()) {
    if (currLine.empty()) {
      continue;
    }
    std::string command{};
    CmdArgs args{};
    std::tie(command, args) = parseCommand(currLine);
    if (!commands.count(command)) {
      err_ << "Unknown command: " << command << '\n';
      continue;
    }
    try {
      commands.at(command)(args);
    } catch (const std::logic_error& e) {
      err_ << "Error: " << e.what() << '\n';
    }
  }
  out_ << '\n';
}

std::pair< std::string, kizhin::CommandProcessor::CmdArgs > kizhin::CommandProcessor::
    parseCommand(const std::string& line) const
{
  std::string command{};
  std::stringstream sin(line); // TODO: Remove stringstream
  sin >> command;
  using InIt = std::istream_iterator< std::string >;
  CmdArgs args(InIt{ sin }, InIt{});
  return std::make_pair(command, args);
}

void kizhin::CommandProcessor::printPrompt()
{
  static const std::string orange = "\033[0;33m";
  static const std::string reset = "\033[0m";
  static const std::string prompt = "[freq-dict]>";
  out_ << orange << prompt << reset << ' ' << std::flush;
}

void kizhin::CommandProcessor::handleLs(const CmdArgs& args) const
{
  // TODO: Refactor
  if (args.empty()) {
    for (const auto& dict: state_) {
      out_ << dict.first << '\n';
    }
  }
  for (const auto& dictName: args) {
    if (!state_.count(dictName)) {
      err_ << "Unknown dictionary: " << dictName << '\n';
      return;
    }
  }
  for (const auto& dictName: args) {
    out_ << dictName << ": ";
    const auto& files = state_[dictName];
    if (files.empty()) {
      out_ << "[empty]\n";
      continue;
    }
    out_ << files.front();
    for (auto i = ++files.begin(), e = files.end(); i != e; ++i) {
      out_ << ", " << *i;
    }
    out_ << '\n';
  }
}

void kizhin::CommandProcessor::handleAdd(const CmdArgs& args)
{
  if (args.empty()) {
    err_ << "Usage: add <dict> [file]...\n";
    return;
  }
  auto& files = state_[args[0]];
  files.insert(files.end(), std::next(args.begin()), args.end());
}

void kizhin::CommandProcessor::handleRm(const CmdArgs& args)
{
  if (args.size() != 1) {
    err_ << "Usage: rm <dict>\n";
    return;
  }
  const auto pos = state_.find(args[0]);
  if (pos == state_.end()) {
    err_ << "No suce dictionary: " << args[0] << '\n';
    return;
  }
  state_.erase(pos);
}

void kizhin::CommandProcessor::handleMv(const CmdArgs& args)
{
  if (args.size() != 2) {
    err_ << "Usage: mv <old-dict> <new-dict>\n";
  } else if (!state_.count(args[0])) {
    err_ << "No such dictionary: " << args[0] << '\n';
  } else if (args[0] == args[1]) {
    err_ << "Same dictionary given\n";
  } else {
    state_[args[1]] = std::move(state_[args[0]]);
    state_.erase(args[0]);
  }
}

void kizhin::CommandProcessor::handleClear(const CmdArgs& args)
{
  if (!args.empty()) {
    err_ << "Usage: clear\n";
    return;
  } else if (!state_.empty()) {
    return;
  }
  out_ << "Are you sure want to delete all (";
  out_ << state_.size() << ") dictionaries? (y/N): ";
  std::string line{};
  if (std::getline(in_, line) && std::tolower(line.front()) == 'y') {
    state_.clear();
  }
}

void kizhin::CommandProcessor::handleMerge(const CmdArgs& args)
{
  if (args.size() != 3) {
    err_ << "Usage: merge <dict1> <dict2> <new-dict>\n";
    return;
  }
  const std::string& first = args[0];
  const std::string& second = args[1];
  const std::string& resultDict = args[2];
  if (!state_.count(first) || !state_.count(second)) {
    err_ << "Unknown dictionary: " << args[state_.count(first)] << '\n';
    return;
  }
  if (state_.count(resultDict)) {
    err_ << "Dictionary already exists: " << resultDict << '\n';
    return;
  }
  const auto& firstFiles = state_[first];
  const auto& secondFiles = state_[second];
  std::set< std::string > path(firstFiles.begin(), firstFiles.end());
  path.insert(secondFiles.begin(), secondFiles.end());
  state_[resultDict].assign(path.begin(), path.end());
}

void kizhin::CommandProcessor::handleInter(const CmdArgs& args) const
{
  if (args.size() != 2) {
    err_ << "Usage: inter <dict1> <dict2>\n";
    return;
  }
  if (!state_.count(args[0]) || !state_.count(args[1])) {
    err_ << "Unknown dictionary: " << args[state_.count(args[0])] << '\n';
    return;
  }
  const FrequencyDictionary first = loadDictionary(state_[args[0]]);
  const FrequencyDictionary second = loadDictionary(state_[args[1]]);
  FrequencyDictionary::FreqVector resultDict{};
  std::size_t resultTotal = 0;
  for (const auto& word: first.byFrequency) {
    if (second.byWord.count(word.second)) {
      const std::size_t count = word.first + second.byWord.at(word.second);
      resultTotal += count;
      resultDict.emplace_back(count, word.second);
    }
  }
  for (const auto& word: resultDict) {
    const float currFr = static_cast< float >(word.first) / resultTotal;
    out_ << std::fixed << std::setprecision(3);
    out_ << word.first << '\t' << currFr << '\t' << word.second << '\n';
  }
}

void kizhin::CommandProcessor::handleDiff(const CmdArgs& args) const
{
  if (args.size() != 2) {
    err_ << "Usage: diff <dict1> <dict2>\n";
    return;
  }
  if (!state_.count(args[0]) || !state_.count(args[1])) {
    err_ << "Unknown dictionary: " << args[state_.count(args[0])] << '\n';
    return;
  }
  const FrequencyDictionary first = loadDictionary(state_[args[0]]);
  const FrequencyDictionary second = loadDictionary(state_[args[1]]);
  FrequencyDictionary::FreqVector resultDict{};
  std::size_t resultTotal = 0;
  for (const auto& word: first.byFrequency) {
    if (!second.byWord.count(word.second)) {
      resultDict.push_back(word);
      resultTotal += word.first;
    }
  }
  for (const auto& word: resultDict) {
    const float currFr = static_cast< float >(word.first) / resultTotal;
    out_ << std::fixed << std::setprecision(3);
    out_ << word.first << '\t' << currFr << '\t' << word.second << '\n';
  }
}

void kizhin::CommandProcessor::handleStat(const CmdArgs& args) const
{
  if (args.size() != 1) {
    err_ << "Usage: stat <dict>\n";
    return;
  } else if (!state_.count(args[0])) {
    err_ << "Unknown dictionary: " << args[0] << '\n';
    return;
  }
  // TODO: Refactor
  const FrequencyDictionary dict = loadDictionary(state_.at(args[0]));
  const auto& freqDict = dict.byFrequency;
  if (freqDict.empty()) {
    out_ << "Dictionary is empty\n";
    return;
  }
  constexpr unsigned titleWidth = 15;
  constexpr unsigned countWidth = 15;
  out_ << std::setw(titleWidth) << std::left << "Unique words";
  out_ << std::setw(countWidth) << std::left << freqDict.size() << '\n';
  out_ << std::setw(titleWidth) << std::left << "Total words";
  out_ << std::setw(countWidth) << std::left << dict.total << '\n';
  out_ << std::setw(titleWidth) << std::left << "Most frequent";
  out_ << std::setw(countWidth) << std::left << freqDict.front().first;
  for (auto i = freqDict.begin();
      i->first == freqDict.front().first && i != freqDict.end(); ++i) {
    out_ << ' ' << i->second;
  }
  out_ << '\n' << std::setw(titleWidth) << std::left << "Rarest";
  out_ << std::setw(countWidth) << std::left << freqDict.back().first << ' ';
  auto i = freqDict.rbegin();
  while (i->first == freqDict.back().first && i != freqDict.rend()) {
    ++i;
  }
  out_ << i.base()->second;
  for (auto j = i.base() + 1; j != freqDict.end(); ++j) {
    out_ << ' ' << j->second;
  }
  out_ << '\n';
}

void kizhin::CommandProcessor::handleTop(const CmdArgs& args) const
{
  if (args.empty() || args.size() > 2) {
    err_ << "Usage: top <dict> [N]\n";
    return;
  } else if (!state_.count(args[0])) {
    err_ << "Unknown dictionary: " << args[0] << '\n';
    return;
  }
  const std::size_t count = args.size() == 2 ? std::stoull(args[1]) : 5;
  const FrequencyDictionary dict = loadDictionary(state_[args[0]]);
  const auto& freqDict = dict.byFrequency;
  auto end = count < freqDict.size() ? freqDict.begin() + count : freqDict.end();
  for (auto i = freqDict.begin(); i != end; ++i) {
    out_ << i->first << '\t' << i->second << '\n';
  }
}

void kizhin::CommandProcessor::handleBot(const CmdArgs& args) const
{
  if (args.empty() || args.size() > 2) {
    err_ << "Usage: top <dict> [N]\n";
    return;
  } else if (!state_.count(args[0])) {
    err_ << "Unknown dictionary: " << args[0] << '\n';
    return;
  }
  const std::size_t count = args.size() == 2 ? std::stoull(args[1]) : 5;
  FrequencyDictionary dict = loadDictionary(state_[args[0]]);
  const auto& freqDict = dict.byFrequency;
  auto end = count < freqDict.size() ? freqDict.rbegin() + count : freqDict.rend();
  for (auto i = freqDict.rbegin(); i != end; ++i) {
    out_ << i->first << '\t' << i->second << '\n';
  } // TODO: Reverse sorted
}

void kizhin::CommandProcessor::handleRange(const CmdArgs& args) const
{
  if (args.empty() || args.size() > 3) {
    err_ << "Usage: range <dict> [min] [max]";
    return;
  } else if (!state_.count(args[0])) {
    err_ << "Unknown dictionary: " << args[0] << '\n';
    return;
  }
  const float min = args.size() > 1 ? std::stof(args[1]) : 0.45;
  const float max = args.size() > 2 ? std::stof(args[2]) : 0.55;
  constexpr float epsilon = std::numeric_limits< float >::epsilon();
  if (min - max > epsilon || min - 1 > epsilon || max - 1 > epsilon) {
    err_ << "Requirements: 0.0 <= min <= max <= 1.0\n";
    return;
  }
  const FrequencyDictionary dict = loadDictionary(state_[args[0]]);
  const auto& freqDict = dict.byFrequency;
  for (const auto& word: freqDict) {
    const float currFr = static_cast< float >(word.first) / dict.total;
    if (currFr - min < epsilon) {
      continue;
    }
    if (currFr - max <= epsilon) {
      out_ << std::fixed << std::setprecision(3);
      out_ << word.first << '\t' << currFr << '\t' << word.second << '\n';
    }
  } // TODO: Obviously unefficiently
}

void kizhin::CommandProcessor::handleFind(const CmdArgs& /*args*/) const
{
  // TODO: Implement find
}

