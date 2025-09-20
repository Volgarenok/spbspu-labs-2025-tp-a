#include "dataStruct.hpp"
#include <vector>
#include <iomanip>
#include <scopeGuard.hpp>
#include <delimiter.hpp>

namespace mozhegova
{
  struct UlloctIO
  {
    unsigned long long & ref;
  };

  struct ComplexIO
  {
    std::complex< double > & ref;
  };

  struct StringIO
  {
    std::string & ref;
  };

  std::istream & operator>>(std::istream & in, UlloctIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{ '0' } >> dest.ref;
    if (in)
    {
      unsigned long long a = dest.ref;
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

  std::istream & operator>>(std::istream & in, ComplexIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    double real, imag = 0;
    in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' };
    in >> DelimiterIO{ '(' };
    in >> real >> imag;
    in >> DelimiterIO{ ')' };
    if (in)
    {
      dest.ref = std::complex< double >(real, imag);
    }
    return in;
  }

  std::istream & operator>>(std::istream & in, StringIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }
}

std::istream & mozhegova::operator>>(std::istream & in, mozhegova::DataStruct & dest)
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
        in >> UlloctIO{ input.key1 };
      }
      else if (keys[i] == "key2")
      {
        in >> ComplexIO{ input.key2 };
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

std::ostream & mozhegova::operator<<(std::ostream & out, const mozhegova::DataStruct & dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  iofmtguard fmtguard(out);
  out << '(';
  out << ":key1 " << '0' << dest.key1;
  out << ":key2 " << "#c(";
  out << std::fixed << std::setprecision(1);
  out << dest.key2.real() << ' ' << dest.key2.imag() << ')';
  out << ":key3 " << '"' << dest.key3 << '"';
  out << ":)";
  return out;
}

bool mozhegova::DataStruct::operator<(const mozhegova::DataStruct & other) const
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }
  else if (key2 != other.key2)
  {
    return std::abs(key2) < std::abs(other.key2);
  }
  else
  {
    return key3.size() < other.key3.size();
  }
}
