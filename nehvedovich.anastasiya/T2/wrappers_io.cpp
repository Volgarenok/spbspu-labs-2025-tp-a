#include "wrappers_io.hpp"

#include <iostream>

std::istream &nehvedovich::operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = 0;
  if ((in >> c) && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &nehvedovich::operator>>(std::istream &in, UnsignedLongLongIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  unsigned long long num = 0;
  char a = 0, b = 0, c = 0;
  char temp = 0;
  bool gotdigit = false;

  while (std::isdigit(in.peek()))
  {
    in >> temp;
    gotdigit = true;
    num *= 10;
    num += (temp - '0');
  }

  if (!gotdigit)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (in >> a >> b >> c && std::tolower(a) == 'u' && std::tolower(b) == 'l' && std::tolower(c) == 'l')
  {
    dest.ref = num;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream &nehvedovich::operator>>(std::istream &in, CharIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{'\''};
  char c = 0;
  in >> c;
  in >> DelimiterIO{'\''};
  if (in)
  {
    dest.ref = c;
  }
  return in;
}

std::istream &nehvedovich::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}
