#include "ioData.hpp"
#include <vector>
#include <complex>
#include <iomanip>
#include <algorithm>
#include <streamGuard.hpp>
#include <delimiter.hpp>

using lebedev::DelimiterIO;
using lebedev::StreamGuard;

namespace
{
  struct UnsignedLongLongLiteralIO
  {
    unsigned long long& obj;
  };

  struct UnsignedLongLongOctIO
  {
    unsigned long long& obj;
  };

  struct StringIO
  {
    std::string& obj;
  };

  std::istream& operator>>(std::istream& in, UnsignedLongLongLiteralIO&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> rhs.obj >> DelimiterIO{'u'} >> DelimiterIO{'l'} >> DelimiterIO{'l'};
    return in;
  }

  std::istream & operator>>(std::istream & in, UnsignedLongLongOctIO && rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{ '0' } >> rhs.obj;
    if (in)
    {
      unsigned long long a = rhs.obj;
      while (a > 0)
      {
        if (a % 10 >= 8)
        {
          in.setstate(std::ios::failbit);
        }
        a /= 10;
      }
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, rhs.obj, '"');
  }
}

std::istream& lebedev::operator>>(std::istream& in, DataStruct& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using sep = DelimiterIO;
    using ullLit = UnsignedLongLongLiteralIO;
    using ullOct = UnsignedLongLongOctIO;
    using str = StringIO;
    in >> sep{ '(' } >> sep{ ':' };
    std::vector< std::string > usedKeys;
    std::string key = "";
    while (in >> key && key != ")")
    {
      if (std::find(usedKeys.begin(), usedKeys.end(), key) != usedKeys.end())
      {
        in.setstate(std::ios::failbit);
        break;
      }
      usedKeys.push_back(key);
      if (key == "key1")
      {
        if (!(in >> ullLit{ input.key1 } >> sep{':'}))
        {
          in.setstate(std::ios::failbit);
          break;
        }
      }
      else if (key == "key2")
      {
        if (!(in >> ullOct{ input.key2 } >> sep{':'}))
        {
          in.setstate(std::ios::failbit);
          break;
        }
      }
      else if (key == "key3")
      {
        if (!(in >> str{ input.key3 } >> sep{':'}))
        {
          in.setstate(std::ios::failbit);
          break;
        }
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }
    }
  }
  if (in)
  {
    rhs = input;
  }
  return in;
}

std::ostream& lebedev::operator<<(std::ostream& out, const DataStruct& rhs)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  StreamGuard outGuard(out);
  out << "(:key1 ";
  out << rhs.key1;
  out << "ull:";
  out << "key2 " << '0';
  out << rhs.key2;
  out << ":key3 \"";
  out << rhs.key3;
  out << "\":)";
  return out;
}
