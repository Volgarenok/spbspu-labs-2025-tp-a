#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <ios>
#include <mutex>

namespace elagin {
class StreamGuard {
public:
  explicit StreamGuard(std::basic_ios<char> &s);
  ~StreamGuard();

  StreamGuard(const StreamGuard &) = delete;
  StreamGuard &operator=(const StreamGuard &) = delete;
  StreamGuard(StreamGuard &&) = delete;
  StreamGuard &operator=(StreamGuard &&) = delete;

private:
  std::basic_ios<char> &stream_;
  char fill_;
  std::streamsize precision_;
  std::basic_ios<char>::fmtflags flags_;
  static std::mutex mutex_;
};
} // namespace elagin

#endif // STREAM_GUARD_HPP
