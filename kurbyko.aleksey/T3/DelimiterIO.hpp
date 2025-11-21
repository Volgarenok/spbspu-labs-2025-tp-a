#ifndef DELIMITER_IO_HPP
#define DELIMITER_IO_HPP

#include <istream>

namespace kurbyko
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}

#endif
