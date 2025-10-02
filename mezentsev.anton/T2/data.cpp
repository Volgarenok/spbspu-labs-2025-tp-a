#include "data.h"
#include <array>
#include <cctype>
#include <Guard.h>
#include <Delimiter.h>

std::istream& mezentsev::operator>>(std::istream& input, DelimitersIO&& dest)
{
  for (char c: dest.exp)
  {
    input >> DelimiterIO{ c };
  }
  return input;
}

std::istream& mezentsev::operator>>(std::istream& input, CharIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  return input >> DelimiterIO{ '\'' } >> dest.def >> DelimitersIO{ "\':" };
}

std::istream& mezentsev::operator>>(std::istream& input, StringIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::getline(input >> DelimiterIO{ '"' }, dest.def, '"');
  return input >> DelimiterIO{ ':' };
}


std::istream& mezentsev::operator>>(std::istream& in, LongLongIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.def >> mezentsev::DelimitersIO{ "ll:" };
  return in;
}

std::istream& mezentsev::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Guard scope(in);
  DataStruct temp;
  using del = DelimitersIO;
  in >> del{ "(:" };
  std::array< bool, 4 > usedKeys = { false };
  for (int i = 0; i < 3; i++)
  {
    in >> del{ "key" };
    int a;
    in >> a;
    if (a < 1 || a > 3 || usedKeys[a])
    {
      in.setstate(std::ios::failbit);
      break;
    }
    usedKeys[a] = true;
    switch (a)
    {
    case 1:
      in >> LongLongIO{ temp.key1 };
      break;
    case 2:
      in >> CharIO{ temp.key2 };
      break;
    case 3:
      in >> StringIO{ temp.key3 };
      break;
    default:
      in.setstate(std::ios::failbit);
    }
  }
  in >> del{ ")" };
  if (in)
  {
    dest = temp;
  }
  return in;
}

std::ostream& mezentsev::operator<<(std::ostream& output, const CharIO&& dest)
{
  return output << "\'" << dest.def << "\'";
}

std::ostream& mezentsev::operator<<(std::ostream &output, const StringIO&& dest)
{
  return output << "\"" << dest.def << "\"";
}

std::ostream& mezentsev::operator<<(std::ostream &output, const LongLongIO&& dest)
{
  return output << dest.def << "ll";
}

std::ostream& mezentsev::operator<<(std::ostream& output, const DataStruct& dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry)
  {
    return output;
  }
  long long ll = dest.key1;
  char symbol = dest.key2;
  std::string string = dest.key3;
  Guard scope(output);
  output << "(:key1 " << LongLongIO{ ll };
  output << ":key2 " << CharIO{ symbol };
  output << ":key3 " << StringIO{ string } << ":)";
  return output;
}

bool mezentsev::compare(const DataStruct& lhs, const DataStruct& rhs)
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
