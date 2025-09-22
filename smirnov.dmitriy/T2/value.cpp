#include "value.hpp"
#include "delimiter.hpp"
#include <bitset>
#include <string>

std::istream &smirnov::operator>>(std::istream &in, ULLBinary &&key)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    in.setstate(std::ios::failbit);
  }
  std::bitset< 64 > bin;
  in >> DelimiterStringI{ "0b" } >> bin;
  if (in)
  {
    key.value = bin.to_ulong();
  }
  return in;
}

std::istream& smirnov::operator>>(std::istream& in, DBLLit&& key)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    in.setstate(std::ios::failbit);
  }

  return in >> key.value >> DelimiterCharI{ 'd' };
}

std::istream& smirnov::operator>>(std::istream& in, StrKey&& key)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    in.setstate(std::ios::failbit);
  }
  std::string tmp{};
  std::getline(in >> DelimiterCharI{ '"' }, tmp, '"');
  if (in)
  {
    key.str = tmp;
  }
  return in;
}
