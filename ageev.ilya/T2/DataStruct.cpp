#include "DataStruct.hpp"
#include <cmath>
#include <string>
#include <iomanip>
#include <iostream>

namespace ageev
{
  bool DataStruct::operator<(const DataStruct& var) const
  {
    if (key1 == var.key1)
    {
      if (key2 == var.key2)
      {
        return key3.size() < var.key3.size();
      }
      return key2 < var.key2;
    }
    return key1 < var.key1;
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

  std::istream& operator>>(std::istream& in, DoubleIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return in >> dest.ref >> DelimiterIO{ 'd' };
  }

  std::istream& operator>>(std::istream& in, DoubleSciIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    std::string str;
    try
    {
      while (in >> c && std::tolower(c) != 'e')
      {
        str += c;
      }
      if (str.empty())
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      double number = std::stod(str);
      str.clear();
      while (in >> c && c != ':')
      {
        str += c;
      }
      int degree = std::stoi(str);
      in.unget();
      dest.ref = number * std::pow(10, degree);
    }
    catch (...)
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
    in >> data;
    if (data != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream & operator<<(std::ostream& out, const DoubleToSciForm& dest)
  {
    Iofmtguard guard(out);
    if (dest.ref == 0.0)
    {
      out << "0.0e+0";
      return out;
    }

    int exponent = 0;
    double mantissa = dest.ref;

    while (std::abs(mantissa) >= 10.0)
    {
      mantissa /= 10.0;
      ++exponent;
    }
    while (std::abs(mantissa) < 1.0)
    {
      mantissa *= 10.0;
      --exponent;
    }

    out.setf(std::ios::fixed);
    out.precision(1);
    out << mantissa;

    out << 'e';
    if (exponent >= 0)
    {
      out << '+' << exponent;
    }
    else
    {
      out << '-' << -exponent;
    }

    return out;
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    DataStruct input;
    bool hasKey1 = false,
    hasKey2 = false,
    hasKey3 = false;
    int fields = 0;

    using sep = DelimiterIO;
    using dbl = DoubleIO;
    using dblsci = DoubleSciIO;
    using str = StringIO;

    in >> sep{'('};
    while (fields < 3 && in)
    {
      std::string key;
      in >> sep{':'} >> key;
      if (key == "key1" && !hasKey1)
      {
        if (in >> dbl{input.key1})
        {
          hasKey1 = true;
        }
        else break;
      }
      else if (key == "key2" && !hasKey2)
      {
        if (in >> dblsci{input.key2})
        {
          hasKey2 = true;
        }
        else break;
      }
      else if (key == "key3" && !hasKey3)
      {
        if (in >> str{input.key3})
        {
          hasKey3 = true;
        }
        else break;
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }

      fields++;
    }
    in >> sep{':'} >> sep{')'};
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
    Iofmtguard fmtguard(out);
    out << "(:key1 " << std::fixed << std::setprecision(1);
    out << src.key1 << "d:" << "key2 ";
    out << DoubleToSciForm{src.key2} << ":" << "key3 \"" << src.key3 << "\":)";
    return out;
  }

  Iofmtguard::Iofmtguard(std::basic_ios< char >& s):
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  Iofmtguard::~Iofmtguard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}
