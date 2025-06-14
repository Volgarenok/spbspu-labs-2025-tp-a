#ifndef IODATA_HPP
#define IODATA_HPP
#include <istream>

namespace finaev
{
  struct delimiterIO
  {
    char obj;
  };

  std::istream& operator>>(std::istream& in, delimiterIO&& dest);
}

#endif
