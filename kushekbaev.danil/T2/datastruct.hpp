#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <string>
#include <ios>

namespace kushekbaev
{
  struct DataStruct
  {
    unsigned long long key1;
    char key2;
    std::string key3;
    bool operator<(const DataStruct& rhs) const;
  };

  struct ULLBinaryI
  {
    unsigned long long& ref;
  };

  struct ULLBinaryO
  {
    const unsigned long long& ref;
  };

  struct ChrLitIO
  {
    char& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, ULLBinaryI&& obj);
  std::istream& operator>>(std::istream& in, ChrLitIO&& obj);
  std::istream& operator>>(std::istream& in, StringIO&& obj);
  std::istream& operator>>(std::istream& in, DataStruct& obj);
  std::ostream& operator<<(std::ostream& out, const ULLBinaryO&& obj);
  std::ostream& operator<<(std::ostream& out, const DataStruct& obj);
}

#endif
