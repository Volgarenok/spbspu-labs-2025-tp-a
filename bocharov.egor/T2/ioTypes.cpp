#include "ioTypes.hpp"
#include <iomanip>
#include <cmath>
#include <ioDelimiter.hpp>
#include <streamGuard.hpp>

std::ostream & bocharov::operator<<(std::ostream & out, const DataStruct & data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  bocharov::StreamGuard guard(out);
  double dbl = data.key1;
  out << "(";
  out << ":key1 " << DoubleSciIO{dbl};
  out << ":key2 '" << data.key2;
  out << "':key3 \"" << data.key3;
  out << "\":)";
  return out;
}

std::istream & bocharov::operator>>(std::istream & in, DoubleSciIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string str;
  if (!std::getline(in, str, ':'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (str.find('e') == str.find('E'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  try
  {
    size_t index = 0;
    double number = std::stod(str, & index);
    if (index != str.length())
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in.unget();
    dest.ref = number;
  }
  catch (const std::exception & e)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & bocharov::operator>>(std::istream & in, CharLitIO && dest)
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
  bocharov::StreamGuard guard(in);
  in >> std::noskipws;
  char c;
  if (!(in >> c))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  dest.ref = c;
  in >> DelimiterIO{'\''};
  return in;
}

std::istream & bocharov::operator>>(std::istream & in, StringIO && dest)
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
  bocharov::StreamGuard guard(in);
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

std::istream & bocharov::operator>>(std::istream & in, DataStruct & data)
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

std::ostream & bocharov::operator<<(std::ostream & out, const DoubleSciIO & dest)
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
