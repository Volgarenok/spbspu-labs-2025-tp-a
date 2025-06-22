#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <array>
#include <algorithm>
#include <complex>
#include <iostream>
#include <string>
namespace brevnov
{
  struct DataStruct
  {
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct DelimitersIO
  {
    std::string& exp;
  };

  struct UnLongLongIO
  {
    unsigned long long & ref;
  };

  struct ComplexIO
  {
    std::complex< double >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct KeyIO
  {
    DataStruct& data;
    std::array< int, 3 > keys;
  };

  struct StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char > & s);
    ~StreamGuard();
  private:
    std::basic_ios< char > & s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  bool compare(const DataStruct& lhs, const DataStruct& rhs);
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DelimitersIO&& dest);
  std::istream& operator>>(std::istream& in, ComplexIO&& dest);
  std::istream& operator>>(std::istream& in, UnLongLongIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, KeyIO&& dest);
  std::istream& operator>>(std::istream& input, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const UnLongLongIO&& src);
  std::ostream& operator<<(std::ostream& out, const ComplexIO&& src);
  std::ostream& operator<<(std::ostream& out, const StringIO&& src);
  std::ostream& operator<<(std::ostream& output, const DataStruct& dest);
}
#endif
