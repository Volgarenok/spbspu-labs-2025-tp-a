#ifndef DELIMITER_HPP
#define DELIMITER_HPP
#include <iosfwd>

namespace kostyukov
{
  struct DelimiterIO
  {
    char expected;
  };
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}
#endif
