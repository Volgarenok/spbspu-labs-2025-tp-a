#include "DataStruct.h"

namespace fedorova
{
  std::istream& operator>>(std::istream& is, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    char c;
    is >> c;
    if (is && (c != dest.exp))
    {
      is.setstate(std::ios::failbit);
    }

    return is;
  }

  std::istream& operator>>(std::istream& is, ULLLiteralIO&& dest)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    is >> dest.ref;

    std::string suffix;
    std::getline(is, suffix, ':');
    is.putback(':');

    if (is && suffix != "ull" && suffix != "ULL")
    {
      is.setstate(std::ios::failbit);
    }
    else
    {
      dest.suffix = suffix;
    }

    return is;
  }

  std::istream& operator>>(std::istream& is, ULLBinaryIO&& dest) {
    std::istream::sentry s(is);
    if (!s) return is;

    is >> DelimiterIO{ '0' };
    char b = '0';
    is >> b;
    if (b != 'b' && b != 'B') {
      is.setstate(std::ios::failbit);
      dest.ref = "";
      return is;
    }

    std::string bits = "";
    char c = '0';
    while (is.get(c)) {
      if (c == '0' || c == '1') {
        bits += c;
      }
      else {
        is.unget();
        break;
      }
    }

    if (bits.empty()) {
      is.setstate(std::ios::failbit);
    }
    dest.ref = bits;

    return is;
  }

  std::istream& operator>>(std::istream& is, StringIO&& dest)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    return std::getline(is >> DelimiterIO{ '\"' }, dest.ref, '\"');
  }

  std::istream& operator>>(std::istream& is, LabelIO&& dest)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    char key;
    for (size_t i = 0; i < dest.exp.size(); i++)
    {
      is >> key;
      if (key != dest.exp[i])
      {
        is.setstate(std::ios::failbit);
        break;
      }
    }
    return is;
  }

  std::istream& operator>>(std::istream& is, DataStruct& data)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    DataStruct in;

    {
      bool key1 = true, key2 = true, key3 = true;

      is >> DelimiterIO{ '(' };

      while ((key1 || key2 || key3) && is)
      {
        std::string fieldName;
        is >> fieldName;
        if (fieldName == ":key1" && key1)
        {
          is >> ULLLiteralIO{ data.key1 };
          in.key1 = data.key1;
          key1 = false;
        }
        else if (fieldName == ":key2" && key2)
        {
          is >> ULLBinaryIO{ data.key2 };
          in.key2 = data.key2;
          key2 = false;
        }
        else if (fieldName == ":key3" && key3)
        {
          is >> StringIO{ data.key3 };
          in.key3 = data.key3;
          key3 = false;
        }
        else
        {
          is.setstate(std::ios::failbit);
          break;
        }
      }
    }

    if (is)
    {
      data = in;
    }

    is >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
    return is;
  }

  std::ostream& operator<<(std::ostream& os, const DataStruct& data)
  {
    std::ostream::sentry sentry(os);
    if (!sentry)
    {
      return os;
    }

    iofmtguard fmtguard(os);

    os << "(:"
      << "key1 " << data.key1 << "ull" << ":"
      << "key2 " << "0b" << data.key2 << ":"
      << "key3 " << "\"" << data.key3 << "\""
      << ":)";

    return os;
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
