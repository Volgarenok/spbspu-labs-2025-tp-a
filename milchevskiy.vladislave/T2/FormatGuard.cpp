#include "FormatGuard.hpp"

milchevskiy::FormatGuard::FormatGuard(std::basic_ios<char>& s):
	s_(s),
	fill_(s.fill()),
	precision_(s.precision()),
	fmt_(s.flags()),
	width_(s.width())
{
}

milchevskiy::FormatGuard::~FormatGuard()
{
	s_.fill(fill_);
	s_.precision(precision_);
	s_.flags(fmt_);
	s_.width(width_);
}

