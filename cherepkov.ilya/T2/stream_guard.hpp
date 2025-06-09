#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <iostream>

namespace cherepkov
{
  class StreamGuard
  {
   public:
    explicit StreamGuard(std::basic_ios< char >& s);
    ~StreamGuard();

    StreamGuard(const StreamGuard&) = delete;
    StreamGuard& operator=(const StreamGuard&) = delete;
    StreamGuard(StreamGuard&&) = delete;
    StreamGuard& operator=(StreamGuard&&) = delete;

   private:
    std::basic_ios< char >& s_;
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };
}

#endif
