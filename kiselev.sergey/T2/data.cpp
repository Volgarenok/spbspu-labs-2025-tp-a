#include "data.hpp"
#include <algorithm>
#include <ios>
#include <iostream>
#include "scopeGuard.hpp"
#include "delimeters.hpp"

std::istream& kiselev::operator>>(std::istream& input, UllIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  return input >> std::oct >> dest.ref >> DelimeterIO{ ':' };
}

std::istream& kiselev::operator>>(std::istream& input, CharIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::string str = "\':";
  return input >> DelimeterIO{ '\'' } >> dest.ref >> DelimetersIO{ str };
}

std::istream& kiselev::operator>>(std::istream& input, StringIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::getline(input >> DelimeterIO{ '"' }, dest.ref, '"');
  return input >> DelimeterIO{ ':' };
}

std::istream& kiselev::operator>>(std::istream& input, KeyIO& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::string str = "key";
  input >> DelimetersIO{ str };
  int key;
  input >> key;
  if (std::find(dest.keys.begin(), dest.keys.end(), key) != dest.keys.end())
  {
    input.setstate(std::ios::failbit);
    return input;
  }
  auto it = std::find(dest.keys.begin(), dest.keys.end(), 0);
  if (it == dest.keys.end())
  {
    input.setstate(std::ios::failbit);
    return input;
  }
  *it = key;
  switch (key)
  {
  case 1:
    input >> UllIO{ dest.data.key1 };
    break;
  case 2:
    input >> CharIO{ dest.data.key2 };
    break;
  case 3:
    input >> StringIO{ dest.data.key3 };
    break;
  default:
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& kiselev::operator>>(std::istream& input, DataStruct& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  DataStruct temp;
  detail::ScopeGuard scope(input);
  {
    std::string str = "(:";
    input >> DelimetersIO{ str };
    KeyIO key{ temp, { 0, 0, 0 } };
    input >> key;
    input >> key;
    input >> key;
    input >> DelimeterIO{ ')' };
  }
  if (input)
  {
    dest = temp;
  }
  return input;
}

std::ostream& kiselev::operator<<(std::ostream& output, const UllIO&& dest)
{
  return output << '0' << std::oct << dest.ref;
}

std::ostream& kiselev::operator<<(std::ostream& output, const CharIO&& dest)
{
  return output << "\'" << dest.ref << "\'";
}

std::ostream& kiselev::operator<<(std::ostream &output, const StringIO&& dest)
{
  return output << "\"" << dest.ref << "\"";
}
std::ostream& kiselev::operator<<(std::ostream& output, const DataStruct& dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry)
  {
    return output;
  }
  unsigned long long ull = dest.key1;
  char symbol = dest.key2;
  std::string string = dest.key3;
  detail::ScopeGuard scope(output);
  output << "(:key1 " << UllIO{ ull };
  output << ":key2 " << CharIO{ symbol };
  output << ":key3 " << StringIO{ string } << ":)";
  return output;
}

bool kiselev::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  else if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}
