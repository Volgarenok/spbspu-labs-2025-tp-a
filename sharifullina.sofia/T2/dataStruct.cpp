#include "dataStruct.hpp"
#include "scopeGuard.hpp"

#include <complex>
#include <vector>
#include <algorithm>

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
    if (!sentry)
    {
      return in;
    }
    char c = 0;
    in >> c;
    if (in && (c != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, ComplexIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    char hash = 0;
    char c = 0;
    char paren = 0;
    in >> hash >> c >> paren;
    if (hash != '#' || c != 'c' || paren != '(')
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    double real = 0.0;
    double imag = 0.0;
    in >> real >> imag >> DelimiterIO{ ')' };
    if (in)
    {
      dest.ref = std::complex<double>(real, imag);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, RationalIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> DelimiterIO{ 'N' };
    in >> dest.ref.first;
    in >> DelimiterIO{ ':' } >> DelimiterIO{ 'D' } >> dest.ref.second;
    in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }
}

bool sharifullina::DataStruct::operator<(const DataStruct& other) const
{
  if (std::abs(key1.real()) != std::abs(other.key1.real()) || 
      std::abs(key1.imag()) != std::abs(other.key1.imag()))
  {
    return std::abs(key1.real()) < std::abs(other.key1.real()) ||
           (std::abs(key1.real()) == std::abs(other.key1.real()) && 
            std::abs(key1.imag()) < std::abs(other.key1.imag()));
  }
  
  const double lhsRatio = static_cast<double>(key2.first) / key2.second;
  const double rhsRatio = static_cast<double>(other.key2.first) / other.key2.second;
  if (std::abs(lhsRatio - rhsRatio) > 1e-6)
  {
    return lhsRatio < rhsRatio;
  }
  
  return key3.length() < other.key3.length();
}

std::istream& sharifullina::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct temp;
  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
  for (int i = 0; i < 3; ++i)
  {
    std::string key;
    in >> key;

    if (key == "key1")
    {
      if (hasKey1)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> ComplexIO{ temp.key1 };
      hasKey1 = in.good();
    }
    else if (key == "key2")
    {
      if (hasKey2)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> RationalIO{ temp.key2 };
      hasKey2 = in.good();
    }
    else if (key == "key3")
    {
      if (hasKey3)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> StringIO{ temp.key3 };
      hasKey3 = in.good();
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    in >> DelimiterIO{ ':' };
  }

  in >> DelimiterIO{ ')' };

  if (in && hasKey1 && hasKey2 && hasKey3)
  {
    dest = temp;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream& sharifullina::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  sharifullina::iofmtguard guard(out);
  out << "(:key1 #c(" << dest.key1.real() << " " << dest.key1.imag() << ")";
  out << ":key2 (:N " << dest.key2.first << ":D " << dest.key2.second << ":)";
  out << ":key3 \"" << dest.key3 << "\":)";
  return out;
}
