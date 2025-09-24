#ifndef DELIMETER_HPP
#define DELIMETER_HPP
#include <iostream>

namespace sveshnikov
{
  struct Delimiter
  {
    char exp;
  };

  std::istream &operator>>(std::istream &in, Delimiter &&dest);
}

#endif
