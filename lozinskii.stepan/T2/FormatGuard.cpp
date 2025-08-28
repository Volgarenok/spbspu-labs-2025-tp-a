#include "FormatGuard.hpp"

FormatGuard::FormatGuard(std::ios& stream) :
	stream_(stream),
	saved_(nullptr)
{
	saved_.copyfmt(stream_);
}

FormatGuard::~FormatGuard()
{
	stream_.copyfmt(saved_);
}
