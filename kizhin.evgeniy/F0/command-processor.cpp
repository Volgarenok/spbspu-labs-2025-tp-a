#include "command-processor.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <set>
#include <sstream>
#include <tuple>
#include "freq-dict.hpp"

namespace kizhin {
  bool isSatisfied(const std::string& str, const std::string& regexp);
  std::string wordAndSizeToString(const WordAndSize&);
  float getFreq(const FrequencyDictionary&, const WordAndSize&);
  void outWordInfo(std::ostream&, const FrequencyDictionary&, const WordAndSize&);
}

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
  using std::placeholders::_1;
  static const CmdContainer commands = {
    { "ls", std::bind(&CommandProcessor::handleLs, this, _1) },
    { "add", std::bind(&CommandProcessor::handleAdd, this, _1) },
    { "rm", std::bind(&CommandProcessor::handleRm, this, _1) },
    { "mv", std::bind(&CommandProcessor::handleMv, this, _1) },
    { "clear", std::bind(&CommandProcessor::handleClear, this, _1) },
    { "merge", std::bind(&CommandProcessor::handleMerge, this, _1) },
    { "inter", std::bind(&CommandProcessor::handleInter, this, _1) },
    { "diff", std::bind(&CommandProcessor::handleDiff, this, _1) },
    { "stat", std::bind(&CommandProcessor::handleStat, this, _1) },
    { "top", std::bind(&CommandProcessor::handleTop, this, _1) },
    { "bot", std::bind(&CommandProcessor::handleBot, this, _1) },
    { "range", std::bind(&CommandProcessor::handleRange, this, _1) },
    { "find", std::bind(&CommandProcessor::handleFind, this, _1) },
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
  std::stringstream sin(line);
  sin >> command;
  using InIt = std::istream_iterator< std::string >;
  CmdArgs args(InIt{ sin }, InIt{});
  return std::make_pair(command, args);
}

void kizhin::CommandProcessor::printPrompt()
{
  constexpr static auto orange = "\033[0;33m";
  constexpr static auto reset = "\033[0m";
  constexpr static auto prompt = "[freq-dict]>";
  out_ << orange << prompt << reset << ' ' << std::flush;
}

void kizhin::CommandProcessor::handleLs(const CmdArgs& args) const
{
  using std::placeholders::_1;
  if (args.empty()) {
    using OutIt = std::ostream_iterator< State::key_type >;
    static const auto getFirst = std::bind(&State::value_type::first, _1);
    std::transform(state_.begin(), state_.end(), OutIt{ out_, "\n" }, getFirst);
  }
  static size_t (State::*countPtr)(const std::string&) const = &State::count;
  static const auto checker = std::bind(countPtr, std::addressof(state_), _1);
  const auto pos = std::find_if_not(args.begin(), args.end(), checker);
  if (pos != args.end()) {
    err_ << "Unknown dictionary: " << *pos << '\n';
    return;
  }
  static const auto outDict = std::bind(&CommandProcessor::outDictionary, this, _1);
  std::for_each(args.begin(), args.end(), outDict);
}

void kizhin::CommandProcessor::handleAdd(const CmdArgs& args)
{
  if (args.empty()) {
    err_ << "Usage: add <dict> [file...]\n";
    return;
  }
  State::mapped_type& files = state_[args[0]];
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
    err_ << "Unknown dictionary: " << args[0] << '\n';
    return;
  }
  state_.erase(pos);
}

