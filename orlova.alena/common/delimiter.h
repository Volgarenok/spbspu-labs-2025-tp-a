#ifndef DELIMITER_H
#define DELIMITER_H

#include <istream>

namespace orlova
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}

#endif

