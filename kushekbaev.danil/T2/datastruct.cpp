#include "datastruct.hpp"
#include <iostream>
#include <delimiter.hpp>

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

std::istream& kushekbaev::operator>>(std::istream& in, ULLBinaryI&& obj)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{ '0' } >> DelimiterIO{ 'b' } >> obj.ref;
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
        in >> ULLBinaryI{ tmp.key1 };
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

std::ostream& kushekbaev::operator<<(std::ostream& out, const ULLBinaryO&& obj)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  if (obj.ref != 0)
  {
    out << "0" << obj.ref;
  }
  else
  {
    out << obj.ref;
  }
  return out;
}

std::ostream& kushekbaev::operator<<(std::ostream& out, const DataStruct& obj)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << "(:";
  out << "key1 " << "0b" << ULLBinaryO{ obj.key1 };
  out << ":";
  out << "key2 " << "\'" << obj.key2 << "\'";
  out << ":";
  out << "key3 " << "\"" << obj.key3 << "\"";
  out << ":)";
  return out;
}
