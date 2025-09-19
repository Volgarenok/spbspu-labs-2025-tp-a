#ifndef DELIMETER_IO_HPP
#define DELIMETER_IO_HPP
#include <iostream>

namespace sveshnikov
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
}

#endif
