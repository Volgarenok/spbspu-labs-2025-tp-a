#include "delimetr.h"
#include <algorithm>
#include <istream>
#include <iterator>
#include <ostream>
#include <string>

std::istream& kharlamov::operator>>(std::istream& input, DelimeterIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  char c;
  input >> c;
  if (input && (c != dest.exp))
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}
