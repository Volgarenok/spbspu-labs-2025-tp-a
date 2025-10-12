#ifndef DICTIONARY_COLLECTION_HPP
#define DICTIONARY_COLLECTION_HPP
#include "dictionary.hpp"

namespace abramov
{
  struct DictionaryCollection
  {
    DictionaryCollection();
    Dictionary &findDict(const std::string &name);
    const Dictionary &cfindDict(const std::string &name) const;
    void addDict(const std::string &name);
    void addCompleteDict(const std::string &name, const Dictionary &dict);
    void deleteDict(const std::string &name);
  private:
    std::unordered_map< std::string, Dictionary > collection_;
  };
}

#endif
