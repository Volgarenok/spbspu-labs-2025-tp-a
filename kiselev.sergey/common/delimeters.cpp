#include "delimeters.hpp"
#include <algorithm>
#include <istream>
#include <iterator>
#include <ostream>
#include <string>
namespace
{
  struct DelimeterChecker
  {
    std::istream& input;
    bool operator()(char exp) const
    {
      return static_cast< bool >(input >> kiselev::DelimeterIO{ exp });
    }
  };
}
std::istream& kiselev::operator>>(std::istream& input, DelimeterIO&& dest)
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

std::istream& kiselev::operator>>(std::istream& input, DelimetersIO&& dest)
{
  DelimeterChecker checker{ input };
  if (!std::all_of(dest.exp.begin(), dest.exp.end(), checker))
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}
