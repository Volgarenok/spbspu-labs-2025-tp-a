#ifndef IOS_HPP
#define IOS_HPP

#include <ios>

namespace firstime {
  class StreamGuard;
}

class firstime::StreamGuard {
public:
  StreamGuard(const StreamGuard&) = delete;
  explicit StreamGuard(std::basic_ios<char>&);
  ~StreamGuard();

  StreamGuard& operator=(const StreamGuard&) = delete;

private:
  std::basic_ios<char>& s_;
  std::streamsize precision_;
  std::streamsize width_;
  std::basic_ios<char>::fmtflags flags_;
  char fill_;
};

#endif