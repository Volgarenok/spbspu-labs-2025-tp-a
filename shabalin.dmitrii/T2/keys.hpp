#ifndef KEYS_HPP
#define KEYS_HPP

#include <complex>
#include <istream>
#include <string>

namespace shabalin
{
  struct ComplexKey
  {
    std::complex< double >& value;
  };
  std::istream& operator>>(std::istream& in, ComplexKey&& key);

  struct DoubleKey
  {
    double& value;
  };

  std::istream& operator>>(std::istream& in, DoubleKey&& key);

  struct StringKey
  {
    std::string& value;
  };
  std::istream& operator>>(std::istream& in, StringKey&& key);
}

#endif

