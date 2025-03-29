#include "input_wrapper_structs.hpp"

#include <istream>

std::istream &puzikov::input::operator>>(std::istream &in, CharacterIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = 0;
  in.get(c);
  if (in && (std::tolower(c) != std::tolower(dest.exp)))
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

  long long n = 0;
  unsigned long long d = 0;
  in >> CharacterIO {'('} >> CharacterIO {':'} >> CharacterIO {'N'} >> n;
  in >> CharacterIO {':'} >> CharacterIO {'D'} >> d;
  in >> CharacterIO {':'} >> CharacterIO {')'} >> CharacterIO {':'};

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
  std::getline(in >> CharacterIO {'"'}, dest.ref, '"');
  in >> CharacterIO {':'};
  return in;
}
