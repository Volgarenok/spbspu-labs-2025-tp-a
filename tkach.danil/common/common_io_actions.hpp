#ifndef COMMON_IO_ACTIONS
#define COMMON_IO_ACTIONS

#include <cstddef>
#include <iostream>

namespace tkach
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}

#endif
