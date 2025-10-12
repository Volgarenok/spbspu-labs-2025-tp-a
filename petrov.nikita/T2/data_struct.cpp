#include "data_struct.hpp"

petrov::StreamGuard::StreamGuard(std::basic_ios< char > & s):
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

petrov::StreamGuard::~StreamGuard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

std::istream & petrov::operator>>(std::istream & in, DelimiterIO && dest)
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

std::istream & petrov::operator>>(std::istream & in, LabelIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string data = "";
  char symbol = '0';
  for (size_t i = 0; i < 3; i++)
  {
    if (in >> symbol)
    {
      data += symbol;
    }
  }
  if (data != dest.exp)
  {
    in.setstate(std::ios_base::failbit);
  }
  return in;
}

std::istream & petrov::operator>>(std::istream & in, DoubleIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref;
  char postfix = '0';
  in >> postfix;
  if (in && postfix != 'd' && postfix != 'D')
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & petrov::operator>>(std::istream & in, LongLongIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref;
  char postfix = '0';
  in >> postfix;
  if (in && postfix == 'l')
  {
    return in >> DelimiterIO{ 'l' };
  }
  else if (in && postfix == 'L')
  {
    return in >> DelimiterIO{ 'L' };
  }
  else if (in)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & petrov::operator>>(std::istream & in, StringIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream & petrov::operator>>(std::istream & in, DataStruct & data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  std::string key = "";
  {
    using sep = DelimiterIO;
    using lbl = LabelIO;
    using dbl = DoubleIO;
    using lli = LongLongIO;
    using str = StringIO;
    in >> sep{ '(' };
    in >> sep{ ':' };
    bool used_keys[3] = { false, false, false };
    char key = '0';
    for (size_t i = 0; i < 3; i++)
    {
      if (in >> lbl{ "key" } && in >> key)
      {
        if (!used_keys[0] && key == '1')
        {
          used_keys[0] = true;
          in >> dbl{ input.key1 };
        }
        else if (!used_keys[1] && key == '2')
        {
          used_keys[1] = true;
          in >> lli{ input.key2 };
        }
        else if (!used_keys[2] && key == '3')
        {
          used_keys[2] = true;
          in >> str{ input.key3 };
        }
        else
        {
          in.setstate(std::ios_base::failbit);
        }
      }
      in >> sep{ ':' };
    }
    in >> sep{ ')' };
  }
  if (in)
  {
    data = input;
  }
  return in;
}

std::ostream & petrov::operator<<(std::ostream & out, const DataStruct & src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard outguard(out);
  out << "(:";
  out << "key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
  out << ":key2 " << src.key2 << "ll";
  out << ":key3 " << "\"" << src.key3 << "\"";
  out << ":)";
  return out;
}
