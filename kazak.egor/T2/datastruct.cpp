#include "datastruct.hpp"
#include "stream_guard.hpp"

namespace kazak
{
  bool operator<(const DataStruct& lhs, const DataStruct& rhs)
  {
    if (lhs.key1 != rhs.key1)
    {
      return lhs.key1 < rhs.key1;
    }

    if (lhs.key2.first != rhs.key2.first)
    {
      return lhs.key2.first < rhs.key2.first;
    }

    if (lhs.key2.second != rhs.key2.second)
    {
      return lhs.key2.second < rhs.key2.second;
    }

    return lhs.key3 < rhs.key3;
  }

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);

    if (!sentry)
    {
      return in;
    }

    char c = '0';
    in >> c;

    if (in && (c != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::istream& operator>>(std::istream& in, ULLHexIO&& dest)
  {
    std::istream::sentry sentry(in);

    if (!sentry)
    {
      return in;
    }

    std::string hexStr;
    in >> std::ws;
    char c1, c2;
    c1 = in.get();
    c2 = in.peek();

    if (c1 == '0' && (c2 == 'x' || c2 == 'X'))
    {
      in.get();
      in >> std::hex >> dest.ref;
      if (!in)
      {
        in.setstate(std::ios::failbit);
      }
    }
    else
    {
      in.unget();
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::istream& operator>>(std::istream& in, RationalIO&& dest)
  {
    std::istream::sentry sentry(in);

    if (!sentry)
    {
      return in;
    }

    using sep = DelimiterIO;
    using lbl = LabelIO;
    long long numerator = 0;
    unsigned long long denominator = 0;

    in >> sep{ '(' } >> lbl{ ":N" } >> numerator;
    in >> lbl{ ":D" } >> denominator;
    in >> sep{ ':' } >> sep{ ')' };

    if (in)
    {
      dest.ref = std::make_pair(numerator, denominator);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);

    if (!sentry)
    {
      return in;
    }

    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, LabelIO&& dest)
  {
    std::istream::sentry sentry(in);

    if (!sentry)
    {
      return in;
    }

    std::string data;

    if ((in >> data) && (data != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);

    if (!sentry)
    {
      return in;
    }

    DataStruct input;
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;

    if (!(in >> DelimiterIO{'('}))
    {
      return in;
    }

    while (true)
    {
      in >> std::ws;

      if (in.peek() == ':')
      {
        in.get();

        if (in.peek() == ')')
        {
          in.get();
          break;
        }
        else
        {
          in.unget();
        }
      }

      if (!(in >> DelimiterIO{ ':' }))
      {
        in.setstate(std::ios::failbit);
        return in;
      }

      std::string key;

      if (!(in >> key))
      {
        in.setstate(std::ios::failbit);
        return in;
      }

      if (key == "key1")
      {
        if (!(in >> ULLHexIO{ input.key1 }))
        {
          return in;
        }

        hasKey1 = true;
      }
      else if (key == "key2")
      {
        if (!(in >> RationalIO{ input.key2 }))
        {
          return in;
        }

        hasKey2 = true;
      }
      else if (key == "key3")
      {
        if (!(in >> StringIO{ input.key3 }))
        {
          return in;
        }

        hasKey3 = true;
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }

    if (hasKey1 && hasKey2 && hasKey3)
    {
      dest = input;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src)
  {
    std::ostream::sentry sentry(out);

    if (!sentry)
    {
      return out;
    }

    StreamGuard Stream(out);
    out << "(:key1 0x" << std::hex << std::uppercase << src.key1
        << ":key2 (:N " << std::dec << src.key2.first << ":D "
        << src.key2.second << ":):key3 \"" << src.key3 << "\":)";

    return out;
  }
}
