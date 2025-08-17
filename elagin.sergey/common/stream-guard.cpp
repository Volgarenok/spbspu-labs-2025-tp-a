#include "stream-guard.hpp"
#include <mutex>
#include <iostream>

namespace elagin
{
    std::mutex StreamGuard::mutex_;

    StreamGuard::StreamGuard(std::basic_ios<char>& s)
        : stream_(s),
          fill_(s.fill()),
          precision_(s.precision()),
          flags_(s.flags())
    {
        mutex_.lock();
    }

    StreamGuard::~StreamGuard()
    {
        try
        {
            stream_.fill(fill_);
            stream_.precision(precision_);
            stream_.flags(flags_);
        }
        catch (...)
        {
            std::cerr << "StreamGuard: failed to restore stream state\n";
        }
        mutex_.unlock();
    }
}