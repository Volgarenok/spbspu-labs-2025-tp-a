#include "DataStruct.hpp"
#include <iomanip>
#include <string>
#include <limits>
#include <cmath>
#include <cctype>
#include <streamGuard.hpp>

namespace
{
  void skip_ws(std::istream& in)
  {
    char ch;
    while (in.get(ch))
    {
      if (ch == std::istream::traits_type::eof())
      {
        break;
      }
      if (!std::isspace(ch))
      {
        in.putback(ch);
        break;
      }
    }
  }
}

bool shramko::dataStruct::operator<(const dataStruct& other)
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }

  if (key2 != other.key2)
  {
    return key2 < other.key2;
  }

  return key3.size() < other.key3.size();
}

std::ostream& shramko::operator<<(std::ostream& out, const DoubleScienceT& x)
{
  if (std::isnan(x.key) || std::isinf(x.key))
  {
    return out;
  }

  if (x.key == 0.0)
  {
    out << "0.0e+0";
    return out;
  }

  double abs_key = std::fabs(x.key);
  int exp = static_cast< int >(std::floor(std::log10(abs_key)));
  double mant = abs_key / std::pow(10.0, exp);

  char sign = (x.key < 0 ? '-' : '\0');
  if (sign != '\0')
  {
    out << sign;
  }

  int mant_int = static_cast< int >(mant);
  double frac_part = mant - mant_int;
  int frac_digit = static_cast< int >(frac_part * 10 + 0.5);

  out << mant_int << '.' << frac_digit << 'e';

  char exp_sign = (exp >= 0 ? '+' : '-');
  out << exp_sign;

  int abs_exp = std::abs(exp);
  out << abs_exp;

  return out;
}

std::ostream& shramko::operator<<(std::ostream& out, const UllBinT& x)
{
  out << "0b";
  if (x.key == 0)
  {
    out << "0";
  }
  else
  {
    out << std::string(x.prefix_zeroes, '0');
    bool started = false;
    for (int i = 63; i >= 0; --i)
    {
      bool bit = (x.key & (1ULL << i)) != 0;
      if (bit || started)
      {
        out << (bit ? '1' : '0');
        started = true;
      }
    }
  }
  return out;
}

std::ostream& shramko::operator<<(std::ostream& out, const StringT& x)
{
  out << '"' << x.key << '"';
  return out;
}

std::ostream& shramko::operator<<(std::ostream& out, const dataStruct& ds)
{
  DoubleScienceT x { ds.key1 };
  UllBinT y { ds.key2, ds.key2_zeroes };
  StringT z { ds.key3 };

  StreamGuard guard(out);
  out << "(";
  out << ":key1 " << x;
  out << ":key2 " << y;
  out << ":key3 " << z;
  out << ":)";
  return out;
}

std::istream& shramko::operator>>(std::istream& in, ExpectCharT&& x)
{
  StreamGuard guard(in);
  char ch;
  if (!(in >> ch) || ch != x.ch)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& shramko::operator>>(std::istream& in, DoubleScienceT& x)
{
  StreamGuard guard(in);
  skip_ws(in);

  double mant_sign = 1.0;
  char ch;
  if (in.get(ch) && ch == std::istream::traits_type::eof())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (ch == '-')
  {
    mant_sign = -1.0;
  }
  else if (ch != '+')
  {
    in.putback(ch);
  }

  double mant = 0.0;
  int frac_digits = 0;
  bool has_dot = false;
  bool has_digits = false;

  while (in.get(ch))
  {
    if (ch == std::istream::traits_type::eof())
    {
      break;
    }
    if (!std::isdigit(ch) && ch != '.')
    {
      break;
    }
    if (ch == '.')
    {
      if (has_dot)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      has_dot = true;
    }
    else
    {
      has_digits = true;
      if (has_dot)
      {
        ++frac_digits;
      }
      mant = mant * 10.0 + (ch - '0');
    }
  }
  if (ch != std::istream::traits_type::eof())
  {
    in.putback(ch);
  }
  if (!has_digits)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (frac_digits > 0)
  {
    mant /= std::pow(10.0, frac_digits);
  }
  mant *= mant_sign;

  if (!in.get(ch) || (ch != 'e' && ch != 'E'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  double exp_sign = 1.0;
  if (in.get(ch) && ch == std::istream::traits_type::eof())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (ch == '-')
  {
    exp_sign = -1.0;
  }
  else if (ch != '+')
  {
    in.putback(ch);
  }

  int exp = 0;
  bool has_exp_digits = false;
  while (in.get(ch))
  {
    if (ch == std::istream::traits_type::eof())
    {
      break;
    }
    if (!std::isdigit(ch))
    {
      break;
    }
    has_exp_digits = true;
    exp = exp * 10 + (ch - '0');
  }
  if (ch != std::istream::traits_type::eof()) in.putback(ch);

  if (!has_exp_digits)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  exp *= static_cast< int >(exp_sign);
  x.key = mant * std::pow(10.0, exp);
  return in;
}

std::istream& shramko::operator>>(std::istream& in, UllBinT& x)
{
  StreamGuard guard(in);
  if (!(in >> ExpectCharT{'0'} >> ExpectCharT{'b'}))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  x.key = 0;
  x.prefix_zeroes = 0;
  bool has_bits = false;

  char ch;
  while (in.get(ch))
  {
    if (ch == std::istream::traits_type::eof())
    {
      break;
    }
    if (ch != '0' && ch != '1')
    {
      break;
    }
    if (ch == '0' && !has_bits)
    {
      ++x.prefix_zeroes;
      continue;
    }
    has_bits = true;
    if (x.key > (std::numeric_limits< unsigned long long >::max() >> 1))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    x.key <<= 1;
    if (ch == '1')
    {
      x.key |= 1;
    }
  }
  if (ch != std::istream::traits_type::eof()) in.putback(ch);

  if (!has_bits && x.prefix_zeroes == 0)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& shramko::operator>>(std::istream& in, StringT& x)
{
  StreamGuard guard(in);
  skip_ws(in);
  in >> std::noskipws;
  in >> ExpectCharT{'"'};

  char next;
  while (in >> next && next != '"')
  {
    if (next == '\n')
    {
      in.setstate(std::ios_base::failbit);
      break;
    }
    x.key.push_back(next);
  }
  return in;
}

std::istream& shramko::operator>>(std::istream& in, dataStruct& ds)
{
  StreamGuard guard(in);
  in >> ExpectCharT{'('};
  dataStruct result;

  bool is_key1 = false, is_key2 = false, is_key3 = false;
  while ((!is_key1 || !is_key2 || !is_key3) && in)
  {
    in >> ExpectCharT{':'};
    std::string keyName;
    in >> keyName;
    if (keyName == "key1" && !is_key1)
    {
      DoubleScienceT x;
      is_key1 = true;
      in >> x;
      result.key1 = x.key;
    }
    else if (keyName == "key2" && !is_key2)
    {
      UllBinT x;
      is_key2 = true;
      in >> x;
      result.key2 = x.key;
      result.key2_zeroes = x.prefix_zeroes;
    }
    else if (keyName == "key3" && !is_key3)
    {
      StringT x;
      is_key3 = true;
      in >> x;
      result.key3 = x.key;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }

  in >> ExpectCharT{':'} >> ExpectCharT{')'};
  if (in)
  {
    ds = result;
  }

  return in;
}
