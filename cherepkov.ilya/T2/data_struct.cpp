#include "data_struct.hpp"
#include "stream_guard.hpp"
#include "input_operators.hpp"


std::istream& cherepkov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
      return in;
  }

  DataStruct temp;
  size_t keysRead = 0;
  bool has_key1 = false, has_key2 = false, has_key3 = false;

  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

  while (keysRead < 3)
  {
    std::string key;
    in >> key;

    if (key == "key1")
    {
      if (has_key1)
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        in >> UllLitValue{ temp.key1 };
        has_key1 = true;
    }
    else if (key == "key2")
    {
      if (has_key2)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
        in >> UllBinValue{ temp.key2 };
        has_key2 = true;
    }
    else if (key == "key3")
    {
      if (has_key3)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
        in >> StringValue{ temp.key3 };
        has_key3 = true;
    }
    else
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (!in)
    {
      return in;
    }
    ++keysRead;
  }

  in >> DelimiterIO{ ')' };

  if (in)
  {
      dest = std::move(temp);
  }

  return in;
}

std::string convertToBinary(unsigned long long val)
{
  if (val == 0)
  {
    return "0";
  }
  std::string bin;
  bin.reserve(64);

  while (val > 0)
  {
    bin.push_back('0' + (val % 2));
    val /= 2;
  }
  std::reverse(bin.begin(), bin.end());
  return bin;
}

std::ostream& cherepkov::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  cherepkov::StreamGuard guard(out);

  out << "(:";
  out << "key1 " << src.key1 << "ull" << ":";
  out << "key2 0b" << (src.key2 == 0 ? "" : "0") << convertToBinary(src.key2);
  out << ":key3 \"" << src.key3;
  out << "\":)";

  return out;
}

bool cherepkov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 == rhs.key1)
  {
    if (lhs.key2 == rhs.key2)
    {
      return lhs.key3.length() < rhs.key3.length();
    }
    return lhs.key2 < rhs.key2;
  }
  return lhs.key1 < rhs.key1;
}
