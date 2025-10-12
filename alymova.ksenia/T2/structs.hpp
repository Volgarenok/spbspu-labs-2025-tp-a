#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <string>
#include <iostream>

namespace alymova
{
  struct UllOctIO
  {
    unsigned long long int& i_oct;
  };
  struct ChrLitIO
  {
    char& c;
  };
  struct StringIO
  {
    std::string& s;
  };
  struct LabelIO
  {
    std::string exp;
  };
  struct DataStruct
  {
    unsigned long long int key1;
    char key2;
    std::string key3;

    bool operator<(const DataStruct& other);
  };
  std::istream& operator>>(std::istream& in, LabelIO&& object);
  std::istream& operator>>(std::istream& in, UllOctIO&& object);
  std::istream& operator>>(std::istream& in, ChrLitIO&& object);
  std::istream& operator>>(std::istream& in, StringIO&& object);
  std::istream& operator>>(std::istream& in, DataStruct& object);
  std::ostream& operator<<(std::ostream& out, const DataStruct& object);
}
#endif
