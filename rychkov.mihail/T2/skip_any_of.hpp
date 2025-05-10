#ifndef SKIP_ANY_OF_HPP
#define SKIP_ANY_OF_HPP

#include <istream>
#include <algorithm>
#include "fmtguard.hpp"

namespace rychkov
{
  namespace iofmt
  {
    template< size_t... Lens >
    struct match_any
    {
      const char* cases[sizeof...(Lens)];
      bool noskipws = false;
      size_t* result_match;
      char* buffer;
      match_any(const char(&... strings)[Lens], bool add_noskipws = false,
          size_t* result_match_p = nullptr, char* buf_p = nullptr):
        cases{strings...},
        noskipws(add_noskipws),
        result_match(result_match_p),
        buffer(buf_p)
      {}
    };
    template< size_t... Lens >
    std::istream& operator>>(std::istream& in, const match_any< Lens... >& possible)
    {
      constexpr size_t min_len = std::min({Lens...});
      constexpr size_t max_len = std::max({Lens...});
      if (min_len <= 1)
      {
        return in;
      }
      std::istream::sentry sentry(in, possible.noskipws);
      if (!sentry)
      {
        return in;
      }
      fmtguard guard(in);

      char static_buffer[max_len]{};
      char* buffer = (possible.buffer == nullptr ? static_buffer : possible.buffer);
      bool skips[sizeof...(Lens)]{};
      constexpr size_t lens[] = {Lens...};
      bool atleast_one = true;
      for (size_t i = 0; atleast_one && (i + 1 < max_len) && in.get(buffer[i]); i++)
      {
        atleast_one = false;
        for (size_t j = 0; j < sizeof...(Lens); j++)
        {
          if (!skips[j])
          {
            if ((i + 1 >= lens[j]) || (buffer[i] != possible.cases[j][i]))
            {
              skips[j] = true;
              continue;
            }
            else if (i + 2 == lens[j])
            {
              if (possible.result_match != nullptr)
              {
                *possible.result_match = j;
              }
              return in;
            }
            atleast_one = true;
          }
        }
      }
      if (possible.result_match != nullptr)
      {
        *possible.result_match = -1;
      }
      in.setstate(std::ios::failbit);
      return in;
    }

    template< size_t... Lens >
    match_any< Lens... > anyof(const char(&... strings)[Lens])
    {
      return {strings...};
    }
    template< size_t... Lens >
    match_any< Lens... > anyof(bool noskipws, const char(&... strings)[Lens])
    {
      return {strings..., noskipws};
    }
    template< size_t... Lens >
    match_any< Lens... > anyof(size_t* result_match, const char(&... strings)[Lens])
    {
      return {strings..., false, result_match};
    }
    template< size_t... Lens >
    match_any< Lens... > anyof(bool noskipws, size_t* result_match, const char(&... strings)[Lens])
    {
      return {strings..., noskipws, result_match};
    }
    template< size_t... Lens >
    match_any< Lens... > anyof(bool noskipws, size_t* result_match, char* buf, const char(&... strings)[Lens])
    {
      return {strings..., noskipws, result_match, buf};
    }
  }
}

#endif
