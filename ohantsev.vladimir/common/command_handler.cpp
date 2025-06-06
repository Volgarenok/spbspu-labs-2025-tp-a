#include "command_handler.h"

namespace ohantsev
{
  CommandHandler::CommandHandler(std::istream& in, std::ostream& out):
    cmds_(),
    in_(in),
    out_(out)
  {}

  void CommandHandler::operator()()
  {
    std::string subcmd;
    in_ >> subcmd;
    run(subcmd);
  }

  bool CommandHandler::add(const std::string& name, std::function< void() > func)
  {
    return cmds_.emplace(name, std::move(func)).second;
  }

  bool CommandHandler::remove(const std::string& name)
  {
    return cmds_.erase(name);
  }

  void CommandHandler::run(const std::string& name)
  {
    cmds_.at(name)();
  }
}
