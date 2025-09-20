#ifndef INPUT_STRUCT_HPP
#define INPUT_STRUCT_HPP

#include <complex>
#include <string>
#include <cstring>

namespace nikitin
{
  struct DelimiterIO
  {
    char exp;
  };

  struct LongLongIO
  {
    long long& ref;
  };

  struct ComplexIO
  {
    std::complex< double >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, LongLongIO&& dest);
  std::istream& operator>>(std::istream& in, ComplexIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
}

#endif
