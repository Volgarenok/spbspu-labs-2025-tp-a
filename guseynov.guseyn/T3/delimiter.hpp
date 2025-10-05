#ifndef DELIMITER_HPP
#define DELIMITER_HPP
#include <iostream>

namespace guseynov
{
  struct DelimiterI
  {
    char exp;
  };
  std::istream & operator>>(std::istream & in, DelimiterI && dest);
}
#endif
