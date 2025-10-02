#ifndef SCOPE_GUARD_HPP
#define SCOPE_GUARD_HPP

#include <iostream>

namespace holodilov
{
  class ScopeGuard
  {
  public:
    explicit ScopeGuard(std::basic_ios< char >& stream);

    ScopeGuard(const ScopeGuard& other) = delete;

    ScopeGuard(ScopeGuard&& other) = delete;

    ~ScopeGuard();

  private:
    std::basic_ios< char >& stream_;
    std::basic_ios< char >::fmtflags flags_;
    std::streamsize width_;
    std::streamsize precision_;
    char fill_;
  };
}

#endif
