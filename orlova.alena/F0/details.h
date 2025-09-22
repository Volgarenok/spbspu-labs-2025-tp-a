#ifndef DETAILS_H
#define DETAILS_H

#include <algorithm>
#include "commands.h"
#include "io-utils.h"

namespace details
{
  using namespace orlova;
  bool dictionaryExists(const Dictionaries&, const std::string&);
  void printTranslations(std::ostream&, const Translations&);
  Translations intersectLists(Translations&, Translations&);
  std::pair< std::string, Translations > intersectListDict(const std::pair< std::string, Translations >&, const Dictionary&);

  struct WordPairGeneratorIterator
  {
  public:
    using value_type = std::pair< std::string, std::list< std::string > >;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::input_iterator_tag;

    WordPairGeneratorIterator():
      file(nullptr),
      end(true)
    {}

    WordPairGeneratorIterator(std::istream& f):
      file(&f),
      end(false)
    {
      ++(*this);
    }

    WordPairGeneratorIterator(const WordPairGeneratorIterator& other) = default;

    WordPairGeneratorIterator& operator=(const WordPairGeneratorIterator& other) = default;

    WordPairGeneratorIterator& operator++()
    {
      value = value_type();
      std::string englishWord;
      std::list< std::string > russianWords;
      if (*file >> englishWord >> russianWords)
      {
        value = std::make_pair(englishWord, russianWords);
      }
      else
      {
        end = true;
      }
      return *this;
    }

    reference operator*() const
    {
      return value;
    }

    pointer operator->() const
    {
      return &value;
    }

    explicit operator bool() const
    {
      return !end;
    }

    bool operator==(const WordPairGeneratorIterator& other) const
    {
      return end == other.end;
    }

    bool operator!=(const WordPairGeneratorIterator& other) const
    {
      return !(*this == other);
    }

  private:
    std::istream* file = nullptr;
    value_type value;
    bool end = true;
  };

  struct DictMerger
  {
    using Pair = std::pair< std::string, std::list< std::string > >;
    Dictionary newDict;
    DictMerger(const Dictionary& dict):
      newDict(dict)
    {}
    Pair operator()(const Pair& pair)
    {
      auto it = newDict.find(pair.first);
      if (it != newDict.end())
      {
        it->second.insert(it->second.end(), pair.second.begin(), pair.second.end());
        return *it;
      }
      else
      {
        newDict[pair.first] = pair.second;
        return pair;
      }
    }
  };

  struct CompareKeys
  {
    bool operator()(const std::pair< std::string, orlova::Translations >& pair1, const std::pair< std::string, Translations >& pair2)
    {
      return pair1.first < pair2.first;
    }
  };

  struct KeyExtractor
  {
    using Pair = std::pair< std::string, std::list < std::string > >;
    std::string operator()(const Pair& p) const
    {
      return p.first;
    }
  };

  struct PairTransformer
  {
    using Pair = std::pair< std::string, std::list< std::string > >;
    const Dictionary& dict1;
    const Dictionary& dict2;

    PairTransformer(const Dictionary& d1,
      const Dictionary& d2):
      dict1(d1),
      dict2(d2)
    {}

    Pair operator()(std::string& key)
    {
      if (dict1.find(key) != dict1.end())
      {
        return { key, dict1.at(key) };
      }
      else
      {
        return { key, dict2.at(key) };
      }
    }
  };

  struct ResidualTransform
  {
    const Dictionary& dict1;
    ResidualTransform(const Dictionary& d1):
      dict1(d1)
    {}
    using Pair = std::pair< std::string, std::list< std::string > >;
    Pair operator()(std::string& key) const
    {
      return std::make_pair(key, dict1.at(key));
    }
  };
}

template<>
struct std::iterator_traits< details::WordPairGeneratorIterator >
{
  using value_type = details::WordPairGeneratorIterator::value_type;
  using pointer = details::WordPairGeneratorIterator::pointer;
  using reference = details::WordPairGeneratorIterator::reference;
  using iterator_category = details::WordPairGeneratorIterator::iterator_category;
};

#endif
