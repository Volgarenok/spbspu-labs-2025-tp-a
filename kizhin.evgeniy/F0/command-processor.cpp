#include "command-processor.hpp"
#include <functional>
#include <iostream>
#include <iterator>
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

void kizhin::CommandProcessor::handleLs(const CmdArgs& /*args*/) const
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleAdd(const CmdArgs& /*args*/)
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleRm(const CmdArgs& /*args*/)
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleMv(const CmdArgs& /*args*/)
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleClear(const CmdArgs& /*args*/)
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleMerge(const CmdArgs& /*args*/)
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleInter(const CmdArgs& /*args*/) const
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleDiff(const CmdArgs& /*args*/) const
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleStat(const CmdArgs& /*args*/) const
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleTop(const CmdArgs& /*args*/) const
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleBot(const CmdArgs& /*args*/) const
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleRange(const CmdArgs& /*args*/) const
{
  // TODO: Implement
}

void kizhin::CommandProcessor::handleFind(const CmdArgs& /*args*/) const
{
  // TODO: Implement
}

