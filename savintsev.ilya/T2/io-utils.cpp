#include "io-utils.hpp"
#include <cmath>
#include <scope-guard.hpp>
#include <io-delimiter.hpp>

std::istream & savintsev::operator>>(std::istream & in, DoubleI && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string token;
  if (!std::getline(in, token, ':'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (token.find('e') == std::string::npos && token.find('E') == std::string::npos)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  try
  {
    size_t idx = 0;
    double value = std::stod(token, &idx);

    if (idx != token.size())
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    dest.ref_ = value;
  }
  catch (const std::exception &)
  {
    in.setstate(std::ios::failbit);
  }
  in.unget();
  return in;
}

std::istream & savintsev::operator>>(std::istream & in, StringIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, dest.ref_, '"');
}

std::istream & savintsev::operator>>(std::istream & in, UllIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  ScopeGuard guard(in);
  in >> std::oct >> dest.ref_;

  return in;
}


std::istream & savintsev::operator>>(std::istream & in, LabelIO & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string data = "";
  if (!std::getline(in, data, ' '))
  {
    in.setstate(std::ios::failbit);
  }
  dest.exp_ = data;
  return in;
}

std::ostream & savintsev::operator<<(std::ostream & out, const DoubleO & dest)
{
  savintsev::ScopeGuard guard(out);

  if (dest.exp_ == 0.0)
  {
    out << "0.0e+0";
    return out;
  }

  int exponent = 0;
  double mantissa = dest.exp_;

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
