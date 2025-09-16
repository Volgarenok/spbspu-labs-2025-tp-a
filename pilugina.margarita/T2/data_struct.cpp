#include "data_struct.hpp"
#include <iostream>
#include <string>
#include <delimiterIO.hpp>
#include "format_guard.hpp"
#include "format_wrapper.hpp"

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

std::ostream &pilugina::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard fg(out);

  out << "(:key1 0" << output::UnsignedLongLongOCT{src.key1};
  out << ":key2 0b" << output::UnsignedLongLongBIN{src.key2};
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

  DataStruct tmp{};
  bool gotKey1 = false;
  bool gotKey2 = false;
  bool gotKey3 = false;

  in >> DelimiterIO{'('};

  std::string field;
  for (int i = 0; i < 3; i++)
  {
    in >> DelimiterIO{':'} >> field;

    if (field == "key1")
    {
      if (gotKey1)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> UnsignedLongLongOCT{tmp.key1};
      gotKey1 = true;
    }
    else if (field == "key2")
    {
      if (gotKey2)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> UnsignedLongLongBIN{tmp.key2};
      gotKey2 = true;
    }
    else if (field == "key3")
    {
      if (gotKey3)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> StringIO{tmp.key3};
      gotKey3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  in >> DelimiterIO{':'} >> DelimiterIO{')'};
  if (in && gotKey1 && gotKey2 && gotKey3)
  {
    dest = tmp;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}
