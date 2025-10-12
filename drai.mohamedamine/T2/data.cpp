#include "data.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include "io.hpp"
#include "stream_guard.hpp"

bool amine::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  double lhs_mod = std::abs(lhs.key1);
  double rhs_mod = std::abs(rhs.key1);
  if (lhs_mod != rhs_mod)
  {
    return lhs_mod < rhs_mod;
  }

  double lhs_ratio = static_cast< double >(lhs.key2.first) / lhs.key2.second;
  double rhs_ratio = static_cast< double >(rhs.key2.first) / rhs.key2.second;
  if (lhs_ratio != rhs_ratio)
  {
    return lhs_ratio < rhs_ratio;
  }

  return lhs.key3.size() < rhs.key3.size();
}

std::ostream& amine::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  StreamGuard fg(out);
  out << "(:key1 #c(";
  out << std::fixed << std::setprecision(1) << src.key1.real() << " " << src.key1.imag() << ")";
  out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}

std::istream& amine::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct tmp;
  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
  std::string field;

  for (int i = 0; i < 3; i++)
  {
    in >> field;
    if (field == "key1")
    {
      in >> ComplexIO{ tmp.key1 };
    }
    else if (field == "key2")
    {
      in >> RationalIO{ tmp.key2 };
    }
    else if (field == "key3")
    {
      in >> StringIO{ tmp.key3 };
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> DelimiterIO{ ':' };
  }

  in >> DelimiterIO{ ')' };
  if (in)
  {
    dest = tmp;
  }
  return in;
}
