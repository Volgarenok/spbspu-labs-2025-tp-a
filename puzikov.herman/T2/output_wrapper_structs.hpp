#ifndef OUTPUT_WRAPPER_STRUCTS
#define OUTPUT_WRAPPER_STRUCTS

#include <ostream>

namespace puzikov
{
  namespace output
  {
    struct ULLValue
    {
      const unsigned long long &ref;
    };
    std::ostream &operator<<(std::ostream &out, const ULLValue &source);

    struct PairValue
    {
      const std::pair< long long, unsigned long long > &ref;
    };
    std::ostream &operator<<(std::ostream &out, const PairValue &source);

    struct StringValue
    {
      const std::string &ref;
    };
    std::ostream &operator<<(std::ostream &out, const StringValue &source);
  }
}

#endif
