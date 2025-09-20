#ifndef STREAMGUARD_HPP
#define STREAMGUARD_HPP
#include <ios>

namespace krylov
{
  class StreamGuard
  {
  public:
    StreamGuard(const StreamGuard&) = delete;
    StreamGuard(StreamGuard&&) = delete;
    explicit StreamGuard(std::basic_ios< char >& s);
    ~StreamGuard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}

#endif
