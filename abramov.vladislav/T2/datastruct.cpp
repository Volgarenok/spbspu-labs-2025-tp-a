#include "datastruct.hpp"
#include <iomanip>
#include "stream_guard.hpp"

std::ostream &abramov::operator<<(std::ostream &out, const DataStruct &data)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  StreamGuard guard(out);
  out << "(";
  out << ":key1 " << std::fixed << std::setprecision(1) << data.key1 << "d";
  out << ":key2 " << data.key2 << "ull";
  out << ":key3 " << "\"" << data.key3 << "\"";
  out << ":)";
  return out;
}

std::istream &abramov::operator>>(std::istream &in, DoubleIO &&dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> dest.ref >> DelimiterIO{ 'd' };
  return in;
}

std::istream &abramov::operator>>(std::istream &in, UllIO &&dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> dest.ref >> DelimiterIO{ 'u' } >> DelimiterIO{ 'l' } >> DelimiterIO{ 'l' };
  return in;
}
std::istream &abramov::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> DelimiterIO{ '"' };
  return std::getline(in, dest.ref, '"');
}

std::istream &abramov::operator>>(std::istream &in, DataStruct &data)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  DataStruct input;
  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
  std::string key;
  constexpr size_t key_numbers = 3;
  size_t count = 0;
  while (count < key_numbers && in)
  {
    in >> key;
    if (key == "key1")
    {
      in >> DoubleIO{ input.key1 };
      in >> DelimiterIO{ ':' };
      ++count;
    }
    else if (key == "key2")
    {
      in >> UllIO{ input.key2 };
      in >> DelimiterIO{ ':' };
      ++count;
    }
    else if (key == "key3")
    {
      in >> StringIO{ input.key3 };
      in >> DelimiterIO{ ':' };
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

bool abramov::operator<(const DataStruct &lhs, const DataStruct &rhs)
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
