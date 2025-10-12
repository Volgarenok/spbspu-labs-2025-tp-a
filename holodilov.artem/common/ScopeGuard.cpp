#include "ScopeGuard.hpp"

holodilov::ScopeGuard::ScopeGuard(std::basic_ios< char >& stream):
  stream_(stream),
  flags_(stream.flags()),
  width_(stream.width()),
  precision_(stream.precision()),
  fill_(stream.fill())
{}

holodilov::ScopeGuard::~ScopeGuard()
{
  stream_.flags(flags_);
  stream_.width(width_);
  stream_.precision(precision_);
  stream_.fill(fill_);
}
