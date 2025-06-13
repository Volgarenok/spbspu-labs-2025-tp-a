#include "DataStruct.h"
#include <iomanip>
#include <limits>

namespace khokhryakova
{
  Iofmtguard::Iofmtguard(std::ostream& s):
    s_(s),
    flags_(s.flags())
  {}

  Iofmtguard::~Iofmtguard()
  {
    s_.flags(flags_);
  }

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '\0' ;
    if ((in >> c) && (c != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
  bool check_for_ll_suffix(std::istream& in)
  {
    char first_char = '0', second_char = '0';
    in >> first_char >> second_char;
    return (first_char == 'l' || first_char == 'L') && (second_char == 'l' || second_char == 'L');
  }

  std::istream& operator>>(std::istream& in, LongLongIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    long long number_value = 0;
    in >> number_value;
    if (!in)
    {
      return in;
    }
    bool has_ll_suffix = check_for_ll_suffix(in);
    if (in && has_ll_suffix)
    {
      dest.ref = number_value;
    }
    else
    {
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
    in >> DelimiterIO{'('} >> DelimiterIO{':'} >> DelimiterIO{'N'};
    in >> dest.ref.first;
    in >> DelimiterIO{':'} >> DelimiterIO{'D'};
    in >> dest.ref.second;
    in >> DelimiterIO{':'} >> DelimiterIO{')'};
    if (dest.ref.second == 0)
    {
      in.setstate(std::ios::failbit);
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
    in >> DelimiterIO{ '"' };
    std::getline(in, dest.ref, '"');
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    DataStruct temp;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
    for (size_t i = 0; i < 3; ++i)
    {
      std::string key;
      in >> key;
      if (key == "key1" && !hasKey1)
      {
        in >> LongLongIO{temp.key1};
        hasKey1 = true;
      }
      else if (key == "key2" && !hasKey2)
      {
        in >> RationalIO{temp.key2};
        hasKey2 = true;
      }
      else if (key == "key3" && !hasKey3)
      {
        in >> StringIO{temp.key3};
        hasKey3 = true;
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> DelimiterIO{':'};
    }
    in >> DelimiterIO{ ')' };
    if (hasKey1 && hasKey2 && hasKey3)
    {
      dest = temp;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src)
  {
    Iofmtguard guard(out);
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << "(:key1 " << src.key1 << "ll";
    out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
    out << ":key3 \"" << src.key3 << "\":)";
    return out;
  }

  bool operator<(const DataStruct& a, const DataStruct& b)
  {
    if (a.key1 != b.key1)
    {
      return a.key1 < b.key1;
    }
    const auto left = a.key2.first * b.key2.second;
    const auto right = b.key2.first * a.key2.second;
    if (left != right)
    {
      return left < right;
    }
    if (a.key3.length() != b.key3.length())
    {
      return a.key3.length() < b.key3.length();
    }
    return a.key3 < b.key3;
  }
}
