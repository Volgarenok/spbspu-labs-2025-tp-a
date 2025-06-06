#include "format_wrapper.hpp"
#include <iostream>
#include "commands.hpp"

std::istream &pilugina::operator>>(std::istream &in, DelimiterIO &&dest)
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

std::istream &pilugina::operator>>(std::istream &in, UnsignedLongLongOCT &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char prefix = 0;
  if (!(in >> prefix) || prefix != '0')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  unsigned long long num = 0;
  bool gotDigit = false;
  char temp;

  while (isOctalDigit(in.peek()))
  {
    in >> temp;
    num = num * 8 + (temp - '0');
    gotDigit = true;
  }

  if (!gotDigit)
  {
    in.setstate(std::ios::failbit);
  }
  else
  {
    dest.ref = num;
  }

  return in;
}

std::istream &pilugina::operator>>(std::istream &in, UnsignedLongLongBIN &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char prefix[2];
  in >> prefix[0] >> prefix[1];

  if (prefix[0] != '0' || (tolower(prefix[1]) != 'b'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  unsigned long long num = 0;
  bool gotDigit = false;
  char temp;

  while (isBinaryDigit(in.peek()))
  {
    in >> temp;
    num = num * 2 + (temp - '0');
    gotDigit = true;
  }

  if (!gotDigit)
  {
    in.setstate(std::ios::failbit);
  }
  else
  {
    dest.ref = num;
  }

  return in;
}

std::istream &pilugina::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  return std::getline(in >> DelimiterIO {'"'}, dest.ref, '"');
}
