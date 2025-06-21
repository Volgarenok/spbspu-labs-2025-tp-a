#include "DataStruct.h"
#include <iostream>
#include <string>
#include <limits>

std::string fedorova::changeKeyToBinary(unsigned long long key)
{
  std::string result;

  if (key == 0)
  {
    return "0b0";
  }

  for (int i = 63; i >= 0; --i)
  {
    result += ((key >> i) & 1) ? '1' : '0';
  }

  size_t firstIndex = result.find('1');
  if (firstIndex == std::string::npos)
  {
    return "0b0";
  }

  result = result.substr(firstIndex);

  if (result.length() == 1)
  {
    result = "0" + result;
  }

  return "0b" + result;
}

std::istream& fedorova::operator>>(std::istream& in, DelimiterIO&& data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '0';
  in >> c;
  c = std::tolower(c);

  if (in && (c != data.exp))
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& fedorova::operator>>(std::istream& is, ULLLiteralIO&& dest)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }
  is >> dest.ref;
  if (is)
  {
    is >> DelimiterIO{ 'u' } >> DelimiterIO{ 'l' } >> DelimiterIO{ 'l' } >> DelimiterIO{ ':' };
  }
  return is;
}

std::istream& fedorova::operator>>(std::istream& is, ULLBinaryIO&& data)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  char c = 0;
  unsigned long long number = 0;
  is >> DelimiterIO{ '0' } >> c;

  if (c != 'b' && c != 'B')
  {
    is.setstate(std::ios::failbit);
    return is;
  }

  while (is >> c)
  {
    if (c == '1' || c == '0')
    {
      number = (number << 1) + (c - '0');
    }
    else
    {
      break;
    }
  }

  if (c == ':')
  {
    data.ref = number;
  }
  else
  {
    is.setstate(std::ios::failbit);
  }

  return is;
}

std::istream& fedorova::operator>>(std::istream& is, StringIO&& dest)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  std::getline(is >> DelimiterIO{ '"' }, dest.ref, '"');
  if (is.fail())
  {
    is.setstate(std::ios::failbit);
  }

  is >> DelimiterIO{ ':' };

  return is;
}

std::istream& fedorova::operator>>(std::istream& is, LabelIO&& dest)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  for (char exp_char : dest.exp)
  {
    char c;
    is >> c;
    if (c != exp_char && !is)
    {
      is.setstate(std::ios::failbit);
      break;
    }
  }
  return is;
}

std::istream& fedorova::operator>>(std::istream& is, DataStruct& data)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  DataStruct temp;

  is >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

  for (size_t i = 0; i < 3; i++)
  {
    std::string key;
    is >> key;
    if (key == "key1")
    {
      is >> ULLLiteralIO{ temp.key1 };
    }
    else if (key == "key2")
    {
      is >> ULLBinaryIO{ temp.key2 };
    }
    else if (key == "key3")
    {
      is >> StringIO{ temp.key3 };
    }
    else
    {
      is.setstate(std::ios::failbit);
      return is;
    }
  }

  is >> DelimiterIO{ ')' };

  if (is)
  {
    data = temp;
  }

  return is;
}

std::ostream& fedorova::operator<<(std::ostream& out, const fedorova::DataStruct& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  IoGuard fmtguard(out);
  out << "(:key1 " << data.key1 << "ull:"
      << "key2 " << changeKeyToBinary(data.key2) << ":"
      << "key3 \"" << data.key3 << "\":)";
  return out;
}

fedorova::IoGuard::IoGuard(std::basic_ios< char >& s) :
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

fedorova::IoGuard::~IoGuard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

bool fedorova::compareDataStruct(const DataStruct& a, const DataStruct& b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
  }
  return a.key3.length() < b.key3.length();
}
