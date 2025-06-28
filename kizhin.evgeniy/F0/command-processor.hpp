#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_COMMAND_PROCESSOR_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_COMMAND_PROCESSOR_HPP

#include <iosfwd>
#include <string>
#include <utility>
#include <vector>
#include "freq-dict.hpp"
#include "state.hpp"

namespace kizhin {
  class CommandProcessor
  {
  public:
    CommandProcessor(State&, std::istream&, std::ostream&, std::ostream&) noexcept;
    void processCommands();

  private:
    using CmdArgs = std::vector< std::string >;

    State& state_;
    std::istream& in_;
    std::ostream& out_;
    std::ostream& err_;

    std::pair< std::string, CmdArgs > parseCommand(const std::string&) const;
    void printPrompt();

    void handleLs(const CmdArgs&) const;
    void handleAdd(const CmdArgs&);
    void handleRm(const CmdArgs&);
    void handleMv(const CmdArgs&);
    void handleClear(const CmdArgs&);

    void handleMerge(const CmdArgs&);
    void handleInter(const CmdArgs&) const;
    void handleDiff(const CmdArgs&) const;

    void handleStat(const CmdArgs&) const;
    void handleTop(const CmdArgs&) const;
    void handleBot(const CmdArgs&) const;
    void handleRange(const CmdArgs&) const;
    void handleFind(const CmdArgs&) const;

    void outDictionary(const State::key_type&) const;
    FrequencyDictionary loadDictionary(const std::vector< std::string >&) const;
  };
}

#endif

