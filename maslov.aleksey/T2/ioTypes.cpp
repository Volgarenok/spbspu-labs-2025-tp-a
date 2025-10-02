#include "ioTypes.hpp"
#include <iomanip>
#include <cmath>
#include <streamGuard.hpp>
#include <ioDelimiter.hpp>

std::ostream & maslov::operator<<(std::ostream & out, const DataStruct & data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard guard(out);
  out << "(";
  out << ":key1 " << DoubleSciIO{const_cast< double & >(data.key1)};
  out << ":key2 '" << data.key2;
  out << "':key3 \"" << data.key3;
  out << "\":)";
  return out;
}

std::istream & maslov::operator>>(std::istream & in, DoubleSciIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string str;
  bool hasExp = false;
  char c;
  while (in.peek() != ':' && in.peek() != std::istream::traits_type::eof())
  {
    in >> c;
    if (c == 'e' || c == 'E')
    {
      hasExp = true;
    }
    str += c;
  }
  if (!hasExp)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  try
  {
    dest.ref = std::stod(str);
  }
  catch (const std::exception &)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & maslov::operator>>(std::istream & in, CharLitIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  if (!(in >> DelimiterIO{'\''}))
  {
    return in;
  }
  StreamGuard guard(in);
  in >> std::noskipws;
  char c;
  if(!(in >> c))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  dest.ref = c;
  in >> DelimiterIO{'\''};
  return in;
}

std::istream & maslov::operator>>(std::istream & in, StringIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  if (!(in >> DelimiterIO{'"'}))
  {
    return in;
  }
  StreamGuard guard(in);
  in >> std::noskipws;
  char c;
  while ((in >> c) && (c != '"'))
  {
    dest.ref += c;
  }
  if (c != '"')
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & maslov::operator>>(std::istream & in, DataStruct & data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct temp;
  in >> DelimiterIO{'('};
  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
  for (size_t i = 0; i < 3; ++i)
  {
    in >> DelimiterIO{':'};
    std::string name;
    in >> name;
    if (name == "key1" && !hasKey1)
    {
      in >> DoubleSciIO{temp.key1};
      hasKey1 = true;
    }
    else if (name == "key2" && !hasKey2)
    {
      in >> CharLitIO{temp.key2};
      hasKey2 = true;
    }
    else if (name == "key3" && !hasKey3)
    {
      in >> StringIO{temp.key3};
      hasKey3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  in >> DelimiterIO{':'};
  in >> DelimiterIO{')'};
  data = temp;
  return in;
}

std::ostream & maslov::operator<<(std::ostream & out, const DoubleSciIO & dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  double value = dest.ref;
  int exp = std::floor(std::log10(std::abs(value)));
  double mantissa = value / std::pow(10.0, exp);
  if (std::abs(mantissa) >= 10.0)
  {
    mantissa /= 10.0;
    exp += 1;
  }
  else if (std::abs(mantissa) < 1.0)
  {
    mantissa *= 10.0;
    exp -= 1;
  }
  out << std::fixed << std::setprecision(1);
  out << mantissa << 'e' << std::showpos << exp << std::noshowpos;
  return out;
}
