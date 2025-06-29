#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <iostream>

namespace mazitov
{
  struct DelimiterInput
  {
    char exp;
  };

  std::istream &operator>>(std::istream &in, DelimiterInput &&dest);
}

#endif
