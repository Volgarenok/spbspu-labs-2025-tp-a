#ifndef DELIMITER_H
#define DELIMITER_H

#include <iostream>

namespace voronina
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);

  template < char Delim, class T >
  struct PrefixedIO
  {
    T value;

    operator T() const
    {
      return value;
    }
  };

  template < char Delim, class T >
  std::istream& operator>>(std::istream& in, PrefixedIO< Delim, T >& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    if (in >> c && (c != Delim))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    T temp;
    if (in >> temp)
    {
      dest.value = std::move(temp);
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}
#endif
