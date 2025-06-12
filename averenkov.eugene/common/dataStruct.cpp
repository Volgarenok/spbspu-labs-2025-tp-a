#include "dataStruct.hpp"
#include <istream>
#include <algorithm>
#include "iofmtguard.hpp"

std::istream& averenkov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct tmp;
  in >> DelimiterIO{'('} >> DelimiterIO{':'};
  std::string field;
  while (in >> field)
  {
    if (field == "key1")
    {
      in >> CharIO{tmp.key1} >> DelimiterIO{':'};
    }
    else if (field == "key2")
    {
      in >> RationalIO{tmp.key2} >> DelimiterIO{':'};
    }
    else if (field == "key3")
    {
      in >> StringIO{tmp.key3} >> DelimiterIO{':'};
    }
    else if (field == ")")
    {
      break;
    }
    else
    {
      in.setstate(std::ios::failbit);
      break;
    }
  }
  if (in)
  {
    dest = tmp;
  }
  return in;
}

std::ostream& averenkov::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  iofmtguard guard(out);
  out << "(:key1 '" << dest.key1;
  out << "':key2 (:N " << dest.key2.first;
  out << ":D " << dest.key2.second;
  out << ":):key3 \"" << dest.key3 << "\":)";
  return out;
}

bool averenkov::operator<(const DataStruct& a, const DataStruct& b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
  }
  return a.key3.length() < b.key3.length();
}
