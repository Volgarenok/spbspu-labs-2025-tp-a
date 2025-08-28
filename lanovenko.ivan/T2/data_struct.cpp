#include "data_struct.hpp"

std::istream& lanovenko::operator>>(std::istream& in, CharIO&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> DelimiterIO{ '\'' } >> dest.ref >> DelimiterIO{ '\'' } >> DelimiterIO{':'};
  return in;
}

std::istream& lanovenko::operator>>(std::istream& in, RationalIO&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> DelimiterIO{ 'N' } >> dest.ref.first;
  in >> DelimiterIO{ ':' } >> DelimiterIO{ 'D' } >> dest.ref.second >> DelimiterIO{ ':' };
  in >> DelimiterIO{ ')' } >> DelimiterIO{ ':' };
  return in;
}

std::istream& lanovenko::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& lanovenko::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  DataStruct input{};
  {
    using sep = DelimiterIO;
    using chr = CharIO;
    using rtn = RationalIO;
    using str = StringIO;
    in >> sep{ '(' } >> sep{ ':' };
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    for (size_t i = 0; i < 3; i++)
    {
      std::string temp = "";
      in >> temp;
      if (temp == "key1" && !hasKey1)
      {
        in >> chr{ input.key1 };
        hasKey1 = true;
      }
      else if (temp == "key2" && !hasKey2)
      {
        in >> rtn{ input.key2 };
        hasKey2 = true;
      }
      else if (temp == "key3" && !hasKey3)
      {
        in >> str{ input.key3 } >> sep{ ':' };
        hasKey3 = true;
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
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

bool lanovenko::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}
