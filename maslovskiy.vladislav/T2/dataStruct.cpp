#include "dataStruct.hpp"
#include <iomanip>
#include <cctype>
#include "streamGuard.hpp"

std::ostream& maslovskiy::operator<<(std::ostream& out, const DataStruct& data)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  StreamGuard guard(out);
  out << "(";
  out << ":key1 " << SllLitO{ data.key1 };
  out << ":key2 " << "0x" << std::hex << std::uppercase << data.key2;
  out << ":key3 " << "\"" << data.key3 << "\"";
  out << ":)";
  return out;
}
std::istream& maslovskiy::operator>>(std::istream& in, SllLitI&& dest)
{
  std::istream::sentry s(in);
  if (!s) return in;

  long long value;
  if (!(in >> value))
  {
    return in;
  }
  char c1 = '\0', c2 = '\0';
  if (!(in >> c1) || !(in >> c2) || std::tolower(static_cast<unsigned char>(c1)) != 'l' ||
      std::tolower(static_cast<unsigned char>(c2)) != 'l')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  dest.ref = value;
  return in;
}
std::ostream& maslovskiy::operator<<(std::ostream& out, const SllLitO&& dest)
{
  std::ostream::sentry s(out);
  if (!s) return out;
  return out << dest.ref << "ll";
}
std::istream& maslovskiy::operator>>(std::istream& in, UllHexIO&& dest)
{
  std::istream::sentry s(in);
  if (!s) return in;
  char c0 = '\0', c1 = '\0';
  if (!(in >> c0) || c0 != '0' || !(in >> c1) || std::tolower(static_cast<unsigned char>(c1)) != 'x')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> std::hex >> dest.ref;
  return in;
}
std::istream& maslovskiy::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry s(in);
  if (!s) return in;
  in >> DelimiterIO{ '"' };
  return std::getline(in, dest.ref, '"');
}
std::istream& maslovskiy::operator>>(std::istream& in, DataStruct& data)
{
  std::istream::sentry s(in);
  if (!s) return in;
  DataStruct input;
  in >> DelimiterIO{ '(' };
  if (!in) return in;
  while (in && in.peek() != ')')
  {
    in >> DelimiterIO{ ':' };
    if (!in) return in;
    if (in.peek() == ')')
    {
      in.get();
      break;
    }
    std::string key;
    in >> key;
    if (key == "key1")
    {
      if (input.key1Set)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> SllLitI{ input.key1 };
      input.key1Set = true;
    }
    else if (key == "key2")
    {
      if (input.key2Set)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> UllHexIO{ input.key2 };
      input.key2Set = true;
    }
    else if (key == "key3")
    {
      if (input.key3Set)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> StringIO{ input.key3 };
      input.key3Set = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  if (!input.key1Set || !input.key2Set || !input.key3Set)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (in)
  {
    data = input;
  }
  return in;
}
bool maslovskiy::operator<(const DataStruct& lhs, const DataStruct& rhs)
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
