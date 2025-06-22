#include "io.hpp"
#include <cctype>
#include <iostream>

std::istream& amine::operator>>(std::istream& in, DelimiterIO&& dest)
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

std::istream& amine::operator>>(std::istream& in, ComplexIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };
  double real = 0.0, imag = 0.0;
  in >> real >> imag >> DelimiterIO{ ')' };

  if (in)
  {
    dest.ref = std::complex< double >(real, imag);
  }
  return in;
}

std::istream& amine::operator>>(std::istream& in, RationalIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> DelimiterIO{ 'N' };
  long long numerator = 0;
  in >> numerator >> DelimiterIO{ ':' } >> DelimiterIO{ 'D' };
  unsigned long long denominator = 0;
  in >> denominator >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };

  if (in && denominator != 0)
  {
    dest.ref = { numerator, denominator };
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& amine::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}
