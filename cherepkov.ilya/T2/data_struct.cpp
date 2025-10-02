#include "data_struct.hpp"
#include <stream_guard.hpp>
#include "input_operators.hpp"
#include "output_operators.hpp"


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

std::ostream& cherepkov::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  cherepkov::StreamGuard guard(out);

  out << "(:key1 " << UllLitOutput{src.key1} << ":";
  out << "key2 0b" << UllBinOutput{src.key2} << ":";
  out << "key3 \"" << src.key3 << "\":)";

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
