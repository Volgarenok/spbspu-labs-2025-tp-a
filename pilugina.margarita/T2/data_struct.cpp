#include "data_struct.hpp"
#include "format_guard.hpp"
#include "format_wrapper.hpp"
#include <iostream>
#include <cctype>

bool pilugina::operator<(const DataStruct &lhs, const DataStruct &rhs)
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

std::string convertToBinString(unsigned long long num)
{
  if (num == 0)
  {
    return "0";
  }
  std::string bin;
  while (num > 0)
  {
    bin = (num % 2 ? "1" : "0") + bin;
    num /= 2;
  }
  return bin;
}

std::ostream &pilugina::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard fg(out);

  out << "(:key1 0" << std::oct << src.key1;
  out << ":key2 0b" << convertToBinString(src.key2);
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}

std::istream &pilugina::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct tmp {};
  in >> DelimiterIO {'('};

  std::string field;
  for (int i = 0; i < 3; i++)
  {
    in >> DelimiterIO {':'} >> field;
    if (field == "key1")
    {
      in >> UnsignedLongLongOCT {tmp.key1};
    }
    else if (field == "key2")
    {
      in >> UnsignedLongLongBIN {tmp.key2};
    }
    else if (field == "key3")
    {
      in >> StringIO {tmp.key3};
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  in >> DelimiterIO {':'} >> DelimiterIO {')'};
  if (in)
  {
    dest = tmp;
  }
  return in;
}
