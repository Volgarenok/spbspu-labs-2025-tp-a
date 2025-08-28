#ifndef DELIMETRS_HPP
#define DELIMETRS_HPP
#include <string>

namespace kharlamov
{
  struct DelimeterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& input, DelimeterIO&& dest);
}
#endif
