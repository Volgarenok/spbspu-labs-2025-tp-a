#ifndef DELIMITER_HPP
#define DELIMITER_HPP
#include <istream>

namespace horoshilov
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}
#endif

