#include "DelimIO.hpp"

std::istream &holodilov::operator>>(std::istream &in, DelimIO &&delim)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char ch;
  in >> ch;
  if (ch != delim.delim)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
