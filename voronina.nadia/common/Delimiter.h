#ifndef DELIMITER_H
#define DELIMITER_H

#include <iostream>

namespace voronina
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}
#endif
