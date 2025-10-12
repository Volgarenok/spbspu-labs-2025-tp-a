#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <functional>
#include <map>
#include <string>

namespace puzikov
{
  using CommandMap = std::map< std::string, std::function< void(std::istream &, std::ostream &) > >;

  void readCommands(std::istream &, std::ostream &, const CommandMap &);
  void restoreInputStream(std::istream &);
}

#endif
