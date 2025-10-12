#include "dataStruct.hpp"
#include <vector>
#include <cstdlib>
#include <complex>
#include <string>
#include <iostream>
#include <iomanip>
#include <scopeGuard.hpp>
#include <delimiter.hpp>

namespace sharifullina
{
  struct LongLongIO
  {
    long long & ref;
  };

  struct RationalIO
  {
    std::pair< long long, unsigned long long > & ref;
  };

  struct StringIO
  {
    std::string & ref;
  };

  std::istream & operator>>(std::istream & in, LongLongIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> dest.ref;
    char c1 = '0';
    char c2 = '0';
    in >> c1 >> c2;
    if (in && ((c1 != 'l' && c1 != 'L') || (c2 != 'l' && c2 != 'L')))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream & operator>>(std::istream & in, RationalIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> DelimiterIO{ 'N' } >> dest.ref.first;
    in >> DelimiterIO{ ':' } >> DelimiterIO{ 'D' } >> dest.ref.second;
    in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
    return in;
  }

  std::istream & operator>>(std::istream& in, StringIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }
}

std::istream & sharifullina::operator>>(std::istream & in, sharifullina::DataStruct & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    std::vector< std::string > keys(3);
    in >> DelimiterIO{ '(' };
    for (size_t i = 0; i < 3; i++)
    {
      in >> DelimiterIO{ ':' };
      in >> keys[i];
      if (keys[i] == "key1")
      {
        in >> LongLongIO{ input.key1 };
      }
      else if (keys[i] == "key2")
      {
        in >> RationalIO{ input.key2 };
      }
      else if (keys[i] == "key3")
      {
        in >> StringIO{ input.key3 };
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    in >> DelimiterIO{ ':' };
    in >> DelimiterIO{ ')' };
  }
  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream & sharifullina::operator<<(std::ostream & out, const sharifullina::DataStruct & dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  sharifullina::IofmtGuard guard(out);
  out << std::fixed << std::setprecision(1);

  out << "(:key1 " << dest.key1 << "ll";
  out << ":key2 (:N " << dest.key2.first << ":D " << dest.key2.second << ":)";
  out << ":key3 \"" << dest.key3 << "\":)";

  return out;
}

bool sharifullina::DataStruct::operator<(const DataStruct & other) const
{

  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }

  const double lhsRatio = static_cast< double >(key2.first) / key2.second;
  const double rhsRatio = static_cast< double >(other.key2.first) / other.key2.second;

  if (lhsRatio != rhsRatio)
  {
    return lhsRatio < rhsRatio;
  }
  return key3.length() < other.key3.length();
}
