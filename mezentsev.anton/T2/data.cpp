#include "data.h"
#include <cctype>
#include "guard.h"

std::istream& mezentsev::operator>>(std::istream& input, DelimiterIO&& dest)
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

std::istream& mezentsev::operator>>(std::istream& in, Data& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Guard scope(in);
  Data temp;
  using del = DelimitersIO;
  in >> del{ "(:" };
  for (int i = 0; i < 3; i++)
  {
    in >> del{ "key" };
    int a;
    in >> a;
    switch(a)
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

std::ostream& mezentsev::operator<<(std::ostream& output, const Data& dest)
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

bool mezentsev::compare(const Data& lhs, const Data& rhs)
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
