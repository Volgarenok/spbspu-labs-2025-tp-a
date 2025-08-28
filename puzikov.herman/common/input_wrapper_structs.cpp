#include "input_wrapper_structs.hpp"
#include <istream>

std::istream &puzikov::input::operator>>(std::istream &in, Character &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c;
  in.get(c);
  if (in && std::tolower(c) != std::tolower(dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &puzikov::input::operator>>(std::istream &in, ULLValue &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  {
    char prefix[2];
    in.get(prefix[0]).get(prefix[1]);
    if (in && (prefix[0] != '0' || std::tolower(prefix[1]) != 'b'))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  unsigned long long num = 0;
  char c = 0;
  while (in.get(c) && c != ':')
  {
    if (c != '0' && c != '1')
    {
      in.setstate(std::ios::failbit);
      break;
    }
    num = (num << 1) | (c - '0');
  }
  dest.ref = num;
  return in;
}

std::istream &puzikov::input::operator>>(std::istream &in, PairValue &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  long long n;
  unsigned long long d;
  in >> Character {'('} >> Character {':'} >> Character {'N'} >> n;
  in >> Character {':'} >> Character {'D'} >> d;
  in >> Character {':'} >> Character {')'} >> Character {':'};

  if (in)
  {
    dest.ref = {n, d};
  }
  return in;
}

std::istream &puzikov::input::operator>>(std::istream &in, StringValue &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> Character {'"'};
  std::getline(in, dest.ref, '"');
  in >> Character {':'};
  return in;
}
