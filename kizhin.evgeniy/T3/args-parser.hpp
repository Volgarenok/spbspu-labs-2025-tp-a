#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T3_ARGS_PARSER_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T3_ARGS_PARSER_HPP

#include <iosfwd>
#include <string>

namespace kizhin {
  struct Args
  {
    std::string filename;
  };
  Args parseArgs(int, const char* const*, std::ostream&);
}

#endif

