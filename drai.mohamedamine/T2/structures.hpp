#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <iosfwd>
#include <string>

namespace firstime {
  struct DataStruct;
  bool operator<(const DataStruct&, const DataStruct&);
  std::istream& operator>>(std::istream&, DataStruct&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
}

struct firstime::DataStruct {
  struct Rational {
    long long numerator;
    unsigned long long denominator;
  };
  double key1;  // DBL SCI
  Rational key2; // RAT LSP
  std::string key3;
};

#endif
