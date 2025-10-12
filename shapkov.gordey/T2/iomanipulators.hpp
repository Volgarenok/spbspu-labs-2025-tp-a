#ifndef INPUT_OUTPUT_MANIPULATORS_HPP
#define INPUT_OUTPUT_MANIPULATORS_HPP
#include <iostream>
#include <string>

namespace shapkov
{
  struct DoubleScientificIO
  {
    double& key;
  };
  struct RatioIO
  {
    std::pair< long long, unsigned long long >& key;
  };
  struct StringIO
  {
    std::string& key;
  };
  struct LabelIO
  {
    const std::string& exp;
  };
  std::istream& operator>>(std::istream& in, DoubleScientificIO&& rhs);
  std::istream& operator>>(std::istream& in, RatioIO&& rhs);
  std::istream& operator>>(std::istream& in, StringIO&& rhs);
  std::istream& operator>>(std::istream& in, LabelIO&& rhs);
  std::ostream& operator<<(std::ostream& out, const DoubleScientificIO& rhs);
}
#endif
