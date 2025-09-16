#include "interim-input-utils.hpp"
#include <istream>

std::istream& kizhin::operator>>(std::istream& in, Delimiter&& dest)
{
  std::istream::sentry sentry(in);
  if (sentry && in.get() != dest.val) {
    in.unget();
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kizhin::operator>>(std::istream& in, OneOfDelimiters&& dest)
{
  std::istream::sentry sentry(in);
  if (sentry && dest.val.find(in.get()) == dest.val.npos) {
    in.unget();
    in.setstate(std::ios::failbit);
  }
  return in;
}

