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
    std::ostream &operator<<(std::ostream &out, const ULLValue &dest);

    struct PairValue
    {
      const std::pair< long long, unsigned long long > &ref;
    };
    std::ostream &operator<<(std::ostream &out, const PairValue &dest);

    struct StringValue
    {
      const std::string &ref;
    };
    std::ostream &operator<<(std::ostream &out, const StringValue &dest);
  } // namespace output
} // namespace puzikov

#endif
