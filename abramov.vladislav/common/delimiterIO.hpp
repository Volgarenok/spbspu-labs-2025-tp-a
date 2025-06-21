#ifndef DELIMITERIO_HPP
#define DELIMITERIO_HPP
#include <iomanip>

namespace abramov
{
  struct DelimiterIO
  {
    char delim;
  };
  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
}
#endif
