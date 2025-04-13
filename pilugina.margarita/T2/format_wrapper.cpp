#include "format_wrapper.hpp"

#include <iostream>

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
  char temp = 0;
  bool gotDigit = false;

  while (in.get(temp) && temp >= '0' && temp <= '7')
  {
    gotDigit = true;
    num = num * 8 + (temp - '0');
  }
  in.unget();

  if (!gotDigit)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  dest.ref = num;
  return in;
}

std::istream &pilugina::operator>>(std::istream &in, UnsignedLongLongBIN &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char prefix[2] = {0};
  in.get(prefix[0]).get(prefix[1]);

  if (prefix[0] != '0' || (tolower(prefix[1]) != 'b'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  unsigned long long num = 0;
  char temp = 0;
  bool gotDigit = false;

  while (in.get(temp) && (temp == '0' || temp == '1'))
  {
    gotDigit = true;
    num = num * 2 + (temp - '0');
  }
  in.unget();

  if (!gotDigit)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  dest.ref = num;
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
