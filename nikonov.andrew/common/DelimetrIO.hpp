#ifndef DELIMETRIO_HPP
#define DELIMETRIO_HPP
#include "istream"
namespace nikonov
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}
#endif
