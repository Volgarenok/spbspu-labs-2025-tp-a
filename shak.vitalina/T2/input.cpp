#include "input.hpp"
#include <delimiter.hpp>

using check = shak::DelimiterIO;
std::istream &shak::operator>>(std::istream &in, ChrLitIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> check{'\''} >> dest.ref >> check{'\''};
  return in;
}

std::istream &shak::operator>>(std::istream &in, RatLspIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  long long n = 0;
  unsigned long long d = 0;
  in >> check{'('} >> check{':'} >> check{'N'} >> n;
  in >> check{':'} >> check{'D'} >> d;
  in >> check{':'} >> check{')'};
  if (in)
  {
    dest.ref.first = n;
    dest.ref.second = d;
  }
  return in;
}

std::istream &shak::operator>>(std::istream &in, StrIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::getline(in >> check{'"'}, dest.ref, '"');
  return in;
}
