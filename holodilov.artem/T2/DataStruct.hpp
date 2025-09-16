#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <complex>
#include <string>

namespace holodilov
{

  struct DataStruct
  {
    unsigned long long key1;
    std::complex< double > key2;
    std::string key3;

    bool operator<(const DataStruct& other) const;
  };

  std::istream& operator>>(std::istream& in, DataStruct& dataStruct);

  std::ostream& operator<<(std::ostream& out, const DataStruct& dataStruct);

  namespace io
  {
    struct DelimIO
    {
      char delim;
    };

    struct UllOctIO
    {
      unsigned long long& ullOctRef;

      bool checkOctFormat() const;
    };

    struct ComplexIO
    {
      std::complex<  double  >& complexRef;
    };

    struct StringIO
    {
      std::string& strRef;
    };

    struct KeyNameIO
    {
    public:
      int getKeyNumber() const;

      void fillDataStructField(std::istream& in, DataStruct& dataStruct) const;

      friend std::istream& operator>>(std::istream& in, KeyNameIO& keyName);
    private:
      int keyNumber_ = 0;

      bool isValid() const;
    };

    std::istream& operator>>(std::istream& in, DelimIO&& delim);

    std::istream& operator>>(std::istream& in, UllOctIO&& ullOct);

    std::istream& operator>>(std::istream& in, ComplexIO&& complex);

    std::istream& operator>>(std::istream& in, StringIO&& str);

    std::istream& operator>>(std::istream& in, KeyNameIO& keyName);
  }
}

#endif
