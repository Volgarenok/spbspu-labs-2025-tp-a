#ifndef DELIM_IO_HPP
#define DELIM_IO_HPP

#include <iostream>

namespace holodilov
{
  struct DelimIO
  {
    char delim;
  };

  std::istream& operator>>(std::istream& in, DelimIO&& delim);
}

#endif
