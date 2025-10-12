#ifndef DELIMITER_HPP
#define DELIMITER_HPP
#include <istream>

namespace dribas
{
  struct DelimiterI
  {
    char exp;
  };

  std::istream& operator>>(std::istream&, DelimiterI&&);
}

#endif
