#include "input_struct.hpp"
#include "format_guard.hpp"

std::istream &filonova::operator>>(std::istream &in, DelimiterValue &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char current;
  in >> current;
  if (in && (current != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &filonova::operator>>(std::istream &in, HexValue &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  FormatGuard guard(in);

  in >> DelimiterValue{'0'} >> DelimiterValue{'x'};
  in >> std::hex >> dest.ref;
  return in;
}

std::istream &filonova::operator>>(std::istream &in, ComplexValue &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterValue{'#'} >> DelimiterValue{'c'} >> DelimiterValue{'('};
  double real = 0.0, imag = 0.0;
  in >> real >> imag >> DelimiterValue{')'};

  if (in)
  {
    dest.ref = std::complex< double >(real, imag);
  }
  return in;
}

std::istream &filonova::operator>>(std::istream &in, StringValue &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterValue{'"'};
  std::getline(in, dest.ref, '"');

  return in;
}
