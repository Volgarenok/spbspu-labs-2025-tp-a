#include "dataStruct.hpp"
#include <iomanip>
#include "streamGuard.hpp"

std::ostream& krylov::operator<<(std::ostream& out, const DataStruct& data)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  StreamGuard guard(out);
  out << "(";
  out << ":key1 " << "0b" << UllBinO{ data.key1 };
  out << ":key2 " << "0x" << std::hex << std::uppercase << data.key2;
  out << ":key3 " << "\"" << data.key3 << "\"";
  out << ":)";
  return out;
}

std::istream& krylov::operator>>(std::istream& in, UllBinI&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> DelimiterIO{ '0' } >> DelimiterIO{ 'b' } >> dest.ref;
  return in;
}

std::ostream& krylov::operator<<(std::ostream& out, const UllBinO&& dest)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  if (dest.ref != 0)
  {
    out << "0" << dest.ref;
  }
  else
  {
    out << dest.ref;
  }
  return out;
}

std::istream& krylov::operator>>(std::istream& in, UllHexIO&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> DelimiterIO{ '0' } >> DelimiterIO{ 'x' } >> std::hex >> dest.ref;
  return in;
}

std::istream& krylov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> DelimiterIO{ '"' };
  return std::getline(in, dest.ref, '"');
}

std::istream& krylov::operator>>(std::istream& in, DataStruct& data)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  DataStruct input;
  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
  std::string key;
  constexpr size_t keysNumbers = 3;
  size_t count = 0;
  bool wasKey1 = false, wasKey2 = false, wasKey3 = false;
  while (count < keysNumbers && in)
  {
    in >> key;
    if (key == "key1" && !wasKey1)
    {
      in >> UllBinI{ input.key1 };
      in >> DelimiterIO{ ':' };
      wasKey1 = true;
      ++count;
    }
    else if (key == "key2" && !wasKey2)
    {
      in >> UllHexIO{ input.key2 };
      in >> DelimiterIO{ ':' };
      wasKey2 = true;
      ++count;
    }
    else if (key == "key3" && !wasKey3)
    {
      in >> StringIO{ input.key3 };
      in >> DelimiterIO{ ':' };
      wasKey3 = true;
      ++count;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }
  in >> DelimiterIO{ ')' };
  if (in)
  {
    data = input;
  }
  return in;
}

bool krylov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 == rhs.key1)
  {
    if (lhs.key2 == rhs.key2)
    {
      return lhs.key3.size() < rhs.key3.size();
    }
    else
    {
      return lhs.key2 < rhs.key2;
    }
  }
  else
  {
    return lhs.key1 < rhs.key1;
  }
}
