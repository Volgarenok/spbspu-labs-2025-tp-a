#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <iostream>

namespace pilugina
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
}

#endif
