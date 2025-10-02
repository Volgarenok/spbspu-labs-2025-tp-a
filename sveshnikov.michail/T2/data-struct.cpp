#include "data-struct.hpp"
#include <iostream>
#include "stream-guard.hpp"

namespace
{
  struct DelimiterIO
  {
    char exp;
  };

  struct SllIO
  {
    long long &ref;
  };

  struct UllIO
  {
    unsigned long long &ref;
  };

  struct StringIO
  {
    std::string &ref;
  };

  struct LabelIO
  {
    std::string &exp;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
  std::istream &operator>>(std::istream &in, LabelIO &&dest);

  std::istream &operator>>(std::istream &in, SllIO &&dest);
  std::istream &operator>>(std::istream &in, UllIO &&dest);

  template< class T >
  std::istream &ll_io_impl(std::istream &in, T &ref, std::string &str1, std::string &str2)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> ref;
    std::string data = "";
    if (std::getline(in, data, ':') && data != str1 && data != str2)
    {
      in.setstate(std::ios::failbit);
    }
    in.putback(':');
    return in;
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

  std::istream &operator>>(std::istream &in, StringIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
  }

  std::istream &operator>>(std::istream &in, LabelIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in, dest.exp, ' ');
  }

  std::istream &operator>>(std::istream &in, SllIO &&dest)
  {
    std::string small_ll = "ll";
    std::string big_ll = "LL";
    return ll_io_impl(in, dest.ref, small_ll, big_ll);
  }

  std::istream &operator>>(std::istream &in, UllIO &&dest)
  {
    std::string small_ull = "ull";
    std::string big_ull = "ULL";
    return ll_io_impl(in, dest.ref, small_ull, big_ull);
  }
}

std::istream &sveshnikov::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  using sep = DelimiterIO;
  using label = LabelIO;
  using sll = SllIO;
  using ull = UllIO;
  using str = StringIO;

  in >> std::noskipws >> sep{'('} >> sep{':'};
  int keys[3] = {};
  for (size_t i = 0; i < 3; i++)
  {
    std::string key;
    in >> label{key};
    if (key == "key1" && keys[0] == 0)
    {
      in >> sll{input.key1};
      keys[0] = 1;
    }
    else if (key == "key2" && keys[1] == 0)
    {
      in >> ull{input.key2};
      keys[1] = 1;
    }
    else if (key == "key3" && keys[2] == 0)
    {
      in >> str{input.key3};
      keys[2] = 1;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    in >> sep{':'};
  }
  in >> sep{')'} >> std::skipws;

  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream &sveshnikov::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  StreamGuard streamguard(out);
  out << "(";
  out << ":key1 " << src.key1 << "ll";
  out << ":key2 " << src.key2 << "ull";
  out << ":key3 " << "\"" << src.key3 << "\"";
  out << ":)";
  return out;
}

bool sveshnikov::DataStruct::operator<(const DataStruct &src) const
{
  if (key1 != src.key1)
  {
    return key1 < src.key1;
  }
  else if (key2 != src.key2)
  {
    return key2 < src.key2;
  }
  return key3.size() < src.key3.size();
}
