#ifndef DELIMITERS_H
#define DELIMITERS_H
#include <string>

namespace mezentsev
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& input, DelimiterIO&& dest);
}
#endif
