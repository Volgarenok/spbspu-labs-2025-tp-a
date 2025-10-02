#ifndef DELIMITER_IO_HPP
#define DELIMITER_IO_HPP

#include <iostream>

namespace cherepkov
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}

#endif
