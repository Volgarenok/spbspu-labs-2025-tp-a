#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <iostream>

namespace milchevskiy
{
  struct DelimiterI
  {
    char exp_;
  };
  std::istream& operator>>(std::istream&, DelimiterI&&);
}

#endif
