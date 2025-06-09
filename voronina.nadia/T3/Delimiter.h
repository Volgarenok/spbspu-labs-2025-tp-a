#ifndef DELIMITER_H
#define DELIMITER_H

#include <iostream>

namespace voronina
{
  struct DelimiterIO
  {
    char exp;
  };
  inline std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
    }
}
#endif
