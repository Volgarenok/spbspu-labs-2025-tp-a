#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <functional>
#include <map>
#include <string>
#include "polygon.hpp"

namespace puzikov
{
  struct CommandHandler
  {
  public:
    explicit CommandHandler(std::vector< Polygon > &refPolys);
    void readCommands(std::istream &in, std::ostream &out);
    void restoreInputStream(std::istream &in);

  private:
    std::string cmd;
    std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands;
  };
}

#endif
