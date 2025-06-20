#include "dataStruct.hpp"
#include "streamGuard.hpp"
#include <sstream>
#include <cmath>
#include <iomanip>

namespace cherkasov
{
  bool DataStruct::operator<(const DataStruct& other) const
  {
    if (std::abs(key1) != std::abs(other.key1))
    {
      return std::abs(key1) < std::abs(other.key1);
    }
    if (key2 != other.key2)
    {
      return key2 < other.key2;
    }
    return key3.size() < other.key3.size();
  }

  std::istream& operator>>(std::istream& in, ExpectChar&& d)
  {
    char c;
    if (!(in >> c) || c != d.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Complex&& io)
  {
    StreamGuard g(in);
    in >> ExpectChar{'#'} >> ExpectChar{'c'} >> ExpectChar{'('};
    double re = 0.0, im = 0.0;
    in >> re;
    if (in.peek() == '+' || in.peek() == '-')
    {
      in >> im;
    }
    in >> ExpectChar{')'};
    if (in)
    {
      io.c = std::complex<double>(re, im);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Rational&& io)
  {
    StreamGuard guard(in);
    in >> ExpectChar{'('} >> ExpectChar{':'} >> ExpectChar{'N'} >> io.rat.first
       >> ExpectChar{':'} >> ExpectChar{'D'} >> io.rat.second
       >> ExpectChar{':'} >> ExpectChar{')'};
    return in;
  }

  std::istream& operator>>(std::istream& in, Strings&& io)
  {
    StreamGuard guard(in);
    in >> std::noskipws >> ExpectChar{'"'};
    char ch;
    while (in >> ch && ch != '"')
    {
      if (ch == '\n')
      {
        in.setstate(std::ios::failbit);
        break;
      }
      io.s += ch;
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& obj)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    DataStruct temp;
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;
    in >> ExpectChar{'('};
    while (in)
    {
      std::string label;
      in >> ExpectChar{':'} >> label;
      if (label == "key1")
      {
        if (in.peek() == '#')
        {
          in >> Complex{temp.key1};
          hasKey1 = true;
        }
        else
        {
         std::string dummy;
          std::getline(in, dummy, ':');
        }
      }
      else if (label == "key2")
      {
        if (in.peek() == '(')
        {
          in >> Rational{temp.key2};
          hasKey2 = true;
        }
        else
        {
          std::string dummy;
          std::getline(in, dummy, ':');
        }
      }
      else if (label == "key3")
      {
        in >> Strings{temp.key3};
        hasKey3 = true;
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }
      if (in.peek() == ':')
      {
        in >> ExpectChar{':'};
        if (in.peek() == ')')
        {
          in >> ExpectChar{')'};
          break;
        }
      }
    }
    if (in && hasKey1 && hasKey2 && hasKey3)
    {
      obj = temp;
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& obj)
  {
    StreamGuard guard(out);
    out << "(:key1 #c(" << std::fixed << std::setprecision(1) << obj.key1.real();
    if (obj.key1.imag() >= 0)
    {
      out << "+";
    }
    out << obj.key1.imag() << ")";
    out << ":key2 (:N" << obj.key2.first << ":D" << obj.key2.second << ":)";
    out << ":key3 \"" << obj.key3 << "\":)";
    return out;
  }
}
