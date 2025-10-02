#ifndef IO_ACTIONS
#define IO_ACTIONS

#include <string>
#include <cstddef>
#include "data_struct.hpp"

namespace tkach
{
  struct DoubleIO
  {
    double& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct UllIO
  {
    size_t& ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);

  struct ConstDoubleIO
  {
    const double& ref;
  };

  struct ConstUllIO
  {
    const size_t& ref;
  };

  std::ostream& operator<<(std::ostream& out, const ConstUllIO& dest);
  std::ostream& operator<<(std::ostream& out, const ConstDoubleIO& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

#endif
