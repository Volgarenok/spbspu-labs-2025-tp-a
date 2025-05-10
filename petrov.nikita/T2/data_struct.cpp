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

namespace petrov
{
  std::istream & operator>>(std::istream & in, DelimiterIO && dest)
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

  std::istream & operator>>(std::istream & in, DoubleIO && dest)
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

  std::istream & operator>>(std::istream & in, LongLongIO && dest)
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

  std::istream & operator>>(std::istream & in, StringIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }

  std::istream & operator>>(std::istream & in, DataStruct & data)
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
      using dbl = DoubleIO;
      using lli = LongLongIO;
      using str = StringIO;
      in >> sep{ '(' };
      in >> sep{ ':' };
      for (size_t i = 1; i <= 3; i++)
      {
        in >> key;
        if (key == "key1")
        {
          in >> dbl{ input.key1 };
          in >> sep{ ':' };
        }
        else if (key == "key2")
        {
          in >> lli{ input.key2 };
          in >> sep{ ':' };
        }
        else if (key == "key3")
        {
          in >> str{ input.key3 };
          in >> sep{ ':' };
        }
        else
        {
          in.setstate(std::ios::failbit);
        }
      }
      in >> sep{ ')' };
    }
    if (in)
    {
      data = input;
    }
    return in;
  }

  std::ostream & operator<<(std::ostream & out, const DataStruct & src)
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
}
