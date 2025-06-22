#include "format_guard.hpp"

namespace fedorov
{
  IOFormatGuard::IOFormatGuard(std::ostream &s):
    s_(s),
    flags_(s.flags()),
    precision_(s.precision())
  {}

  IOFormatGuard::~IOFormatGuard()
  {
    s_.precision(precision_);
    s_.flags(flags_);
  }
}
