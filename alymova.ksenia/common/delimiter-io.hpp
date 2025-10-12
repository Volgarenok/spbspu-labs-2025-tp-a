#ifndef DELIMITER_IO_HPP
#define DELIMITER_IO_HPP
#include <iostream>

namespace alymova
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& object);
}

#endif
