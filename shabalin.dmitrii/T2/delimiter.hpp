#ifndef DELIMITER_H
#define DELIMITER_H

#include <iostream>

namespace shabalin
{
  struct DelimiterSym
  {
    char expected;
  };

  struct DelimiterStr
  {
    const char* expected;
  };

  std::istream& operator>>(std::istream& in, DelimiterSym&& exp);
  std::istream& operator>>(std::istream& in, DelimiterStr&& exp);
}
#endif
