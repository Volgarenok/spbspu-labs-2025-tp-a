#include "test.hpp"
#include <iostream>
#include <sstream>
#include <cctype>

std::istream &amine::operator>>(std::istream &in, DelimiterIO &&dest)
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

std::istream &amine::operator>>(std::istream &in, DoubleSCI &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  double num = 0.0;
  if (!(in >> num))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  char e = 0;
  if (!(in >> e) || (tolower(e) != 'e'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  char sign = 0;
  if (!(in >> sign) || (sign != '+' && sign != '-'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  int exponent = 0;
  if (!(in >> exponent))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (sign == '-')
  {
    exponent = -exponent;
  }

  dest.ref = num * std::pow(10, exponent);
  return in;
}

std::istream &amine::operator>>(std::istream &in, RationalLSP &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO {'('};
  std::string field;

  long long numerator = 0;
  unsigned long long denominator = 1;

  for (int i = 0; i < 2; i++)
  {
    in >> DelimiterIO {':'} >> field;
    if (field == "N")
    {
      in >> numerator;
    }
    else if (field == "D")
    {
      in >> denominator;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  in >> DelimiterIO {':'} >> DelimiterIO {')'};
  if (in)
  {
    dest.ref = std::make_pair(numerator, denominator);
  }
  return in;
}

std::istream &amine::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  return std::getline(in >> DelimiterIO {'"'}, dest.ref, '"');
}
