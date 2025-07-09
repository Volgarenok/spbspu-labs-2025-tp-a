#ifndef DELIMETR_HPP
#define DELIMETR_HPP
#include <iostream>

namespace smirnov
{
  struct DelimiterI
  {
    char exp;
  };
  std::istream & operator>>(std::istream & in, DelimiterI && dest);
}
#endif
