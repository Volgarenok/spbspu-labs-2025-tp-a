#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <ios>

namespace kalmbah {

class StreamGuard {
public:
    explicit StreamGuard(std::basic_ios<char>& s);
    StreamGuard(const StreamGuard&) = delete;
    StreamGuard(StreamGuard&&) = delete;
    ~StreamGuard();
private:
    std::basic_ios<char>& stream_;
    std::ios_base::fmtflags flags_;
    std::streamsize precision_;
    char fill_;
};

}

#endif