void kizhin::CommandProcessor::handleMv(const CmdArgs& args)
{
  if (args.size() != 2) {
    err_ << "Usage: mv <old-dict> <new-dict>\n";
  } else if (!state_.count(args[0])) {
    err_ << "Unknown dictionary: " << args[0] << '\n';
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
  } else if (state_.empty()) {
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
  std::set< std::string > intersection{};
  const auto ins = std::inserter(intersection, intersection.end());
  const WordSet& firstW = first.wordSet;
  const WordSet& secondW = second.wordSet;
  const auto beg = firstW.begin();
  std::set_intersection(beg, firstW.end(), secondW.begin(), secondW.end(), ins);
  WordMap result{};
  const auto inserter = std::inserter(result, result.end());
  std::size_t (WordSet::*count)(const std::string&) const = &WordSet::count;
  using std::placeholders::_1;
  static const auto getFirst = std::bind(&WordMap::value_type::first, _1);
  const auto interPtr = std::addressof(intersection);
  const auto contains = std::bind(count, interPtr, std::bind(getFirst, _1));
  std::copy_if(first.wordMap.begin(), first.wordMap.end(), inserter, contains);
  using OutIt = std::ostream_iterator< std::string >;
  std::transform(result.begin(), result.end(), OutIt{ out_, "\n" }, wordAndSizeToString);
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
  using std::placeholders::_1;
  std::set< std::string > difference{};
  const auto ins = std::inserter(difference, difference.end());
  const WordSet& firstW = first.wordSet;
  const WordSet& secondW = second.wordSet;
  std::set_difference(firstW.begin(), firstW.end(), secondW.begin(), secondW.end(), ins);
  WordMap result{};
  const auto inserter = std::inserter(result, result.end());
  std::size_t (WordSet::*count)(const std::string&) const = &WordSet::count;
  static const auto getFirst = std::bind(&WordMap::value_type::first, _1);
  const auto diffPtr = std::addressof(difference);
  const auto contains = std::bind(count, diffPtr, std::bind(getFirst, _1));
  std::copy_if(first.wordMap.begin(), first.wordMap.end(), inserter, contains);
  using OutIt = std::ostream_iterator< std::string >;
  std::transform(result.begin(), result.end(), OutIt{ out_, "\n" }, wordAndSizeToString);
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
  const FrequencyDictionary dict = loadDictionary(state_.at(args[0]));
  const SizeSet& freqDict = dict.sizeSet;
  if (freqDict.empty()) {
    out_ << "Dictionary is empty\n";
    return;
  }
  constexpr static unsigned titleWidth = 15;
  constexpr static unsigned countWidth = 15;
  out_ << std::setw(titleWidth) << std::left << "Unique words";
  out_ << std::setw(countWidth) << std::left << freqDict.size() << '\n';
  out_ << std::setw(titleWidth) << std::left << "Total words";
  out_ << std::setw(countWidth) << std::left << dict.total << '\n';
  out_ << std::setw(titleWidth) << std::left << "Most frequent";
  out_ << std::setw(countWidth) << std::left << freqDict.begin()->second;
  using std::placeholders::_1;
  static const auto getFirst = std::bind(&SizeSet::value_type::first, _1);
  static const auto getSecond = std::bind(&SizeSet::value_type::second, _1);
  constexpr static auto eqTo = std::equal_to<>{};
  const auto frontEq = std::bind(eqTo, getSecond, freqDict.begin()->second);
  const auto backEq = std::bind(eqTo, getSecond, std::prev(freqDict.end())->second);
  const auto frontEnd = std::find_if_not(freqDict.begin(), freqDict.end(), frontEq);
  const auto backBegin = std::find_if_not(freqDict.rbegin(), freqDict.rend(), backEq);
  using OutIt = std::ostream_iterator< std::string >;
  std::transform(freqDict.begin(), std::prev(frontEnd), OutIt{ out_, " " }, getFirst);
  out_ << std::prev(frontEnd)->first << '\n';
  out_ << std::setw(titleWidth) << std::left << "Rarest";
  out_ << std::setw(countWidth) << std::left << std::prev(freqDict.end())->second;
  const auto backEnd = std::prev(freqDict.end());
  std::transform(backBegin.base(), backEnd, OutIt{ out_, " " }, getFirst);
  out_ << backEnd->first << '\n';
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
  const SizeSet& freqDict = dict.sizeSet;
  const bool beforeEnd = count < freqDict.size();
  const auto end = beforeEnd ? std::next(freqDict.begin(), count) : freqDict.end();
  using OutIt = std::ostream_iterator< std::string >;
  std::transform(freqDict.begin(), end, OutIt{ out_, "\n" }, wordAndSizeToString);
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
  const SizeSet& freqDict = dict.sizeSet;
  const bool beforeEnd = count < freqDict.size();
  const auto beg = beforeEnd ? std::next(freqDict.rbegin(), count) : freqDict.rend();
  using OutIt = std::ostream_iterator< std::string >;
  std::transform(beg.base(), freqDict.end(), OutIt{ out_, "\n" }, wordAndSizeToString);
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
  const float min = args.size() > 1 ? std::stof(args[1]) : 0;
  const float max = args.size() > 2 ? std::stof(args[2]) : 1;
  constexpr static float epsilon = std::numeric_limits< float >::epsilon();
  if (min - max > epsilon || min - 1 > epsilon || max - 1 > epsilon) {
    err_ << "Requirements: 0.0 <= min <= max <= 1.0\n";
    return;
  }
  const FrequencyDictionary dict = loadDictionary(state_[args[0]]);
  const SizeSet& freqDict = dict.sizeSet;
  const auto begin = freqDict.upper_bound({ "", max * dict.total });
  const auto end = freqDict.lower_bound({ "", min * dict.total });
  using std::placeholders::_1;
  const auto printer = std::bind(&outWordInfo, std::ref(out_), std::cref(dict), _1);
  std::for_each(begin, end, printer);
}

void kizhin::CommandProcessor::handleFind(const CmdArgs& args) const
{
  if (args.size() != 2) {
    err_ << "Usage: find <dict> <regexp>\n";
    return;
  } else if (!state_.count(args[0])) {
    err_ << "Unknown dictionary: " << args[0] << '\n';
    return;
  }
  const FrequencyDictionary dict = loadDictionary(state_.at(args[0]));
  const WordMap& wordMap = dict.wordMap;
  WordMap result{};
  using std::placeholders::_1;
  const auto inserter = std::inserter(result, result.end());
  static const auto getFirst = std::bind(&WordMap::value_type::first, _1);
  static const auto validator = std::addressof(isSatisfied);
  const auto isRight = std::bind(validator, std::bind(getFirst, _1), std::cref(args[1]));
  std::copy_if(wordMap.begin(), wordMap.end(), inserter, isRight);
  using OutIt = std::ostream_iterator< std::string >;
  std::transform(result.begin(), result.end(), OutIt{ out_, "\n" }, wordAndSizeToString);
}

void kizhin::CommandProcessor::outDictionary(const State::key_type& dictionary) const
{
  out_ << dictionary << ":\n";
  const State::mapped_type& files = state_.at(dictionary);
  if (!files.empty()) {
    using OutIt = std::ostream_iterator< std::string >;
    std::copy(files.begin(), std::prev(files.end()), OutIt{ out_, ", " });
    out_ << files.back();
  }
  out_ << '\n';
}

kizhin::FrequencyDictionary kizhin::CommandProcessor::loadDictionary(
    const std::vector< std::string >& files) const
{
  FrequencyDictionary result{};
  using std::placeholders::_1;
  const auto expander = std::bind(std::addressof(expandDictionary), _1, std::ref(result));
  std::vector< std::ifstream > fins(files.begin(), files.end());
  std::for_each(fins.begin(), fins.end(), expander);
  return result;
}

bool kizhin::isSatisfied(const std::string& str, const std::string& reg)
{
  auto strPos = str.begin();
  auto regPos = reg.begin();
  std::tie(strPos, regPos) = std::mismatch(strPos, str.end(), regPos, reg.end());
  while (strPos != str.end() && regPos != reg.end()) {
    if (*regPos == '*') {
      strPos = std::find(strPos, str.end(), *(++regPos));
    } else {
      return false;
    }
    std::tie(strPos, regPos) = std::mismatch(strPos, str.end(), regPos, reg.end());
  }
  const bool isRegEnd = regPos == reg.end();
  const bool doesMathcAny = (*regPos == '*' && std::next(regPos) == reg.end());
  return strPos == str.end() && (isRegEnd || doesMathcAny);
}

std::string kizhin::wordAndSizeToString(const WordAndSize& val)
{
  return std::to_string(val.second) + '\t' + val.first;
}

float kizhin::getFreq(const FrequencyDictionary& dict, const WordAndSize& word)
{
  return static_cast< float >(word.second) / dict.total;
};

void kizhin::outWordInfo(std::ostream& out, const FrequencyDictionary& dict,
    const WordAndSize& word)
{
  out << std::fixed << std::setprecision(3);
  out << word.second << '\t' << getFreq(dict, word) << '\t' << word.first << '\n';
}

