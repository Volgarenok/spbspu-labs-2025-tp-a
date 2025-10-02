#ifndef OUTPUT_STRUCT_HPP
#define OUTPUT_STRUCT_HPP

#include <ostream>

namespace mazitov
{
  struct UnsignedLongLongBinOutput
  {
    const unsigned long long &ref;
  };

  unsigned calcBitWidth(unsigned long long num);
  std::ostream &operator<<(std::ostream &out, const UnsignedLongLongBinOutput &dest);
}

#endif
