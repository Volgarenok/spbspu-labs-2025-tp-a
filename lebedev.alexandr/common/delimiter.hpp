#ifndef IODATA_HPP
#define IODATA_HPP
#include <istream>

namespace lebedev
{
  struct DelimiterIO
  {
    char obj;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}

#endif
