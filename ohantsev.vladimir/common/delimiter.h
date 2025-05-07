#ifndef DELIMITER_H
#define DELIMITER_H
#include <iosfwd>

namespace ohantsev
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}
#endif
