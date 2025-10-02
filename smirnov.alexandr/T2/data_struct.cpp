#include "data_struct.hpp"
#include <cctype>
#include <iostream>
#include <guard.hpp>

std::istream & smirnov::operator>>(std::istream & in, ULLIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref;
  char u = 0;
  char l1 = 0;
  char l2 = 0;
  in >> u >> l1 >> l2;
  if (!in || std::tolower(u) != 'u' || std::tolower(l1) != 'l' || std::tolower(l2) != 'l')
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & smirnov::operator>>(std::istream & in, LLIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref;
  char l1 = 0;
  char l2 = 0;
  in >> l1 >> l2;
  if (!in || std::tolower(l1) != 'l' || std::tolower(l2) != 'l')
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & smirnov::operator>>(std::istream & in, QuotedStringIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterI{'"'};
  std::getline(in, dest.ref, '"');
  if (!in)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & smirnov::operator>>(std::istream & in, DataStruct & data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  DataStruct temp{};
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;
  in >> DelimiterI{'('};
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  for (int i = 0; i < 3; ++i)
  {
    in >> DelimiterI{':'} >> DelimiterI{'k'} >> DelimiterI{'e'} >> DelimiterI{'y'};
    if (!in)
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
      if (hasKey1)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      hasKey1 = true;
      in >> ULLIO{temp.key1};
      if (!in)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    else if (keyIndex == 2)
    {
      if (hasKey2)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      hasKey2 = true;
      in >> LLIO{temp.key2};
      if (!in)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    else if (keyIndex == 3)
    {
      if (hasKey3)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      hasKey3 = true;
      in >> QuotedStringIO{temp.key3};
      if (!in)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  in >> DelimiterI{':'} >> DelimiterI{')'};
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  data = std::move(temp);
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
  out << "(:key1 " << data.key1 << "ull";
  out << ":key2 " << data.key2 << "ll";
  out << ":key3 \"" << data.key3 << "\":)";
  return out;
}
