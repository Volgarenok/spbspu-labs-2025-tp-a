#ifndef IO_DELIMITER_HPP
#define IO_DELIMITER_HPP
#include <iostream>

namespace savintsev
{
  struct DelimiterIO
  {
    char exp_;
  };

  std::istream & operator>>(std::istream & in, DelimiterIO && dest);
}

#endif
