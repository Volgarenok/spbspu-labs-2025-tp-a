#include "stream_guard.hpp"

namespace kalmbah {

StreamGuard::StreamGuard(std::basic_ios<char>& s) :
    stream_(s),
    flags_(s.flags()),
    precision_(s.precision()),
    fill_(s.fill())
{}

StreamGuard::~StreamGuard() {
    stream_.flags(flags_);
    stream_.precision(precision_);
    stream_.fill(fill_);
}

}
