#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <string>
#include <complex>

namespace mozhegova
{
  struct DataStruct
  {
    unsigned long long key1;
    std::complex< double > key2;
    std::string key3;

    bool operator<(const DataStruct & other) const;
  };
  std::istream & operator>>(std::istream & in, DataStruct & dest);
  std::ostream & operator<<(std::ostream & out, const DataStruct & dest);
}

#endif
