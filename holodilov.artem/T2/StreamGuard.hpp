#ifndef STREAM_GUARD_H
#define STREAM_GUARD_H

#include <iostream>

namespace holodilov
{
  class StreamGuard final
  {
  public:
    explicit StreamGuard(std::basic_ios< char >& stream);

    StreamGuard(const StreamGuard& other) = delete;

    StreamGuard(StreamGuard&& other) = delete;

    ~StreamGuard();

  private:
    std::basic_ios< char >& stream_;
    std::basic_ios< char >::fmtflags flags_;
    std::streamsize width_;
    std::streamsize precision_;
    char fill_;
  };
}

#endif
