#include "data_struct.hpp"
#include <cmath>
#include <complex>
#include <iomanip>
#include <ios>
#include <iostream>
#include <utility>
#include "stream_guard.hpp"

namespace
{
  bool avaliable(unsigned int k, evstyunichev::KeyIO &dest)
  {
    return (1 << k) & dest.done;
  }
}

double evstyunichev::abscmpl(const std::complex< double > &cmpl)
{
  return std::pow(std::pow(cmpl.real(), 2) + std::pow(cmpl.imag(), 2), 0.5);
}

std::istream & evstyunichev::operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, DelimitersIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  std::noskipws(in);
  for (char c: dest.exps)
  {
    in >> DelimiterIO{ c };
  }
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, UllIO &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  unsigned long long ull = 0;
  {
    in >> ull;
    in >> DelimitersIO{ "ull" };
  }
  if (in)
  {
    dest.ref = ull;
  }
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, CmplIO &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  double a = 0, b = 0;
  std::skipws(in);
  in >> DelimitersIO{ "#c(" };
  in >> a >> b >> DelimiterIO{ ')' };
  if (in)
  {
    dest.cmpl = std::complex< double >{ a, b };
  }
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, KeyIO &key)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  unsigned int k = 0;
  {
    evstyunichev::StreamGuard guard(in);
    in >> DelimitersIO{ ":key" };
    std::noskipws(in);
    in >> k;
  }
  if (k == 1 && avaliable(k, key))
  {
    UllIO ull{ key.data.key1 };
    in >> ull;
  }
  else if (k == 2 && avaliable(k, key))
  {
    CmplIO cmpl{ key.data.key2 };
    in >> cmpl;
  }
  else if (k == 3 && avaliable(k, key))
  {
    StringIO str{ key.data.key3 };
    in >> str;
  }
  else
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  key.done -= (1 << k);
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, StringIO &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  std::skipws(in);
  in >> DelimiterIO{ '"' };
  std::noskipws(in);
  std::getline(in, dest.ref, '"');
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, DataStruct &data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct temp;
  in >> DelimiterIO{ '(' };
  KeyIO key{ temp, (UllKey | CmplKey | StrKey) };
  in >> key;
  in >> key;
  in >> key;
  in >> DelimitersIO{ ":)" };
  if (in && !key.done)
  {
    data = temp;
  }
  return in;
}

std::ostream & evstyunichev::operator<<(std::ostream &out, UllIO &&data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << data.ref << "ull";
  return out;
}

std::ostream & evstyunichev::operator<<(std::ostream &out, CmplIO &&data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  double a = data.cmpl.real(), b = data.cmpl.imag();
  {
    StreamGuard guard(out);
    if (floor(a) == a)
    {
      out << std::fixed << std::setprecision(1);
    }
    out << "#c(" << a << ' ';
  }
  {
    StreamGuard guard(out);
    if (floor(b) == b)
    {
      out << std::fixed << std::setprecision(1);
    }
    out << b << ')';
  }
  return out;
}

std::ostream & evstyunichev::operator<<(std::ostream &out, StringIO &&data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << '"' << data.ref << '"';
  return out;
}

std::ostream & evstyunichev::operator<<(std::ostream &out, const DataStruct &data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard guard(out);
  unsigned long long ull = data.key1;
  std::complex< double > cmpl = data.key2;
  std::string string = data.key3;
  out << "(:key1 " << UllIO{ ull };
  out << ":key2 " << CmplIO{ cmpl };
  out << ":key3 " << StringIO{ string } << ":)";
  return out;
}

bool evstyunichev::comparator(const DataStruct &a, const DataStruct &b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  if (a.key2 != b.key2)
  {
    return abscmpl(a.key2) < abscmpl(b.key2);
  }
  return a.key3.size() < b.key3.size();
}
