#include "StreamGuard.hpp"

holodilov::StreamGuard::StreamGuard(std::basic_ios< char >& stream):
  stream_(stream),
  flags_(stream.flags()),
  width_(stream.width()),
  precision_(stream.precision()),
  fill_(stream.fill())
{}

holodilov::StreamGuard::~StreamGuard()
{
  stream_.flags(flags_);
  stream_.width(width_);
  stream_.precision(precision_);
  stream_.fill(fill_);
}
