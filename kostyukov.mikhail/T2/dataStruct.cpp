#include "dataStruct.hpp"
#include "scopeGuard.hpp"
#include <iostream>
#include <string>
#include <bitset>
#include <iomanip>
#include <limits>
#include <cctype>

std::istream& kostyukov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (std::tolower(c) != std::tolower(dest.expected)))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kostyukov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  ScopeGuard scopeGrd(in);
  in >> DelimiterIO{ '"' };
  std::getline(in, dest.ref, '"');
  in >> DelimiterIO{ ':' };
  return in;
}

bool readUll(std::istream& in, unsigned long long& value, size_t base)
{
  std::string digits;
  std::getline(in, digits, ':');
  if (digits.empty())
  {
    return false;
  }
  try
  {
    size_t processedChars = 0;
    unsigned long long tempValue = std::stoull(digits, &processedChars, base);
    if (processedChars != digits.length())
    {
      return false;
    }
    value = tempValue;
    return true;
  }
  catch(const std::exception&)
  {
    return false;
  }
}

std::istream& kostyukov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  ScopeGuard scopeGrd(in);
  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
  DataStruct temp{};
  const size_t EXPECTED_KEY_COUNT = 3;
  const size_t KEY_NAME_LENGTH = 4;
  for (size_t i = 0; i < EXPECTED_KEY_COUNT; ++i)
  {
    std::string key = "";
    in >> key;
    if (!in || key.length() != KEY_NAME_LENGTH || key.substr(0,3) != "key")
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    char keyNum = key.back();
    bool readSuccess = false;
    char p1 = '\0',
         p2 = '\0';
    in >> p1;
    if (keyNum == '1' && p1 == '0')
    {
      in >> p2;
      if (in && std::tolower(p2) == 'b')
      {
        readSuccess = readUll(in, temp.key1, 2);
      }
    }
    else if (keyNum == '2' && p1 == '0')
    {
      in >> p2;
      if (in && std::tolower(p2) == 'x')
      {
        readSuccess = readUll(in, temp.key2, 16);
      }
    }
    else if (keyNum == '3')
    {
      if (p1 == '"')
      {
        in.putback(p1);
        in >> StringIO{ temp.key3 };
        readSuccess = static_cast< bool >(in);
      }
    }
    else
    {
      readSuccess = false;
    }
    if (!readSuccess)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  in >> DelimiterIO{ ')' };
  if (in)
  {
    dest = temp;
  }
  return in;
}

std::ostream& kostyukov::operator<<(std::ostream& out, BinUllIO&& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  ScopeGuard scopeGrd(out);
  out << "0b";
  if (dest.value == 0)
  {
    out << '0';
    return out;
  }
  const size_t ULL_BIT_COUNT = std::numeric_limits< unsigned long long >::digits;
  std::bitset< ULL_BIT_COUNT > bits(dest.value);
  bool leadingZeros = true;
  if (dest.value == 1)
  {
    out << "01";
    return out;
  }
  for (size_t i = 0; i < ULL_BIT_COUNT; ++i)
  {
    size_t bit_index = ULL_BIT_COUNT - 1 - i;
    if (bits[bit_index])
    {
      leadingZeros = false;
      out << '1';
    }
    else if (!leadingZeros)
    {
      out << '0';
    }
  }
    return out;
}

std::ostream& kostyukov::operator<<(std::ostream& out, HexUllIO&& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  ScopeGuard scopeGrd(out);
  out << "0x" << std::hex << std::uppercase << dest.value;
  return out;
}

std::ostream& kostyukov::operator<<(std::ostream& out, ConstStringIO&& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  ScopeGuard scopeGrd(out);
  return out << '"' << dest.ref << '"';
}

std::ostream& kostyukov::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  ScopeGuard scopeGrd(out);
  out << "(:key1 " << BinUllIO{ dest.key1 }
      << ":key2 " << HexUllIO{ dest.key2 }
      << ":key3 " << ConstStringIO{ dest.key3 }
      << ":)";
  return out;
}

bool kostyukov::DataStruct::operator<(const DataStruct& rhs) const
{
  if (key1 != rhs.key1)
  {
    return key1 < rhs.key1;
  }
  else if (key2 != rhs.key2)
  {
    return key2 < rhs.key2;
  }
  return key3.length() < rhs.key3.length();
}
