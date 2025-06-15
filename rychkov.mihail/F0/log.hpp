#ifndef LOG_HPP
#define LOG_HPP

#include <iosfwd>
#include <cstddef>
#include <string>

namespace rychkov
{
  struct CParseContext
  {
    std::ostream& out;
    std::ostream& err;
    std::string file;
    CParseContext* base = nullptr;
    size_t line = 0, symbol = 0;
    std::string last_line;
    size_t nerrors = 0;
  };
  void log(CParseContext& context, std::string message);
}

#endif
