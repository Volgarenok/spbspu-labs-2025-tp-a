#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <map>
#include <string>
#include <cstddef>

namespace shiryaeva
{
  struct FrequencyDictionary
  {
    std::map< std::string, size_t > dict;
    size_t total_words = 0;

    void clear();
    void add_word(const std::string &word);
    bool remove_word(const std::string &word);
    size_t get_freq(const std::string &word) const;
    double get_rel_freq(const std::string &word) const;
  };
}

#endif
