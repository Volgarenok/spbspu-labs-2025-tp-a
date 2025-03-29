#ifndef TYPES_HPP
#define TYPES_HPP
#include <iostream>
#include <string>

namespace shapkov
{
  struct doubleScientificIO
  {
    double& key;
  };
  struct ratioIO
  {
    std::pair< long long, unsigned long long >& key;
  };
  struct stringIO
  {
    std::string& key;
  };
  struct labelIO
  {
    std::string exp;
  };
  std::istream& operator>>(std::istream& in, doubleScientificIO&& rhs);
  std::istream& operator>>(std::istream& in, ratioIO&& rhs);
  std::istream& operator>>(std::istream& in, stringIO&& rhs);
  std::istream& operator>>(std::istream& in, labelIO&& rhs);
  std::ostream& operator<<(std::ostream& out, const doubleScientificIO& rhs);
}
#endif
