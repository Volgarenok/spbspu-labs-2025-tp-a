#include "data.hpp"
#include "stream_guard.hpp"
#include <iomanip>
#include <sstream>
#include <cmath>

namespace smirnov
{

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && c != dest.expected)
    {
      in.setstate(std::ios::failbit);
    }
      return in;
  }

  std::istream& operator>>(std::istream& in, DoubleIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    double value = 0;
    if (!(in >> value))
    {
      return in;
    }

    double abs_val = 0;
    abs_val = std::abs(value);

    if (abs_val == 0.0)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    int exp = 0;
    double mantissa = 0.0;
    exp = static_cast<int>(std::floor(std::log10(abs_val)));
    mantissa = abs_val / std::pow(10, exp);

    if (!(mantissa >= 1.0 && mantissa < 10.0))
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    dest.ref = value;
    return in;
  }

  std::istream& operator>>(std::istream& in, UllIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    std::string str;
    in >> std::ws;
    char prefix[2];
    in.read(prefix, 2);
    if (!in || prefix[0] != '0' || (prefix[1] != 'x' && prefix[1] != 'X'))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> std::hex >> dest.ref;
    if (!in)
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
    std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    return in;
  }

  std::istream& operator>>(std::istream& in, KeyNumIO& dest)
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

    iofmtguard guard(in);
    DataStruct temp{};
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;
    using sep = smirnov::DelimiterIO;
    using smirnov::DoubleIO;
    using smirnov::UllIO;
    using smirnov::StringIO;
    in >> sep{ '(' };
    for (int i = 0; i < 3; ++i)
    {
      in >> sep{ ':' };
      in >> sep{ 'k' } >> sep{ 'e' } >> sep{ 'y' };
      int keyNumber = 0;
      in >> keyNumber;
      switch (keyNumber)
      {
      case 1:
        if (hasKey1)
        {
          in.setstate(std::ios::failbit);
          break;
        }
        in >> DoubleIO{ temp.key1 };
        hasKey1 = true;
        break;
      case 2:
        if (hasKey2)
        {
          in.setstate(std::ios::failbit);
          break;
        }
        in >> UllIO{ temp.key2 };
        hasKey2 = true;
        break;
      case 3:
        if (hasKey3)
        {
          in.setstate(std::ios::failbit);
          break;
        }
        in >> StringIO{ temp.key3 };
        hasKey3 = true;
        break;
      default:
        in.setstate(std::ios::failbit);
        break;
      }
    }

    in >> sep{ ':' } >> sep{ ')' };
    if (in && hasKey1 && hasKey2 && hasKey3)
    {
      dest = temp;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DoubleIO& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    iofmtguard guard(out);

    double value = std::abs(dest.ref);
    bool is_negative = (dest.ref < 0);
    int exponent = (value == 0.0) ? 0 : static_cast<int>( std::floor(std::log10(value)));
    double mantissa = value / std::pow(10, exponent);

    if (mantissa >= 10.0)
    {
      mantissa /= 10.0;
      exponent++;
    }
    else if (mantissa < 1.0)
    {
      mantissa *= 10.0;
      exponent--;
    }

    if (is_negative)
    {
      out << '-';
    }

    out << std::fixed << std::setprecision(1) << mantissa;
    out << 'e' << (exponent >= 0 ? '+' : '-') << std::abs(exponent);
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const UllIO& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    return out << "0x" << std::uppercase << std::hex << dest.ref;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    double dbval = 0.0;
    unsigned long long ullval = 0;
    dbval = src.key1;
    ullval = src.key2;
    iofmtguard guard(out);
    out << "(:key1 " << smirnov::DoubleIO{ dbval };
    out << ":key2 " << smirnov::UllIO{ ullval };
    out << ":key3 \"" << src.key3 << "\":)";

    return out;
  }

  bool comparator(const DataStruct& a, const DataStruct& b)
  {
    if (a.key1 != b.key1)
    {
      return a.key1 < b.key1;
    }

    if (a.key2 != b.key2)
    {
      return a.key2 < b.key2;
    }
    return a.key3 < b.key3;
  }
}
