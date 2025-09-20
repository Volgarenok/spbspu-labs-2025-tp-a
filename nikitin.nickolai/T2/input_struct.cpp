#include "input_struct.hpp"

std::istream& nikitin::operator>>(std::istream& in, DelimiterIO&& dest)
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
    in.setstate(std::ios_base::failbit);
  }
  return in;
}

std::istream& nikitin::operator>>(std::istream& in, LongLongIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref;

  char suf[3]{'\0'};
  char c = '\0';
  for (int i = 0; i < 2; ++i)
  {
    if (!(in >> c))
    {
      return in;
    }
    suf[i] = std::tolower(c);
  }
  if (std::strcmp(suf, "ll") != 0)
  {
    in.setstate(std::ios_base::failbit);
  }
  return in;
}

std::istream& nikitin::operator>>(std::istream& in, ComplexIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  double real, imag = 0;
  in >> DelimiterIO{'#'} >> DelimiterIO{'c'} >> DelimiterIO{'('};
  in >> real >> imag;
  in >> DelimiterIO{')'};
  if (in)
  {
    dest.ref = std::complex< double >(real, imag);
  }
  return in;
}

std::istream& nikitin::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}
