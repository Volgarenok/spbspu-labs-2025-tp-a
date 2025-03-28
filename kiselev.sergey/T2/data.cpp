#include "data.hpp"
#include <cctype>
#include <exception>
#include <vector>
#include "scopeGuard.hpp"

std::istream& kiselev::operator>>(std::istream& input, DelimeterIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  char c = '0';
  input >> c;
  if (input && (std::tolower(c) != dest.exp))
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& kiselev::operator>>(std::istream& input, DelimetersIO&& dest)
{
  for (char c: dest.exp)
  {
    input >> DelimeterIO{ c };
  }
  return input;
}

std::istream& kiselev::operator>>(std::istream& input, UllIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::string number;
  std::getline(input >> DelimeterIO{ '0' }, number, ':');
  if (number.empty())
  {
    input.setstate(std::ios::failbit);
    return input;
  }
  try
  {
    dest.ref = std::stoull(number, nullptr, 8);
  }
  catch (const std::exception&)
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& kiselev::operator>>(std::istream& input, CharIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  return input >> DelimeterIO{ '\'' } >> dest.ref >> DelimetersIO{ "\':" };
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

std::istream& kiselev::operator>>(std::istream& input, KeyIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  input >> DelimetersIO{ "key" };
  int key;
  input >> key;
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
  {
    input >> DelimetersIO{ "(:" };
    input >> KeyIO{ temp };
    input >> KeyIO{ temp };
    input >> KeyIO{ temp };
    input >> DelimetersIO{ ")" };
  }
  if (input)
  {
    dest = temp;
  }
  return input;
}

std::ostream& kiselev::operator<<(std::ostream& output, const UllIO&& dest)
{
  unsigned long long number = dest.ref;
  if (dest.ref == 0)
  {
    return output << "00";
  }
  std::vector< unsigned > octalNum;
  while (number > 0)
  {
    octalNum.push_back(number % 8);
    number /= 8;
  }
  output << "0";
  for (auto it = octalNum.rbegin(); it != octalNum.rend(); ++it)
  {
    output << *it;
  }
  return output;
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
