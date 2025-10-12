#include "DataStruct.h"
#include <iomanip>
#include <cmath>
#include <limits>

guseynov::FormatGuard::FormatGuard(std::basic_ios<char>& s):
  s_(s),
  fmt_(s.flags()),
  precision_(s.precision()),
  fill_(s.fill())
{}

guseynov::FormatGuard::~FormatGuard()
{
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

bool guseynov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  double modLhs = std::abs(lhs.key2);
  double modRhs = std::abs(rhs.key2);
  if (std::abs(modLhs - modRhs) > 1e-10)
  {
    return modLhs < modRhs;
  }
  return lhs.key3.length() < rhs.key3.length();
}

std::istream& guseynov::operator>>(std::istream& in, DelimiterValue&& dest)
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

std::istream& guseynov::operator>>(std::istream& in, HexValue&& dest)
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

std::istream& guseynov::operator>>(std::istream& in, ComplexValue&& dest)
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
    dest.ref = std::complex<double>(real, imag);
  }
  return in;
}

std::istream& guseynov::operator>>(std::istream& in, StringValue&& dest)
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

std::istream& guseynov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct input;
  using sep = DelimiterValue;
  using hex = HexValue;
  using cmp = ComplexValue;
  using str = StringValue;

  in >> sep{'('};

  size_t keysRead = 0;
  bool key1Read = false, key2Read = false, key3Read = false;

  while (keysRead < 3 && in)
  {
    std::string key;
    in >> sep{':'} >> key;

    if (key == "key1")
    {
      if (key1Read)
      {
        in.setstate(std::ios::failbit);
        break;
      }
      in >> hex{input.key1};
      key1Read = true;
      keysRead++;
    }
    else if (key == "key2")
    {
      if (key2Read)
      {
        in.setstate(std::ios::failbit);
        break;
      }
      in >> cmp{input.key2};
      key2Read = true;
      keysRead++;
    }
    else if (key == "key3")
    {
      if (key3Read)
      {
        in.setstate(std::ios::failbit);
        break;
      }
      in >> str{input.key3};
      key3Read = true;
      keysRead++;
    }
    else
    {
      in.setstate(std::ios::failbit);
      break;
    }
  }

  if (in)
  {
    in >> sep{':'} >> sep{')'};
    dest = input;
  }

  return in;
}

std::ostream& guseynov::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  FormatGuard guard(out);
  out << "(:key1 0x" << std::hex << std::uppercase << src.key1;
  out << ":key2 #c(" << std::fixed << std::setprecision(1);
  out << src.key2.real() << " " << src.key2.imag() << ")";
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}
