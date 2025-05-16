#include "DataStruct.h"
namespace nspace
{
  bool comparator(DataStruct &left, DataStruct &right)
  {
    if (left.key1 == right.key1 && left.key2 == right.key2)
    {
      return left.key3 < right.key3;
    }
    if (left.key1 == right.key1)
    {
      return left.key2 < right.key2;
    }
    return left.key1 < right.key1;
  }

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest)
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

  std::istream &operator>>(std::istream &in, DoubleIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    in >> dest.ref;
    in >> c;
    if (!(c == 'd' || c == 'D'))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, StringIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{'"'};
    return std::getline(in, dest.ref, '"');
  }

  std::istream &operator>>(std::istream &in, UnsignedLongLongIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{'0'};
    in >> dest.ref;
    if (in && ((std::to_string(dest.ref).find('8')) != std::string::npos ||
               std::to_string(dest.ref).find('9') != std::string::npos))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, DataStruct &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    DataStruct input;
    {
      in >> DelimiterIO{'('};
      bool flagKey1 = true;
      bool flagKey3 = true;
      bool flagKey2 = true;
      std::string label = "";

      while (flagKey1 || flagKey2 || flagKey3)
      {
        if (!std::cin)
        {
          break;
        }
        if (std::cin.eof())
        {
          in.setstate(std::ios::failbit);
          break;
        }
        in >> label;
        if (label == ":key1")
        {
          in >> DoubleIO{input.key1};
          flagKey1 = false;
        }
        else if (label == ":key2")
        {
          in >> UnsignedLongLongIO{input.key2};
          flagKey2 = false;
        }
        else if (label == ":key3")
        {
          in >> StringIO{input.key3};
          flagKey3 = false;
        }
        else
        {
          in.setstate(std::ios::failbit);
          break;
        }
      }
      in >> DelimiterIO{':'} >> DelimiterIO{')'};
    }
    if (in)
    {
      dest = input;
    }
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const DataStruct &src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard fmtguard(out);
    out << "(";
    out << ":key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
    out << ":key2 " << "0" << src.key2;
    out << ":key3 \"" << src.key3 << "\"";
    out << ":)";
    return out;
  }

  StreamGuard::StreamGuard(std::basic_ios<char> &s):
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  StreamGuard::~StreamGuard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}
