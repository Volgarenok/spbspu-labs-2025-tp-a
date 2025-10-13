#ifndef VALUE_HPP
#define VALUE_HPP

#include <istream>
#include <string>
#include <utility>

namespace smirnov
{
  struct ULLBinary
  {
    unsigned long long& value;
  };

  std::istream& operator>>(std::istream& in, ULLBinary&& key);

  struct DBLLit
  {
    double& value;
  };

  std::istream& operator>>(std::istream& in, DBLLit&& key);

  struct StrKey
  {
    std::string& str;
  };

  std::istream& operator>>(std::istream& in, StrKey&& key);
}
#endif

