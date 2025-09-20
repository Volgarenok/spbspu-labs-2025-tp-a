#include "data_struct.hpp"
#include <stream_guard.hpp>
#include <delimiter.hpp>
#include "input.hpp"

bool shak::operator<(const DataStruct &left, const DataStruct &right)
{
  if (left.key1 != right.key1)
  {
    return left.key1 < right.key1;
  }
  else if (left.key2 != right.key2)
  {
    return left.key2 < right.key2;
  }
  else
  {
    return left.key3.size() < right.key3.size();
  }
}

std::istream &shak::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct data;
  bool key1Read = false;
  bool key2Read = false;
  bool key3Read = false;
  in >> DelimiterIO{'('};
  for (size_t i = 0; i < 3; ++i)
  {
    std::string key;
    in >> DelimiterIO{':'} >> key;
    if (key == "key1")
    {
      if (key1Read)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> ChrLitIO{data.key1};
      key1Read = true;
    }
    else if (key == "key2")
    {
      if (key2Read)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> RatLspIO{data.key2};
      key2Read = true;
    }
    else if (key == "key3")
    {
      if (key3Read)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> StrIO{data.key3};
      key3Read = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }
  in >> DelimiterIO{':'} >> DelimiterIO{')'};

  if (in)
  {
    dest = data;
  }
  return in;
}

std::ostream &shak::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  shak::StreamGuard guard(out);
  out << "(";
  out << ":key1 \'" << src.key1 << '\'';
  out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
  out << ":key3 \"" << src.key3 << '"';
  out << ":)";
  return out;
}
