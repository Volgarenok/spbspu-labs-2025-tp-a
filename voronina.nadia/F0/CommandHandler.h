#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "ShannonFano.h"

namespace voronina
{
  class CommandHandler
  {
  public:
    CommandHandler();
    CommandHandler(const CommandHandler&) = delete;
    CommandHandler& operator=(const CommandHandler&) = delete;
    CommandHandler(CommandHandler&&) = delete;
    CommandHandler& operator=(CommandHandler&&) = delete;
    void executeCommand(const std::string& command, std::istream& input, std::ostream& output);

  private:
    std::vector< ShannonFanoTable > vectorOfTables_;
    std::vector< std::string > vectorOfOriginFiles_;
    std::map< std::string, std::function< void(std::istream&, std::ostream&) > > cmds_;
  };
}

#endif
