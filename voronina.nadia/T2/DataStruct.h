#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <iomanip>


namespace voronina
{
  struct DataStruct
  {
    double key1;
    unsigned long long int key2;
    std::string key3;
  };

  struct DelimiterDIO {};

  struct DoubleIO
  {
    double &ref;
  };

  struct UnsignedLongLongIO
  {
    unsigned long long int &ref;
  };

  struct StringIO
  {
    std::string &ref;
  };

  std::istream &operator>>(std::istream &in, DoubleIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
  std::istream &operator>>(std::istream &in, UnsignedLongLongIO &&dest);
  std::istream &operator>>(std::istream &in, DataStruct &dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &dest);
  std::istream &operator>>(std::istream &in, DelimiterDIO &&);

  bool compare(const DataStruct &left, const DataStruct &right);
}
#endif
