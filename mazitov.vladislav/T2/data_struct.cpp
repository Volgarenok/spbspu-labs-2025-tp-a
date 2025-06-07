#include "data_struct.hpp"
#include <algorithm>
#include <iomanip>
#include "stream_guard.hpp"
#include "input_struct.hpp"


std::string toBin(unsigned long long n)
{
  if (n == 0)
  {
    return "0";
  }

  std::string binary;
  while (n > 0)
  {
    binary += (n & 1) ? '1' : '0';
    n >>= 1;
  }
  std::reverse(binary.begin(), binary.end());
  if (binary.size() == 1)
  {
    binary = "0" + binary;
  }
  return binary;
}

bool mazitov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}

std::ostream& mazitov::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard guard(out);
  out << "(:key1 " << std::fixed << std::setprecision(1) << dest.key1 << "d";
  out << ":key2 0b" << toBin(dest.key2);
  out << ":key3 \"" << dest.key3 << "\":)";
  return out;
}

std::istream& mazitov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct data;
  {
    using sep = DelimiterIO;
    using ull = UnsignedLongLongBinIO;
    using dbl = DoubleIO;
    using str = StringIO;

    in >> sep{ '(' } >> sep{ ':' };
    for (int i = 0; i < 3; i++)
    {
      std::string key;
      in >> key;

      if (key == "key1")
      {
        in >> dbl{ data.key1 } >> sep{ ':' };
      }
      else if (key == "key2")
      {
        in >> ull{ data.key2 };
      }
      else if (key == "key3")
      {
        in >> str{ data.key3 } >> sep{ ':' };
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    in >> sep{ ')' };
  }
  if (in)
  {
    dest = data;
  }
  return in;
}
