#ifndef SPBSPU_LABS_2025_TP_A_FIRSTRY_T2_DATA_STRUCT_HPP
#define SPBSPU_LABS_2025_TP_A_FIRSTRY_T2_DATA_STRUCT_HPP

#include <iosfwd>
#include <string>

namespace firstry {
  struct DataStruct;
  bool operator<(const DataStruct&, const DataStruct&);
  std::istream& operator>>(std::istream&, DataStruct&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
}

struct firstry::DataStruct
{
  struct Rational
  {
    long long numerator;
    unsigned long long denominator;
  };
  double key1;  // DBL SCI
  Rational key2;  // RAT LSP
  std::string key3;
};

#endif
