#include "scopeguard.hpp"

smirnov::ScopeGuard::ScopeGuard(std::basic_ios< char >& s):
  s_(s),
  precision_(s.precision()),
  flags_(s.flags())
{}

smirnov::ScopeGuard::~ScopeGuard()
{
  s_.precision(precision_);
  s_.flags(flags_);
}

