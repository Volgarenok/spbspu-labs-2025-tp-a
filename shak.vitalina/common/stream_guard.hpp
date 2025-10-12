#ifndef STREAMGUARD_HPP
#define STREAMGUARD_HPP
#include <ios>

namespace shak
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char > &s);
    StreamGuard(const StreamGuard &) = delete;
    StreamGuard(StreamGuard &&) = delete;
    ~StreamGuard();

  private:
    std::basic_ios< char > &s_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };
}
#endif
