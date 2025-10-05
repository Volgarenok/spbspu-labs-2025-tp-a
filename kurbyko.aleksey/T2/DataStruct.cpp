#include "DataStruct.hpp"
#include <algorithm>
#include <stdexcept>

namespace kurbyko
{
  std::string ullToBinaryString(unsigned long long value)
  {
    if (value == 0)
    {
      return "0";
    }

    std::string result;
    unsigned long long n = value;

    while (n > 0)
    {
      result = (n % 2 ? '1' : '0') + result;
      n /= 2;
    }

    return "0" + result;
  }

  unsigned long long binaryStringToULL(const std::string& str)
  {
    if (str.empty())
    {
      return 0;
    }

    unsigned long long result = 0;
    unsigned long long power = 1;

    for (int i = str.length() - 1; i >= 0; --i)
    {
      char c = str[i];
      if (c == '1')
      {
        result += power;
      }
      else if (c != '0')
      {
        throw std::invalid_argument("Invalid binary string");
      }
      power *= 2;
    }
    return result;
  }

  bool DataStruct::operator<(const DataStruct& other) const
  {
    if (key1 == other.key1)
    {
      unsigned char uc1 = static_cast<unsigned char>(key2);
      unsigned char uc2 = static_cast<unsigned char>(other.key2);

      if (uc1 == uc2)
      {
        return key3.size() < other.key3.size();
      }
      return uc1 < uc2;
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

  std::ostream& operator<<(std::ostream& out, const DataStruct& value)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    iofmtguard fmtguard(out);

    out << "(:key1 0b" << ullToBinaryString(value.key1);
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
      return in;
    }

    in >> StringIO{ "0b" };
    if (!in)
    {
      return in;
    }

    std::string binaryStr;
    char c;

    if (!in.get(c))
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (c != '0' && c != '1')
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    binaryStr += c;

    while (in.get(c) && (c == '0' || c == '1'))
    {
      binaryStr += c;
    }

    if (in)
    {
      in.unget();
    }

    try
    {
      key.value = binaryStringToULL(binaryStr);
    }
    catch (const std::exception&)
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }


  std::istream& operator>>(std::istream& in, CharI&& ch)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      in.setstate(std::ios::failbit);
      return in;
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
      return in;
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

  iofmtguard::iofmtguard(std::basic_ios< char >& s) :
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
