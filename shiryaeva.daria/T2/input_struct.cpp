#include "input_struct.hpp"
#include "format_guard.hpp"
#include <iomanip>

std::istream &shiryaeva::operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char current;
  if (in >> current && current != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &shiryaeva::operator>>(std::istream &in, HexUllIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  shiryaeva::FormatGuard guard(in);
  in >> DelimiterIO{'0'} >> DelimiterIO{'x'};
  in >> std::hex >> dest.ref;
  return in;
}

std::istream &shiryaeva::operator>>(std::istream &in, CharIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char quote, ch, endquote;
  in >> quote >> ch >> endquote;

  if (quote != '\'' || endquote != '\'')
  {
    in.setstate(std::ios::failbit);
  }
  else
  {
    dest.ref = ch;
  }
  return in;
}

std::istream &shiryaeva::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{'"'};
  std::getline(in, dest.ref, '"');
  return in;
}
