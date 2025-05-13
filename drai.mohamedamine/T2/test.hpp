#ifndef TEST_HPP
#define TEST_HPP

#include <string>
#include <utility>

namespace amine
{
<<<<<<< HEAD
  struct DelimiterIO
  {
    char exp;
  };
  struct DoubleSCI
  {
    double &ref;
  };
  struct RationalLSP
  {
    std::pair<long long, unsigned long long> &ref;
  };
  struct StringIO
  {
    std::string &ref;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, DoubleSCI &&dest);
  std::istream &operator>>(std::istream &in, RationalLSP &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
=======
    struct DelimiterIO { char exp; };
    struct DoubleSCI { double &ref; };
    struct RationalIO { std::pair<long long, unsigned long long> &ref; };
    struct StringIO { std::string &ref; };
    std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
    std::istream &operator>>(std::istream &in, DoubleSCI &&dest);
    std::istream &operator>>(std::istream &in, RationalIO &&dest);
    std::istream &operator>>(std::istream &in, StringIO &&dest);
>>>>>>> c515838cf784c143137d2c17bd825b2c1b191313
}

#endif
