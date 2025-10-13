#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <iostream>

namespace smirnov
{
  struct DelimiterCharI
  {
    char expected;
  };

  std::istream& operator>>(std::istream& in, const DelimiterCharI&& value);

  struct DelimiterStringI
  {
    const char* expected;
  };

  std::istream& operator>>(std::istream& in, const DelimiterStringI&& value);
}
#endif
