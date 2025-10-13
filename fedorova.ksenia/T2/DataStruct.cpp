#include "DataStruct.h"
#include <iostream>
#include <string>
#include <limits>
#include <delimiter.h>
#include <io-guard.h>

std::ostream& fedorova::operator<<(std::ostream& os, const ULLBinaryO& dest)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
  {
    return os;
  }
  if (dest.ref != 0)
  {
    os << "0" << dest.ref;
  }
  else
  {
    os << dest.ref;
  }
  return os;
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
    is >> DelimiterIO{ 'u' } >> DelimiterIO{ 'l' } >> DelimiterIO{ 'l' };
  }
  return is;
}

std::istream& fedorova::operator>>(std::istream& is, ULLBinaryI&& data)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  is >> DelimiterIO{ '0' } >> DelimiterIO{ 'b' } >> data.ref;

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

  return is;
}

std::istream& fedorova::operator>>(std::istream& is, LabelIO&& dest)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  for (char exp_char:dest.exp)
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
  {
    bool key1 = false, key2 = false, key3 = false;
    is >> DelimiterIO{ '(' };
    while (is && (key1 == false || key2 == false || key3 == false))
    {
      std::string key;
      is >> key;
      if (key1 == false && key == ":key1")
      {
        is >> ULLLiteralIO{ temp.key1 };
        key1 = true;
      }
      else if (key2 == false && key == ":key2")
      {
        is >> ULLBinaryI{ temp.key2 };
        key2 = true;
      }
      else if (key3 == false && key == ":key3")
      {
        is >> StringIO{ temp.key3 };
        key3 = true;
      }
      else
      {
        is.setstate(std::ios::failbit);
      }
    }
    is >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
  }

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
  out << "(:key1 " << data.key1 << "ull:";
  out << "key2 " << "0b" << ULLBinaryO{ data.key2 } << ":";
  out << "key3 \"" << data.key3 << "\":)";
  return out;
}

/*fedorova::IoGuard::IoGuard(std::basic_ios< char >& s):
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
}*/

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
