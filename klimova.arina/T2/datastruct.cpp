#include "datastruct.hpp"
#include "iofmtguard.hpp"

std::istream& klimova::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& klimova::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> dest.ref >> DelimiterIO{ 'd' };
}

std::istream& klimova::operator>>(std::istream& in, ComplexIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  double real = 0.0;
  double imag = 0.0;
  in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };
  in >> real >> imag >> DelimiterIO{ ')' };
  if (in)
  {
    dest.ref = std::complex< double >(real, imag);
  }
  return in;
}

std::istream& klimova::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& klimova::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> dest.ref;
}

std::istream& klimova::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using sep = DelimiterIO;
    using lbl = LabelIO;
    using dbl = DoubleIO;
    using str = StringIO;
    using cmp = ComplexIO;
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    if (!(in >> sep{ '(' } >> sep{ ':' }))
    {
      return in;
    }
    for (int i = 0; i < 3; i++)
    {
      std::string label;
      in >> lbl{ label };
      if (label == "key1")
      {
        in >> dbl{ input.key1 };
      }
      else if (label == "key2")
      {
        in >> cmp{ input.key2 };
      }
      else if (label == "key3")
      {
        in >> str{ input.key3 };
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }
      in >> sep{ ':' };
    }
    in >> sep{ ')' };
  }
  if (in)
  {
    dest = std::move(input);
  }
  return in;
}

std::ostream& klimova::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  Iofmtguard fmtguard(out);
  out << "(:";
  out << "key1 " << std::fixed << std::setprecision(1) << src.key1 << "d:";
  out << "key2 #c(" << std::real(src.key2) << " " << std::imag(src.key2) << "):";
  out << "key3 " << '"' << src.key3 << '"';
  out << ":)";
  return out;
}

bool klimova::compareData(const DataStruct& a, const DataStruct& b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  if (a.key2 != b.key2)
  {
    return std::abs(a.key2) < std::abs(b.key2);
  }
  return a.key3.length() < b.key3.length();
}
