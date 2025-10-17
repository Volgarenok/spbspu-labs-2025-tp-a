#include "keys.hpp"
#include "delimiter.hpp"

std::istream& shabalin::operator>>(std::istream& in, ComplexKey&& key)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  double re = 0;
  double im = 0;
  in >> DelimiterStr{ "#c(" } >> re >> im >> DelimiterSym{ ')' };
  if (in)
  {
    key.value = { re, im };
  }
  return in;
}

std::istream& shabalin::operator>>(std::istream& in, DoubleKey&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  int mantissa = 0;
  int number = 0;
  int power = 0;
  in >> mantissa >> DelimiterSym{ '.' } >> number >> DelimiterSym{ 'E' } >> power;
  value.value = (mantissa * 1.0 + number * 0.01) * std::pow(10, power);
  return in;
}

std::istream& shabalin::operator>>(std::istream& in, StringKey&& key)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> shabalin::DelimiterSym{ '"' };
  std::getline(in, key.value, '"');
  return in;
}

