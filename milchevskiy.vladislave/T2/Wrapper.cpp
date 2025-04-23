#include "Wrapper.hpp"

#include <iostream>

std::istream& milchevskiy::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = 0;
  in.get(c);
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& milchevskiy::operator>>(std::istream& in, LITvalue&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  unsigned long long num = 0;
  char a = 0, b = 0, c = 0;
  char temp = 0;
  bool is_digit = false;

  while (in >> temp && std::isdigit(temp))
  {
    is_digit = true;
    num = num * 10 + (temp - '0');
  }
  in.unget();

  if (!is_digit)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (in >> a >> b >> c && tolower(a) == 'u' && tolower(b) == 'l' && tolower(c) == 'l')
  {
    dest.ref = num;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& milchevskiy::operator>>(std::istream& in, OCTvalue&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char prefix = 0;
  in.get(prefix);
  if (!in || prefix != '0')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  unsigned long long num = 0;
  char temp = 0;
  bool is_digit = false;

  while (in.get(temp) && temp >= '0' && temp <= '7')
  {
    is_digit = true;
    num = num * 8 + (temp - '0');
  }
  in.unget();

  if (!is_digit)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  dest.ref = num;
  return in;
}

std::istream& milchevskiy::operator>>(std::istream& in, STRvalue&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}
