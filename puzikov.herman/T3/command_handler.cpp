#include "command_handler.hpp"
#include <iostream>
#include <limits>
#include <functional>

using namespace std::placeholders;

namespace puzikov
{
  void readCommands(std::istream &in, std::ostream &out, const CommandMap &commands)
  {
    std::string cmd;
    while (!(in >> cmd).eof())
    {
      try
      {
        commands.at(cmd)(in, out);
      }
      catch (...)
      {
        restoreInputStream(in);
        out << "<INVALID COMMAND>\n";
      }
    }
  }

  void restoreInputStream(std::istream &in)
  {
    if (in.fail())
    {
      in.clear(in.rdstate() ^ std::ios::failbit);
    }
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
