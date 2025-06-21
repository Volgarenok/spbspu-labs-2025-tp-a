#include "data_struct.hpp"
#include "stream_guard.hpp"
#include <cctype>
#include <iostream>

namespace
{
  static bool checkChar(std::istream & in, char expected)
  {
    char c = 0;
    in >> c;
    if (!in)
    {
      return false;
    }
    return std::tolower(c) == std::tolower(expected);
  }
}

std::istream & smirnov::operator>>(std::istream& in, DataStruct & data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  DataStruct temp;
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;
  if (!checkChar(in, '('))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  for (int i = 0; i < 3; ++i)
  {
    if (!checkChar(in, ':') || !checkChar(in, 'k') || !checkChar(in, 'e') || !checkChar(in, 'y'))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    int keyIndex = 0;
    in >> keyIndex;
    if (!in)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (keyIndex == 1)
    {
      unsigned long long val = 0;
      in >> val;
      if (!in || !checkChar(in, 'u') || !checkChar(in, 'l') || !checkChar(in, 'l') || hasKey1)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      temp.key1 = val;
      hasKey1 = true;
    }
    else if (keyIndex == 2)
    {
      long long val = 0;
      in >> val;
      if (!in || !checkChar(in, 'l') || !checkChar(in, 'l') || hasKey2)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      temp.key2 = val;
      hasKey2 = true;
    }
    else if (keyIndex == 3)
    {
      if (!checkChar(in, '"') || hasKey3)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      std::string s;
      std::getline(in, s, '"');
      if (!in)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      temp.key3 = std::move(s);
      hasKey3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  if (!checkChar(in, ':') || !checkChar(in, ')'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (hasKey1 && hasKey2 && hasKey3)
  {
    data = std::move(temp);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

bool smirnov::compare(const DataStruct & lhs, const DataStruct & rhs)
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

std::ostream & smirnov::operator<<(std::ostream & out, const DataStruct & data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard guard(out);
  out << "(:key1 " << data.key1 << "ull"
      << ":key2 " << data.key2 << "ll"
      << ":key3 \"" << data.key3 << "\":)";
  return out;
}
