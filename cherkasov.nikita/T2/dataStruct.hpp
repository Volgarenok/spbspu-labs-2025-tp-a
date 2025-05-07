#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <string>
#include <iostream>
#include <complex>
#include <utility>

namespace cherkasov
{
  struct DataStruct
  {
    std::complex< double > key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
    bool operator<(const DataStruct& other) const;
  };

  struct Complex
  {
    std::complex< double >& c;
  };

  struct Rational
  {
    std::pair< long long, unsigned long long >& rat;
  };

  struct Strings
  {
    std::string& s;
  };

  struct ExpectChar
  {
    char exp;
  };

  struct Label
  {
    std::string exp;
  };

  std::istream& operator>>(std::istream& in, ExpectChar&&);
  std::istream& operator>>(std::istream& in, Label&&);
  std::istream& operator>>(std::istream& in, Complex&&);
  std::istream& operator>>(std::istream& in, Rational&&);
  std::istream& operator>>(std::istream& in, Strings&&);
  std::istream& operator>>(std::istream& in, DataStruct&);
  std::ostream& operator<<(std::ostream& out, const DataStruct&);
}
#endif
