#include "delimeters.hpp"
#include <algorithm>
#include <istream>
#include <iterator>
#include <ostream>
#include <string>
namespace
{
  struct DelimeterCheck
  {
    std::istream& input;
    bool operator()(char exp) const
    {
      return static_cast< bool >(input >> brevnov::DelimeterIO{ exp });
    }
  };
}
std::istream& brevnov::operator>>(std::istream& input, DelimeterIO&& dest)
{
  std::istream::sentry sent(input);
  if (!sent)
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

std::istream& brevnov::operator>>(std::istream& input, DelimetersIO&& dest)
{
  DelimeterCheck check{ input };
  if (!std::all_of(dest.exp.begin(), dest.exp.end(), check))
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}
