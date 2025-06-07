#include "command_handler.hpp"
#include <limits>
#include "commands.hpp"

void puzikov::CommandHandler::readCommands(std::istream &in, std::ostream &out)
{
  while (!(in >> cmd).eof())
  {
    try
    {
      commands.at(cmd)(in, out);
    }
    catch (...)
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      out << "<INVALID COMMAND>\n";
    }
  }
}

puzikov::CommandHandler::CommandHandler(std::vector< Polygon > &refPolys):
  commands{
        {"AREA", std::bind(puzikov::areaCommand, _1, _2, std::cref(refPolys))},
        {"MAX", std::bind(puzikov::maxCommand, _1, _2, std::cref(refPolys))},
        {"MIN", std::bind(puzikov::minCommand, _1, _2, std::cref(refPolys))},
        {"COUNT", std::bind(puzikov::countCommand, _1, _2, std::cref(refPolys))},
        {"RMECHO", std::bind(puzikov::rmEchoCommand, _1, _2, std::ref(refPolys))},
        {"SAME", std::bind(puzikov::sameCommand, _1, _2, std::cref(refPolys))}
    }
{};
