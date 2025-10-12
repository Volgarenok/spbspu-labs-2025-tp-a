#include "iofmtguard.h"

ohantsev::iofmtguard::iofmtguard(std::basic_ios< char >& s):
  precision_(s.precision()),
  width_(s.width()),
  s_(s),
  fmt_(s.flags()),
  fill_(s.fill())
{}

ohantsev::iofmtguard::~iofmtguard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
