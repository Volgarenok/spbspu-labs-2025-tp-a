#include "datastruct.hpp"
#include "stream_guard.hpp"
#include <string>
#include <algorithm>

bool kazak::operator<(const DataStruct& lhs, const DataStruct& rhs)
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

std::istream& kazak::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kazak::operator>>(std::istream& in, HexPrefixIO&&)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c1 = in.get();
  char c2 = in.get();

  if (c1 != '0' || (c2 != 'x' && c2 != 'X'))
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}


std::istream& kazak::operator>>(std::istream& in, ULLHexIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> std::ws >> HexPrefixIO{};
  in >> std::hex >> dest.ref;

  if (!in)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& kazak::operator>>(std::istream& in, RationalIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  long long numerator = 0;
  unsigned long long denominator = 0;

  in >> DelimiterIO{'('} >> LabelIO{":N"} >> numerator >> LabelIO{":D"} >> denominator >> DelimiterIO{':'} >> DelimiterIO{')'};

  if (in)
  {
    dest.ref = std::make_pair(numerator, denominator);
  }

  return in;
}

std::istream& kazak::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{'"'};
  return std::getline(in, dest.ref, '"');
}

std::istream& kazak::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string label;
  in >> label;
  if (in && label != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& kazak::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct input;
  size_t keyCount = 0;

  in >> DelimiterIO{'('} >> DelimiterIO{':'};

  while (keyCount < 3 && in)
  {
    std::string key;
    in >> key;

    if (key == "key1")
    {
      in >> ULLHexIO{input.key1} >> DelimiterIO{':'};
      ++keyCount;
    }
    else if (key == "key2")
    {
      in >> RationalIO{input.key2} >> DelimiterIO{':'};
      ++keyCount;
    }
    else if (key == "key3")
    {
      in >> StringIO{input.key3} >> DelimiterIO{':'};
      ++keyCount;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }

  in >> DelimiterIO{')'};

  if (in)
  {
    dest = std::move(input);
  }

  return in;
}

std::ostream& kazak::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  StreamGuard guard(out);
  out << "(:key1 0x" << std::hex << std::uppercase << src.key1;
  out << ":key2 (:N " << std::dec << src.key2.first << ":D " << src.key2.second << ":)";
  out << ":key3 \"" << src.key3 << "\":)";

  return out;
}
