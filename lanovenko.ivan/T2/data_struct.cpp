#include "data_struct.hpp"
#include "scope_guard.hpp"

namespace
{
  struct DelimetrIO
  {
    char exp;
  };

  struct CharIO
  {
    char& ref;
  };

  struct RationalIO
  {
    std::pair< long long, unsigned long long >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimetrIO&& dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    char c = 0;
    in >> c;
    if (in && (c != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, CharIO&& dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> DelimetrIO{ '\'' } >> dest.ref >> DelimetrIO{ '\'' } >> DelimetrIO{ ':' };
    return in;
  }

  std::istream& operator>>(std::istream& in, RationalIO&& dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> DelimetrIO{ '(' } >> DelimetrIO{ ':' } >> DelimetrIO{ 'N' } >> dest.ref.first;
    in >> DelimetrIO{ ':' } >> DelimetrIO{ 'D' } >> dest.ref.second >> DelimetrIO{ ':' };
    in >> DelimetrIO{ ')' } >> DelimetrIO{ ':' };
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    return std::getline(in >> DelimetrIO{ '"' }, dest.ref, '"');
  }
}

std::istream& lanovenko::operator>>(std::istream &in, lanovenko::DataStruct &dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  lanovenko::DataStruct input{};
  {
    using sep = DelimetrIO;
    using chr = CharIO;
    using rtn = RationalIO;
    using str = StringIO;
    in >> sep{ '(' } >> sep{ ':' };
    for (size_t i = 0; i < 3; i++)
    {
      std::string temp = "";
      in >> temp;
      if (temp == "key1")
      {
        in >> chr{ input.key1 };
      }
      else if (temp == "key2")
      {
        in >> rtn{ input.key2 };
      }
      else if (temp == "key3")
      {
        in >> str{ input.key3 } >> sep{ ':' };
      }
    }
    in >> sep{ ')' };
  }
  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream& lanovenko::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << "(:" << "key1 " << "'" << dest.key1 << "'" << ":";
  out << "key2 " << "(:" << "N " << dest.key2.first << ":" << "D " << dest.key2.second << ":):";
  out << "key3 " << '"' << dest.key3 << '"' << ":)";
  return out;
}
