#include "data_input.hpp"
#include "stream_guard.hpp"

using Del = trukhanov::DelimiterIO;
using Lbl = trukhanov::LabelIO;

std::istream& trukhanov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '\0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& trukhanov::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  for (char expected : dest.exp)
  {
    char actual = '\0';
    in.get(actual);
    if (!in || actual != expected)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  return in;
}

std::istream& trukhanov::operator>>(std::istream& in, OctalULLIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  StreamGuard guard(in);
  in >> std::oct >> dest.ref;
  return in;
}

std::istream& trukhanov::operator>>(std::istream& in, RationalIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  long long num = 0;
  unsigned long long den = 0;

  in >> Del{ '(' } >> Del{ ':' } >> Lbl{ "N" } >> num >> Del{ ':' };
  in >> Lbl{ "D" } >> den >> Del{ ':' } >> Del{ ')' };

  if (in && den != 0)
  {
    dest.ref = { num, den };
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& trukhanov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}
