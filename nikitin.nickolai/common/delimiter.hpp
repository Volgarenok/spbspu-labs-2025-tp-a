#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <iostream>

namespace nikitin
{
  struct DelimiterI
  {
    char exp_;
  };
  std::istream& operator>>(std::istream &, DelimiterI &&);
}

#endif
