#include "streamGuard.hpp"

dribas::StreamGuard::StreamGuard(std::basic_ios< char >& stream):
  fmtflags_(stream.flags()),
  width_(stream.width()),
  precision_(stream.precision()),
  stream_(stream),
  fill_(stream.fill())
{}

dribas::StreamGuard::~StreamGuard()
{
  stream_.width(width_);
  stream_.fill(fill_);
  stream_.precision(precision_);
  stream_.flags(fmtflags_);
}
