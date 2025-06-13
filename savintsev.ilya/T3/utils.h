#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <iostream>

namespace savintsev
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream & operator>>(std::istream & in, DelimiterIO && dest);
}

#endif
