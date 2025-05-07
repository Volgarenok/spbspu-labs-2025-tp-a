#ifndef SCOPE_GUARD_HPP
#define SCOPE_GUARD_HPP
#include <iostream>

namespace kostyukov
{
  class ScopeGuard
  {
    public:
      explicit ScopeGuard(std::basic_ios< char >& s);
      ~ScopeGuard();
      ScopeGuard(const ScopeGuard&) = delete;
      ScopeGuard(ScopeGuard&&) = delete;
      ScopeGuard& operator=(const ScopeGuard&) = delete;
      ScopeGuard& operator=(ScopeGuard&&) = delete;
    private:
      std::streamsize precision_;
      std::streamsize width_;
      std::basic_ios< char >& s_;
      std::basic_ios< char >::fmtflags fmt_;
      char fill_;
  };
}
#endif
