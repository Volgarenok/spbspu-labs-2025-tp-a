#include "dataStruct.hpp"
#include "scopeGuard.hpp"
#include <complex>
#include <string>
#include <iostream>
#include <iomanip>
#include <utility>
#include <limits>
#include <cmath>

namespace
{
  struct DelimiterIO
  {
    char exp;
  };

  struct ComplexIO
  {
    std::complex<double>& ref;
  };

  struct RationalIO
  {
    std::pair<long long, unsigned long long>& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char c = 0;
    in >> c;
    if (in && c != dest.exp)
      in.setstate(std::ios::failbit);

    return in;
  }

  std::istream& operator>>(std::istream& in, ComplexIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char hash = 0, c = 0, lparen = 0, rparen = 0;
    double real = 0.0, imag = 0.0;

    in >> hash >> c >> lparen >> real >> imag >> rparen;
    if (in && hash == '#' && c == 'c' && lparen == '(' && rparen == ')')
    {
      dest.ref = std::complex<double>(real, imag);
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, RationalIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char lparen = 0, colon1 = 0, nchar = 0, colon2 = 0, dchar = 0, colon3 = 0, rparen = 0;
    long long num = 0;
    unsigned long long den = 0;

    in >> lparen >> colon1 >> nchar >> num >> colon2 >> dchar >> den >> colon3 >> rparen;
    if (in &&
        lparen == '(' && colon1 == ':' && nchar == 'N' &&
        colon2 == ':' && dchar == 'D' && colon3 == ':' &&
        rparen == ')')
    {
      dest.ref = std::make_pair(num, den);
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    std::string tmp;
    if (!(in >> std::quoted(tmp)))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    dest.ref = tmp;
    return in;
  }
}

bool sharifullina::DataStruct::operator<(const DataStruct& other) const
{
  const double epsilon = 1e-10;
  const double abs1 = std::abs(key1);
  const double abs2 = std::abs(other.key1);

  if (std::abs(abs1 - abs2) > epsilon)
  {
    return abs1 < abs2;
  }

  const double lhsRatio =
      static_cast<double>(key2.first) / key2.second;
  const double rhsRatio =
      static_cast<double>(other.key2.first) / other.key2.second;

  if (lhsRatio != rhsRatio)
  {
    return lhsRatio < rhsRatio;
  }

  return key3.length() < other.key3.length();
}

std::istream& sharifullina::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) return in;

  DataStruct result;
  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

  while (in && in.peek() != ')')
  {
    std::string key;
    if (!(in >> key))
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (key == "key1")
    {
      if (!(in >> ComplexIO{ result.key1 }))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      hasKey1 = true;
    }
    else if (key == "key2")
    {
      if (!(in >> RationalIO{ result.key2 }))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      hasKey2 = true;
    }
    else if (key == "key3")
    {
      if (!(in >> StringIO{ result.key3 }))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      hasKey3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (!(in >> DelimiterIO{ ':' }))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  if (!(in >> DelimiterIO{ ')' }))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!(hasKey1 && hasKey2 && hasKey3))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  dest = std::move(result);
  return in;
}

std::ostream& sharifullina::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  sharifullina::IofmtGuard guard(out);
  out << std::fixed << std::setprecision(1);

  out << "(:key1 #c(" << dest.key1.real() << " " << dest.key1.imag() << ")";
  out << ":key2 (:N " << dest.key2.first << ":D " << dest.key2.second << ":)";
  out << ":key3 \"" << dest.key3 << "\":)";

  return out;
}
