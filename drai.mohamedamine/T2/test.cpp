#include "test.hpp"
#include <iostream>
#include <sstream>
#include <cctype>

namespace amine {
std::istream &operator>>(std::istream &in, DelimiterIO &&dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char c;
    if (in >> c && c != dest.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}
<<<<<<< HEAD

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
=======
std::istream &operator>>(std::istream &in, DoubleSCI &&dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    double num;
    if (in >> num) {
        dest.ref = num;
    } else {
        in.setstate(std::ios::failbit);
    }
    return in;
}
std::istream &operator>>(std::istream &in, RationalIO &&dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    long long numerator = 0;
    unsigned long long denominator = 0;
    in >> DelimiterIO{'('} >> DelimiterIO{':'} >> DelimiterIO{'N'};
    in >> numerator;
    in >> DelimiterIO{':'} >> DelimiterIO{'D'};
    in >> denominator;
    in >> DelimiterIO{':'} >> DelimiterIO{')'};
    if (denominator == 0 || !in) {
        in.setstate(std::ios::failbit);
    } else {
        dest.ref = {numerator, denominator};
    }
    return in;
>>>>>>> c515838cf784c143137d2c17bd825b2c1b191313
}
std::istream &operator>>(std::istream &in, StringIO &&dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char quote;
    if (in >> quote && quote != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string str;
    char c;
    while (in.get(c)) {  // Fixed syntax here - removed '{' and added ')'
        if (c == '\\') {
            if (!in.get(c)) break;
            str += c;
        }
        else if (c == '"') {
            break;
        }
        else {
            str += c;
        }
    }
    if (c != '"') {
        in.setstate(std::ios::failbit);
    } else {
        dest.ref = str;
    }
    return in;
<<<<<<< HEAD
  }

  return std::getline(in >> DelimiterIO {'"'}, dest.ref, '"');
=======
}
>>>>>>> c515838cf784c143137d2c17bd825b2c1b191313
}
