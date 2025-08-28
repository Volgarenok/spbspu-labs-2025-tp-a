#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <istream>

namespace mozhegova
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream & operator>>(std::istream & in, DelimiterIO && dest);
}

#endif
