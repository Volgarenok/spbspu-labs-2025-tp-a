#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <string>

namespace demehin
{
  struct DataStruct
  {
    long long key1;
    double key2;
    std::string key3;

    bool operator<(const DataStruct&) const noexcept;
  };

  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);

  namespace io
  {
    struct DoubleIO
    {
      double& ref;
    };

    struct LlIO
    {
      long long& ref;
    };

    struct KeyNumIO
    {
      int key;
    };

    struct StringIO
    {
      std::string& ref;
    };

    std::istream& operator>>(std::istream& in, DoubleIO&& dest);
    std::istream& operator>>(std::istream& in, LlIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, KeyNumIO& dest);
    std::ostream& operator<<(std::ostream& out, const DoubleIO&& dest);
    std::ostream& operator<<(std::ostream& out, const LlIO&& dest);
  }
}

#endif
