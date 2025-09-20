#ifndef DELIMITERS_HPP
#define DELIMITERS_HPP

#include <iostream>
#include <string>

namespace smirnov
{
  struct DelimiterString
  {
    const char* expected;
  };

  std::istream& operator>>(std::istream& in, DelimiterString&& exp);

  struct DelimiterChar
  {
    char expected;
  };

  std::istream& operator>>(std::istream& in, DelimiterChar&& exp);
}

#endif
