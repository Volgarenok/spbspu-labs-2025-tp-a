#include "data_struct.hpp"
#include <exception>
#include <vector>
#include <iomanip>
#include <cmath>

brevnov::StreamGuard::StreamGuard(std::basic_ios< char > & s):
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

brevnov::StreamGuard::~StreamGuard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

std::istream& brevnov::operator>>(std::istream& input, DelimiterIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  char c = '0';
  input >> c;
  if (input && (std::tolower(c) != dest.exp))
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& brevnov::operator>>(std::istream& input, DelimitersIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  for (char c: dest.exp)
  {
    input >> DelimiterIO{ c };
  }
  return input;
}

std::istream& brevnov::operator>>(std::istream& input, UnLongLongIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::string num;
  std::getline(input >> DelimiterIO{ '0' }, num, ':');
  if (num.empty())
  {
    input.setstate(std::ios::failbit);
    return input;
  }
  try
  {
    dest.ref = std::stoull(num, nullptr, 8);
  }
  catch (const std::exception&)
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& brevnov::operator>>(std::istream& in, ComplexIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  double real = 0.0;
  double imag = 0.0;
  std::string str = "#c(";
  in >> DelimitersIO{ str };
  str = "):";
  in >> real;
  in >> imag;
  in >> DelimitersIO{ str };
  if (in)
  {
    dest.ref = std::complex< double >(real, imag);
  }
  return in;
}

std::istream& brevnov::operator>>(std::istream& input, StringIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::getline(input >> DelimiterIO{ '"' }, dest.ref, '"');
  return input >> DelimiterIO{ ':' };
}

std::istream& brevnov::operator>>(std::istream& input, KeyIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::string str = "key";
  input >> DelimitersIO{ str };
  int key;
  input >> key;
  if (std::find(dest.keys.begin(), dest.keys.end(), key) != dest.keys.end())
  {
    input.setstate(std::ios::failbit);
    return input;
  }
  auto it = std::find(dest.keys.begin(), dest.keys.end(), 0);
  if (it == dest.keys.end())
  {
    input.setstate(std::ios::failbit);
    return input;
  }
  *it = key;
  switch (key)
  {
  case 1:
    input >> UnLongLongIO{ dest.data.key1 };;
    break;
  case 2:
    input >> ComplexIO{ dest.data.key2 };
    break;
  case 3:
    input >> StringIO{ dest.data.key3 };
    break;
  default:
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& brevnov::operator>>(std::istream& input, DataStruct& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  DataStruct temp;
  StreamGuard scope(input);
  {
    std::string str = "(:";
    input >> DelimitersIO{ str };
    input >> KeyIO{ temp };
    input >> KeyIO{ temp };
    input >> KeyIO{ temp };
    input >> DelimiterIO{ ')' };
  }
  if (input)
  {
    dest = temp;
  }
  return input;
}

std::ostream& brevnov::operator<<(std::ostream& output, const UnLongLongIO&& dest)
{
  return output << '0' << std::oct << dest.ref;
}

std::ostream& brevnov::operator<<(std::ostream& output, const ComplexIO&& dest)
{
  output << "#c(" << std::fixed << std::setprecision(1) << dest.ref.real();
  output << " " << dest.ref.imag() << ")";
  return output;
}

std::ostream& brevnov::operator<<(std::ostream &output, const StringIO&& dest)
{
  return output << "\"" << dest.ref << "\"";
}

std::ostream& brevnov::operator<<(std::ostream& output, const DataStruct& dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry)
  {
    return output;
  }
  unsigned long long ull = dest.key1;
  std::complex< double > complex = dest.key2;
  std::string string = dest.key3;
  StreamGuard scope(output);
  output << "(:key1 " << UnLongLongIO{ ull };
  output << ":key2 " << ComplexIO{ complex };
  output << ":key3 " << StringIO{ string } << ":)";
  return output;
}

bool brevnov::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  const double lhscom = std::abs(lhs.key2);
  const double rhscom = std::abs(rhs.key2);
  if (lhscom != rhscom)
  {
    return lhscom < rhscom;
  }
  return lhs.key3.size() < rhs.key3.size();
}
