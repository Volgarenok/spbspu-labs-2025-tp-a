#include "dataIO.hpp"

std::istream& averenkov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = 0;
  if (in >> c && c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& averenkov::operator>>(std::istream& in, CharIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'\''};
  in >> dest.ref;
  in >> DelimiterIO{'\''};
  return in;
}

std::istream& averenkov::operator>>(std::istream& in, RationalIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  long long n = 0;
  unsigned long long d = 1;
  in >> DelimiterIO{'('};
  in >> DelimiterIO{':'};
  in >> LabelIO{"N"} >> n;
  in >> DelimiterIO{':'};
  in >> LabelIO{"D"} >> d;
  in >> DelimiterIO{':'};
  in >> DelimiterIO{')'};
  if (in)
  {
    dest.ref = {n, d};
  }
  return in;
}

std::istream& averenkov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
  return in;
}

std::istream& averenkov::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string label;
  if (in >> label && label != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
