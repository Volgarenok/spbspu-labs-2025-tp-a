#include "datastruct.hpp"
#include "delimiter.hpp"
#include "scopeguard.hpp"
#include "value.hpp"
#include <bitset>

namespace
{
  std::string getBinNumber(unsigned long long value)
  {
    std::bitset< 64 > bin(value);
    std::string binString = bin.to_string();
    return "0" + binString.erase(0, binString.find('1'));
  }
}

bool smirnov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 == rhs.key1)
  {
    if (lhs.key2 == rhs.key2)
    {
      return lhs.key3.length() < rhs.key3.length();
    }
    else
    {
      return lhs.key2 < rhs.key2;
    }
  }
  else
  {
    return lhs.key1 < rhs.key1;
  }
}

std::istream& smirnov::operator>>(std::istream& in, DataStruct& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  ScopeGuard scopeGuard(in);

  const size_t KEY_NUMBER = 3;
  size_t keyI = 0;

  in >> DelimiterCharI{ '(' };

  for (size_t i = 0; i < KEY_NUMBER; ++i)
  {
    in >> DelimiterStringI{ ":key" } >> keyI;
    switch (keyI)
    {
      case 1:
      {
        in >> DBLLit{value.key1};
        break;
      }
      case 2:
      {
        in >> ULLBinary{value.key2};
        break;
      }
      case 3:
      {
        in >> StrKey{value.key3};
        break;
      }
      default:
      {
        in.setstate(std::ios::failbit);
      }
    }
  }

  in >> DelimiterStringI{ ":)" };
  return in;
}

std::ostream& smirnov::operator<<(std::ostream& out, const DataStruct& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  ScopeGuard scopeGuard(out);

  out << std::setprecision(1) << std::fixed << "(";
  out << ":key1 " << value.key1 << "d";
  out << ":key2 0b" << getBinNumber(value.key2);
  out << ":key3 \"" << value.key3 << "\":)";

  return out;
}

