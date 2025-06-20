#ifndef IO_UTILITIES_HPP
#define IO_UTILITIES_HPP
#include <string>
#include <istream>
#include "DataStruct.hpp"
namespace nikonov
{
  namespace detail
  {
    struct DelimiterIO
    {
      char exp_;
    };
    struct DoubleIO
    {
      double& ref_;
    };
    struct UnsignedLongLong
    {
      unsigned long long& ref_;
    };
    struct StringIO
    {
      std::string& ref_;
    };
    struct LabelIO
    {
      std::string& ref_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleIO&& dest);
    std::istream& operator>>(std::istream& in, UnsignedLongLong&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);

    std::ostream& operator<<(std::ostream& out, DoubleIO&& dest);
    std::ostream& operator<<(std::ostream& out, UnsignedLongLong&& dest);
    std::ostream& operator<<(std::ostream& out, StringIO&& dest);
  }
}
#endif
