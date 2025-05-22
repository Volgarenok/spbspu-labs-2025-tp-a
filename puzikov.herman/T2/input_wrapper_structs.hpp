#ifndef INPUT_FORMAT_HPP
#define INPUT_FORMAT_HPP

#include <string>
#include <utility>

namespace puzikov
{
  namespace input
  {
    struct Character
    {
      char exp;
    };
    std::istream &operator>>(std::istream &in, Character &&dest);

    struct ULLValue
    {
      unsigned long long &ref;
    };
    std::istream &operator>>(std::istream &in, ULLValue &&dest);

    struct PairValue
    {
      std::pair< long long, unsigned long long > &ref;
    };
    std::istream &operator>>(std::istream &in, PairValue &&dest);

    struct StringValue
    {
      std::string &ref;
    };
    std::istream &operator>>(std::istream &in, StringValue &&dest);
  }
}
#endif
