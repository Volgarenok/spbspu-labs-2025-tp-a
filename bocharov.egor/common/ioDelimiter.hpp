#ifndef IO_DELIMITER_HPP
#define IO_DELIMITER_HPP

#include <iosfwd>

namespace bocharov
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream & operator>>(std::istream & in, DelimiterIO && dest);
}

#endif
