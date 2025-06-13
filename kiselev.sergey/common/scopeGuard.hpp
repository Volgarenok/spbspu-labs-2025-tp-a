#ifndef SCOPEGUARD_HPP
#define SCOPEGUARD_HPP
#include <ios>

namespace detail
{
  class ScopeGuard
  {
  public:
    explicit ScopeGuard(std::basic_ios< char >& s);
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard(ScopeGuard&&) = delete;
    ~ScopeGuard();
  private:
    std::basic_ios< char >& s_;
    std::basic_ios< char >::fmtflags fmt_;
    std::streamsize width_;
    std::streamsize precision_;
    char fill_;
  };
}
#endif
