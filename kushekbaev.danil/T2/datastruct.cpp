#include "datastruct.hpp"
#include <iostream>

bool kushekbaev::DataStruct::operator<(const DataStruct& other) const
{
  if (key1 == other.key1)
  {
    if (key2 == other.key2)
    {
      return key3.size() < other.key3.size();
    }
    return key2 < other.key2;
  }
  return key1 < other.key1;
}

std::istream& kushekbaev::operator>>(std::istream& in, DelimiterIO&& obj)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = 0;
  in >> c;
  if (in && std::tolower(c) != obj.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kushekbaev::operator>>(std::istream& in, ULLBinaryIO&& obj)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{ '0' } >> DelimiterIO{ 'b' } >> obj.ref;
  if (in)
  {
    unsigned long long tmp = obj.ref;
    while (tmp > 0)
    {
      if (tmp % 10 >= 2)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      tmp /= 10;
    }
  }
  return in;
}

std::istream& kushekbaev::operator>>(std::istream& in, ChrLitIO&& obj)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{ '\'' };
  in >> obj.ref;
  in >> DelimiterIO{ '\'' };
  return in;
}

std::istream& kushekbaev::operator>>(std::istream& in, StringIO&& obj)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{ '\"' };
  std::getline(in, obj.ref, '\"');
  return in;
}

std::istream& kushekbaev::operator>>(std::istream& in, DataStruct& obj)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
  DataStruct tmp;
  try
  {
    in >> DelimiterIO{ '(' };
    while ((!hasKey1 || !hasKey2 || !hasKey3) && in)
    {
      std::string label;
      in >> label;
      if (label == ":key1" && !hasKey1)
      {
        in >> ULLBinaryIO{ tmp.key1 };
        hasKey1 = true;
      }
      else if (label == ":key2" && !hasKey2)
      {
        in >> ChrLitIO{ tmp.key2 };
        hasKey2 = true;
      }
      else if (label == ":key3" && !hasKey3)
      {
        in >> StringIO{ tmp.key3 };
        hasKey3 = true;
      }
      else
      {
        throw std::runtime_error("Unsupported key");
      }
    }
    in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
    if (!hasKey1 || !hasKey2 || !hasKey3)
    {
      throw std::runtime_error("Not enough keys");
    }
  }
  catch (...)
  {
    in.setstate(std::ios::failbit);
  }
  if (in)
  {
    obj = tmp;
  }
  return in;
}

std::ostream& kushekbaev::operator<<(std:ostream& out, const ULLBinaryIO& obj)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  else
  {
    out << "0b";
    for (std::size_t i = 0; i < obj.leadingZeroes; ++i)
    {
      out << "0";
    }
    out << obj.ref;
  }
}

std::ostream& kushekbaev::operator<<(std::ostream& out, const DataStruct& obj)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << "(:";
  out << "key1 " << ULLBinaryIO{ obj, leadingZeroes };
  out << ":";
  out << "key2 " << "\'" << obj.key2 << "\'";
  out << ":";
  out << "key3 " << "\"" << obj.key3 << "\"";
  out << ":)";
  return out;
}