#include "delimiter.hpp"

#include <input_delimiter.hpp>

namespace fedorov
{
  std::istream &operator>>(std::istream &in, DelimiterStr &&exp)
  {
    std::istream::sentry guard(in);
    if (!guard)
    {
      return in;
    }
    for (size_t i = 0; exp.exp[i] != '\0'; ++i)
    {
      in >> DelimiterInput{exp.exp[i]};
    }
    return in;
  }
}
