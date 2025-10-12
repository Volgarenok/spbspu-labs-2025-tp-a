#ifndef DELIMITER
#define DELIMITER

#include <iostream>

namespace fedorov
{
  struct DelimiterStr
  {
    const char *exp;
  };
  std::istream &operator>>(std::istream &in, DelimiterStr &&exp);
}

#endif
