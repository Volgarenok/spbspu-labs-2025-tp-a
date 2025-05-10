#ifndef SCOPEGUARD_HPP
#define SCOPEGUARD_HPP
#include <ios>

namespace dribas
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char >&);
    ~StreamGuard();
    StreamGuard(const StreamGuard&) = delete;
    StreamGuard(StreamGuard&&) = delete;
    StreamGuard& operator=(const StreamGuard&) = delete;
    StreamGuard& operator=(StreamGuard&&) = delete;
  private:
    std::basic_ios< char >::fmtflags fmtflags_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >& stream_;
    char fill_;
  };
}

#endif
