#include "DataStruct.hpp"
#include "FormatGuard.hpp"
#include "Wrapper.hpp"

#include <cctype>
#include <iostream>

bool milchevskiy::operator<(const DataStruct& a, const DataStruct& b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
  }
  return a.key3.size() < b.key3.size();
}

std::ostream& milchevskiy::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard fg(out);

  out << "(:key1 " << src.key1;
  out << "ull:key2 0" << std::oct << src.key2;
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}

std::istream& milchevskiy::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{ '(' };

  std::string key;
  for (int i = 0; i < 3; i++)
  {
    in >> DelimiterIO{ ':' } >> key;
    if (key == "key1")
    {
      in >> LITvalue{ dest.key1 };
    }
    else if (key == "key2")
    {
      in >> OCTvalue{ dest.key2 };
    }
    else if (key == "key3")
    {
      in >> STRvalue{ dest.key3 };
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
  return in;
}

