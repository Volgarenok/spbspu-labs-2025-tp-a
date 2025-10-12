#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <istream>

namespace lanovenko
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}

#endif
