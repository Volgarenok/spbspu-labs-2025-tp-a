#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <functional>
#include <map>
#include <string>
#include "polygon.hpp"

using namespace std::placeholders;

namespace puzikov
{
  struct CommandHandler
  {
  public:
    void readCommands(std::istream &in, std::ostream &out);
    CommandHandler(std::vector< Polygon > &refPolys);

  private:
    std::string cmd;
    std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands;
  };
}

#endif
