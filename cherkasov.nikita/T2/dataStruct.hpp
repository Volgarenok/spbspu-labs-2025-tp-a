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

  std::istream& operator>>(std::istream& in, DelimiterIO&& obj);
  std::istream& operator>>(std::istream& in, ComplexIO&& obj);
  std::istream& operator>>(std::istream& in, RationalIO&& obj);
  std::istream& operator>>(std::istream& in, StringIO&& obj);
  std::istream& operator>>(std::istream& in, DataStruct& obj);
  std::ostream& operator<<(std::ostream& out, const DataStruct& obj);
}
#endif
