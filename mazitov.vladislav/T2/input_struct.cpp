#include "input_struct.hpp"
#include "stream_guard.hpp"

std::istream& mazitov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '\0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& mazitov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& mazitov::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref;
  if (!in)
  {
    return in;
  }

  char literal;
  in >> literal;
  if (literal != 'd' && literal != 'D')
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& mazitov::operator>>(std::istream& in, UnsignedLongLongBinIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{ '0' } >> DelimiterIO{ 'b' };

  char c = 0;
  bool flag = false;
  unsigned long long binaryValue = 0;
  while (in >> c && (c == '1' || c == '0'))
  {
    flag = true;
    binaryValue = (binaryValue << 1) | (c == '1' ? 1 : 0);
  }

  if (!flag)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (c != ':')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  dest.ref = binaryValue;
  return in;
}

