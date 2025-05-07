#include "DataStruct.h"

namespace orlova
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
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

  std::istream& operator>>(std::istream& in, LongLongIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> dest.ref;
    if (in)
    {
      std::string suffix;
      std::getline(in, suffix, ':');
      in.putback(':');
      if (suffix != "ll" && suffix != "LL")
      {
        in.setstate(std::ios::failbit);
      }
      else
      {
        dest.suffix = suffix;
      }
     }
    return in;
  }

  std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    iofmtguard i(in);
    in >> DelimiterIO{ '0' } >> std::oct >> dest.ref;
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, LabelIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
       return in;
    }
    char c;
    for (size_t i = 0; i < dest.exp.size(); i++)
    {
      in >> c;
      if (c != dest.exp[i])
      {
        in.setstate(std::ios::failbit);
        break;
      }
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    DataStruct input;
    {
      using sep = DelimiterIO;
      using ll = LongLongIO;
      using ull = UnsignedLongLongIO;
      using str = StringIO;
      bool key1 = false, key2 = false, key3 = false;
      in >> sep{ '(' };
      while ((key1 == false || key2 == false || key3 == false) && in)
      {
        std::string keynumber;
        in >> keynumber;
        if (key1 == false && keynumber == ":key1")
        {
          in >> ll{ input.key1 };
          key1 = true;
        }
        else if (key2 == false && keynumber == ":key2")
        {
          in >> ull{ input.key2 };
          key2 = true;
        }
        else if (key3 == false && keynumber == ":key3")
        {
          in >> str{ input.key3 };
          key3 = true;
        }
        else
        {
          in.setstate(std::ios::failbit);
        }
      }
      in >> sep{ ':' } >> sep{ ')' };
    }
    if (in)
    {
      dest = input;
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    iofmtguard fmtguard(out);
    out << "(:";
    out << "key1 " << src.key1 << "ll" << ":";
    out << "key2 " << "0" << std::oct << src.key2 << ":";
    out << "key3 " << "\"" << src.key3 << "\"";
    out << ":)";
    return out;
  }

  iofmtguard::iofmtguard(std::basic_ios< char >& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  iofmtguard::~iofmtguard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}
