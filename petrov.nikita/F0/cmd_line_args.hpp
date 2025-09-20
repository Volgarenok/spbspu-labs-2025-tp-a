#ifndef CMD_LINE_ARGS_HPP
#define CMD_LINE_ARGS_HPP

#include <iostream>

namespace petrov
{
  void outputHelp();
  void setPrecision(std::ostream & out, const size_t & precision);

  void processCmdLineAgruments(const int agrc, const char * const * argv);
}

#endif