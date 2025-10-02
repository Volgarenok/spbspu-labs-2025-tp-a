#ifndef IO_HPP
#define IO_HPP

#include <complex>
#include <string>
#include <utility>

namespace amine
{
  struct DelimiterIO
  {
    char exp;
  };
  struct ComplexIO
  {
    std::complex< double >& ref;
  };
  struct RationalIO
  {
    std::pair< long long, unsigned long long >& ref;
  };
  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, ComplexIO&& dest);
  std::istream& operator>>(std::istream& in, RationalIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
}
#endif
