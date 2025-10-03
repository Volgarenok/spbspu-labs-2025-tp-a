#include "dataStruct.hpp"
#include "streamGuard.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>

namespace cherkasov
{
  bool DataStruct::operator<(const DataStruct& other) const
  {
    if (key1 == other.key1)
    {
      if (key2 == other.key2)
      {
        return key3.size() < other.key3.size();
      }
      if (key2.first == other.key2.first)
      {
        return key2.second < other.key2.second;
      }
      return key2.first < other.key2.first;
    }
    return std::abs(key1) < std::abs(other.key1);
  }

  std::istream& operator>>(std::istream& in, DelimiterIO&& obj)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    char c = 0;
    in >> c;
    if (in && c != obj.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, ComplexIO&& obj)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> DelimiterIO{'#'} >> DelimiterIO{'c'} >> DelimiterIO{'('};
    double re = 0, im = 0;
    in >> re >> im;
    in >> DelimiterIO{')'};
    obj.ref = std::complex<double>(re, im);
    return in;
  }

  std::istream& operator>>(std::istream& in, RationalIO&& obj)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> DelimiterIO{'('};
    std::string label;
    in >> label;
    if (label != ":N")
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> obj.ref.first;
    in >> DelimiterIO{':'};
    in >> label;
    if (label != "D")
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> obj.ref.second;
    in >> DelimiterIO{':'};
    in >> DelimiterIO{')'};
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& obj)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> DelimiterIO{'\"'};
    std::getline(in, obj.ref, '\"');
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& obj)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    bool has1 = false, has2 = false, has3 = false;
    DataStruct tmp;
    try
    {
      in >> DelimiterIO{'('};
      while ((!has1 || !has2 || !has3) && in)
      {
        std::string label;
        in >> label;
        if (label == ":key1" && !has1)
        {
          in >> ComplexIO{tmp.key1};
          has1 = true;
        }
        else if (label == ":key2" && !has2)
        {
          in >> RationalIO{tmp.key2};
          has2 = true;
        }
        else if (label == ":key3" && !has3)
        {
          in >> StringIO{tmp.key3};
          has3 = true;
        }
        else
        {
          throw std::runtime_error("Unsupported key");
        }
      }
      in >> DelimiterIO{':'} >> DelimiterIO{')'};
      if (!has1 || !has2 || !has3)
      {
        throw std::runtime_error("Not enough keys");
      }
    }
    catch (...)
    {
      in.setstate(std::ios::failbit);
    }

    if (in)
    {
      obj = tmp;
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& obj)
  {
    std::ostream::sentry s(out);
    if (!s)
    {
      return out;
    }
    StreamGuard guard(out);
    out << std::fixed << std::showpoint << std::setprecision(1);
    out << "(:key1 #c(" << obj.key1.real() << " " << obj.key1.imag() << "):";
    out << "key2 (:N " << obj.key2.first << ":D " << obj.key2.second << ":):";
    out << "key3 \"" << obj.key3 << "\":)";
    return out;
  }
}
