#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <complex>

namespace evstyunichev
{
  constexpr unsigned int UllKey = 2, CmplKey = 4, StrKey = 8;

  struct DataStruct
  {
    unsigned long long key1;
    std::complex< double > key2;
    std::string key3;
  };
  std::istream & operator>>(std::istream &in, DataStruct &data);
  std::ostream & operator<<(std::ostream &out, const DataStruct &data);

  bool comparator(const DataStruct &, const DataStruct &);

  struct DelimiterIO
  {
    char exp;
  };

  struct DelimitersIO
  {
    std::string exps;
  };

  struct UllIO
  {
    unsigned long long &ref;
  };

  struct CmplIO
  {
    std::complex< double > &cmpl;
  };

  struct StringIO
  {
    std::string &ref;
  };

  struct KeyIO
  {
    DataStruct &data;
    int done;
  };

  std::istream & operator>>(std::istream &in, KeyIO &key);
  std::istream & operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream & operator>>(std::istream &in, DelimitersIO &&dest);
  std::istream & operator>>(std::istream &in, UllIO &dest);
  std::istream & operator>>(std::istream &in, CmplIO &dest);
  std::istream & operator>>(std::istream &in, StringIO &dest);
  std::istream & operator>>(std::istream &in, DataStruct &dest);
  std::ostream & operator<<(std::ostream &out, UllIO &&dest);
  std::ostream & operator<<(std::ostream &out, StringIO &&dest);
  std::ostream & operator<<(std::ostream &out, CmplIO &&dest);
  std::ostream & operator<<(std::ostream &out, const DataStruct &dest);
  double abscmpl(const std::complex< double > &cmpl);
}

#endif
