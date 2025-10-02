#ifndef LOG_HPP
#define LOG_HPP

#include <cstddef>
#include <iosfwd>
#include <string>

namespace rychkov
{
  struct CParseContext
  {
    std::ostream& out;
    std::ostream& err;
    std::string file;
    CParseContext* base = nullptr;
    bool macro_expansion = false;
    size_t line = 0, symbol = 0;
    std::string last_line;
    size_t nerrors = 0;
  };
  void log(CParseContext& context, std::string message);
  std::ostream& start_log(CParseContext& context);
  void finish_log(CParseContext& context);
}

#endif
