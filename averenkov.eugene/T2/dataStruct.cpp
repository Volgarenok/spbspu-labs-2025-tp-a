#include "dataStruct.hpp"
#include <sstream>
#include <algorithm>
#include "iofmtguard.hpp"

using namespace DataIO;

std::istream& operator>>(std::istream& in, DataStruct& dest)
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

std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
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

bool operator<(const DataStruct& a, const DataStruct& b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  double a_val = static_cast< double >(a.key2.first) / a.key2.second;
  double b_val = static_cast< double >(b.key2.first) / b.key2.second;
  if (std::abs(a_val - b_val) > 1e-9)
  {
    return a_val < b_val;
  }
  return a.key3.length() < b.key3.length();
}
