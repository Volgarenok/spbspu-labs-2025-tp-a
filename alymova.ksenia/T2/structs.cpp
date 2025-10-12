#include "structs.hpp"
#include <iostream>
#include <stream-guard.hpp>
#include <delimiter-io.hpp>

std::istream& alymova::operator>>(std::istream& in, LabelIO&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c;
  for (size_t i = 0; i < object.exp.size(); i++)
  {
    in >> c;
    if (c != object.exp[i])
    {
      in.setstate(std::ios::failbit);
      break;
    }
  }
  return in;
}

std::istream& alymova::operator>>(std::istream& in, UllOctIO&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  alymova::StreamGuard guard(in);
  in >> DelimiterIO{'0'} >> std::oct >> object.i_oct;
  return in;
}

std::istream& alymova::operator>>(std::istream& in, ChrLitIO&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> DelimiterIO{'\''} >> object.c >> DelimiterIO{'\''};
}

std::istream& alymova::operator>>(std::istream& in, StringIO&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  alymova::StreamGuard guard(in);
  in >> std::noskipws;

  in >> DelimiterIO{'"'};
  char next;
  while (in >> next && next != '"')
  {
    if (next == '\n')
    {
      in.setstate(std::ios_base::failbit);
      break;
    }
    object.s.push_back(next);
  }
  return in;
}

std::istream& alymova::operator>>(std::istream& in, DataStruct& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  bool flag1 = false;
  bool flag2 = false;
  bool flag3 = false;

  in >> DelimiterIO{'('};
  while ((!flag1 || !flag2 || !flag3) && in)
  {
    std::string name;
    in >> name;
    if (name == ":key1" && !flag1)
    {
      flag1 = true;
      in >> UllOctIO{input.key1};
    }
    else if (name == ":key2" && !flag2)
    {
      flag2 = true;
      in >> ChrLitIO{input.key2};
    }
    else if (name == ":key3" && !flag3)
    {
      flag3 = true;
      in >> StringIO{input.key3};
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }
  in >> DelimiterIO{':'};
  in >> DelimiterIO{')'};
  if (in)
  {
    object = input;
  }
  return in;
}

std::ostream& alymova::operator<<(std::ostream& out, const DataStruct& object)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  alymova::StreamGuard guard(out);
  out << "(";
  out << ":key1 ";
  out << '0' << std::oct << object.key1;
  out << ":key2 ";
  out << "'" << object.key2 << "'";
  out << ":key3 ";
  out << "\"" << object.key3 << "\"";
  out << ":)";
  return out;
}

bool alymova::DataStruct::operator<(const DataStruct& other)
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }
  if (key2 != other.key2)
  {
    return key2 < other.key2;
  }
  return key3.size() < other.key3.size();
}
