#include "stream-guard.hpp"

belyaev::StreamGuard::StreamGuard(std::basic_ios< char >& s):
  s_(s),
  fill_(s.fill()),
  precision_(s.precision()),
  width_(s.width()),
  flags_(s.flags())
{}

belyaev::StreamGuard::~StreamGuard()
{
  s_.precision(precision_);
  s_.flags(flags_);
}
