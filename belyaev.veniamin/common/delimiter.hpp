#ifndef DELIMITER_HPP
#define DELIMITER_HPP
#include <iostream>

namespace belyaev
{
  struct DelimiterIO
  {
    char expected;
  };

  std::istream& operator>>(std::istream& in, const DelimiterIO&& dest);
}

#endif
