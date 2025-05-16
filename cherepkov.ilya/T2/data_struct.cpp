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
  using sep = DelimiterIO;
  using lit = UllLitValue;
  using bin = UllBinValue;
  using str = StringValue;

  in >> sep{ '(' } >> sep{ ':' };

  for (size_t i = 0; i < 3; i++)
  {
    std::string key;
    in >> key;

    if (key == "key1")
    {
      in >> lit{ temp.key1 };
    }
    else if (key == "key2")
    {
      in >> bin{ temp.key2 };
    }
    else if (key == "key3")
    {
      in >> str{ temp.key3 };
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  in >> sep{ ')' };

  if (in)
  {
    dest = temp;
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

  out << "(:";
  out << "key1 " << src.key1 << "ull" << ":";
  out << "key2 0b" << (src.key2 == 0 ? "" : "0") << ULLtoBinary(src.key2);
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
