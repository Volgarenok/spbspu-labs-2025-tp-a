#include "ioData.hpp"

std::istream& finaev::operator>>(std::istream& in, delimiterIO&& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != rhs.obj))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
