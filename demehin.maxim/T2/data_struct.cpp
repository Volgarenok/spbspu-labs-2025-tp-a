#include "data_struct.hpp"
#include <cctype>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "scope_guard.hpp"

namespace
{
  using namespace demehin::ioStructs;
}

std::istream& demehin::ioStructs::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '0';
  in >> c;
  if (in && (std::tolower(c) != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& demehin::ioStructs::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string num;
  std::getline(in, num, ':');
  size_t ePos = num.find_first_of("eE");
  if (ePos == std::string::npos || num[ePos] == num.back())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (num[ePos + 1] != '+' && num[ePos + 1] != '-')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::istringstream iss(num);
  if (!(iss >> dest.ref))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& demehin::ioStructs::operator>>(std::istream& in, LlIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> dest.ref >> DelimiterIO{ 'l' } >> DelimiterIO{ 'l' };
}

std::istream& demehin::ioStructs::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& demehin::ioStructs::operator>>(std::istream& in, KeyNumIO& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string label = "";
  in >> label;
  if (label.substr(0, 3) != "key")
  {
    in.setstate(std::ios::failbit);
  }
  dest.ref = label.back();
  return in;
}

std::istream& demehin::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using sep = DelimiterIO;
    using keyNum = KeyNumIO;
    using dbl = DoubleIO;
    using ll = LlIO;
    using str = StringIO;
    in >> sep{ '(' } >> sep{ ':' };
    for (size_t i = 0; i < 3; i++)
    {
      keyNum key_num{ 0 };
      in >> key_num;
      if (key_num.ref == '1')
      {
        in >> ll{ input.key1 } >> sep{ ':' };
      }
      else if (key_num.ref == '2')
      {
        in >> dbl{ input.key2 };
      }
      else if (key_num.ref == '3')
      {
        in >> str{ input.key3 } >> sep{ ':' };
      }
    }
    in >> sep { ')' };
  }
  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream& demehin::ioStructs::operator<<(std::ostream& out, const DoubleIO&& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  demehin::iofmtguard fmtguard(out);

  double val = dest.ref;
  bool is_negative = val < 0;
  val = std::abs(val);
  int exp = (val == 0.0) ? 0 : std::floor(std::log10(val));
  double mant = val / std::pow(10, exp);

  if (mant >= 10.0)
  {
    mant /= 10.0;
    exp++;
  }
  else if (mant < 1.0)
  {
    mant *= 10.0;
    exp--;
  }

  if (is_negative)
  {
    out << '-';
  }

  out << std::fixed << std::setprecision(1) << mant;
  out << 'e' << (exp >= 0 ? '+' : '-') << std::abs(exp);
  return out;
}

std::ostream& demehin::ioStructs::operator<<(std::ostream& out, const LlIO&& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << dest.ref << "ll";
}

std::ostream& demehin::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  iofmtguard fmtguard(out);
  long long llval = src.key1;
  double dblval = src.key2;
  out << "(:key1 " << LlIO{ llval };
  out << ":key2 " << DoubleIO{ dblval };
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}
