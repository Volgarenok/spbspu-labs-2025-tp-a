#include "format_wrapper.hpp"
#include <iostream>
#include <delimiterIO.hpp>
#include "utils.hpp"
#include "format_guard.hpp"

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

  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::ostream &pilugina::output::operator<<(std::ostream &out, const UnsignedLongLongOCT &dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard fg(out);
  return out << std::oct << dest.ref;
}

std::ostream &pilugina::output::operator<<(std::ostream &out, const UnsignedLongLongBIN &dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard fg(out);

  unsigned long long copy = dest.ref;
  if (copy == 1)
  {
    out << "01";
    return out;
  }
  else if (copy == 0)
  {
    out << "0";
    return out;
  }

  while (copy > 0)
  {
    out << copy % 2;
    copy /= 2;
  }
  return out;
}
