#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>

namespace bob
{
  struct DataStruct
  {
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct ULLLiteralIO
  {
    unsigned long long &ref;
  };

  struct ULLBinaryI
  {
    unsigned long long &ref;
  };

  struct ULLBinaryO
  {
    const unsigned long long &ref;
  };

  struct StringIO
  {
    std::string &ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  class IoGuard
  {
  public:
    explicit IoGuard(std::basic_ios< char > &s);
    ~IoGuard();

  private:
    std::basic_ios< char > &s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  bool compareDataStruct(const DataStruct &a, const DataStruct &b);
  std::istream &operator>>(std::istream &is, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &is, ULLLiteralIO &&dest);
  std::istream &operator>>(std::istream &is, ULLBinaryI &&dest);
  std::istream &operator>>(std::istream &is, StringIO &&dest);
  std::istream &operator>>(std::istream &is, LabelIO &&dest);
  std::istream &operator>>(std::istream &is, DataStruct &data);
  std::ostream &operator<<(std::ostream &os, const DataStruct &data);
  std::ostream &operator<<(std::ostream &os, const ULLBinaryO &dest);

};
#endif
