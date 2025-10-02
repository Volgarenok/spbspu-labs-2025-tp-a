#include "guard.hpp"

smirnov::StreamGuard::StreamGuard(std::basic_ios< char > & stream):
  stream_(stream),
  fmt_(stream.flags()),
  width_(stream.width()),
  precision_(stream.precision()),
  fill_(stream.fill())
{}

smirnov::StreamGuard::~StreamGuard()
{
  stream_.flags(fmt_);
  stream_.width(width_);
  stream_.precision(precision_);
  stream_.fill(fill_);
}
