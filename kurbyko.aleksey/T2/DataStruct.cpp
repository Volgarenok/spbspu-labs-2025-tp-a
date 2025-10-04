#include "DataStruct.hpp"
#include <algorithm>

namespace dStruct
{

  bool DataStruct::operator<(const DataStruct& other) const
  {
    if (key1 == other.key1)
    {
      if (std::abs(key2) == std::abs(other.key2))
      {
        return key3.size() < other.key3.size();
      }
      return std::abs(key2) < std::abs(other.key2);
    }
    return key1 < other.key1;
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

  std::string convertIntoBinNumber(unsigned long long value)
  {
    std::bitset< 64 > bin(value);
    std::string binString = bin.to_string();
    return "0" + binString.erase(0, binString.find('1'));
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& value)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    iofmtguard fmtguard(out);

    out << "(:key1 0b" << convertIntoBinNumber(value.key1);
    out << ":key2 \'" << value.key2 << "\'";
    out << ":key3 \"" << value.key3 << "\":)";

    return out;
  }

  std::istream& operator>>(std::istream& in, ULLBinary&& key)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      in.setstate(std::ios::failbit);
    }
    std::bitset< 64 > bin;
    in >> StringIO{ "0b" } >> bin;
    if (in)
    {
      key.value = bin.to_ulong();
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, CharI&& ch)
  {
    std::istream::sentry guard(in);
    if (!guard)
    {
      in.setstate(std::ios::failbit);
    }

    in >> DelimiterIO{ '\'' } >> ch.value >> DelimiterIO{ '\'' };

    return in;
  }

  std::istream& operator>>(std::istream& in, StrKeyI&& key)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      in.setstate(std::ios::failbit);
    }
    std::string tmp{};
    std::getline(in >> DelimiterIO{ '"' }, tmp, '"');
    if (in)
    {
      key.value = tmp;
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, const StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    for (size_t i = 0; dest.expected[i] != '\0'; ++i)
    {
      char c{};
      in >> c;

      if (std::isalpha(c))
      {
        c = std::tolower(c);
      }

      if (c != dest.expected[i])
      {
        in.setstate(std::ios::failbit);
        break;
      }
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& value)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    iofmtguard fmtguard(in);

    size_t KEY_AMOUNT = 3;
    unsigned int keyI{};

    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

    while (in && KEY_AMOUNT > 0)
    {
      in >> StringIO{ "key" } >> keyI;
      switch (keyI)
      {
      case 1:
      {
        in >> ULLBinary{ value.key1 };
        break;
      }
      case 2:
      {
        in >> CharI{ value.key2 };
        break;
      }
      case 3:
      {
        in >> StrKeyI{ value.key3 };
        break;
      }
      default:
      {
        in.setstate(std::ios::failbit);
      }
      }
      --KEY_AMOUNT;
      in >> DelimiterIO{ ':' };
    }

    in >> DelimiterIO{ ')' };
    return in;
  }

  iofmtguard::iofmtguard(std::basic_ios< char >& s):
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  iofmtguard::~iofmtguard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}
