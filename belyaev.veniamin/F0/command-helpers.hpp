#ifndef COMMAND_HELPERS_HPP
#define COMMAND_HELPERS_HPP
#include "dictionary.hpp"

namespace belyaev
{
  Dictionary* searchDictByName(Dictionaries& data, const std::string& name);
  const Dictionary* searchDictByName(const Dictionaries& data, const std::string& name); // ¯\_(ツ)_/¯
  bool insertEntry(Dictionary& currentDictionary, std::string russianWord, std::string translation);
  bool removeEntry(Dictionary& currentDictionary, std::string russianWord);

  using dictionaryIterator = std::_Rb_tree_const_iterator< entryPair >;
  bool isTranslationInEntry(const std::pair< const std::string, std::string > entry, const std::string& translation);
  dictionaryIterator getItOfWordInDictByRu(const Dictionary& dictionary, const std::string& ruWord);
  dictionaryIterator getItOfWordInDictByEn(const Dictionary& dictionary, const std::string& enWord);
  bool isRuWordInDictionary(const Dictionary& dictionary, dictionaryIterator itRuWord);
  bool isEnWordInDictionary(const Dictionary& dictionary, dictionaryIterator itEnWord);

  bool pairContainsRuChars(const entryPair entry, std::string subStringRu);
  bool pairContainsEnChars(const entryPair entry, std::string subStringEn);

  const Dictionary& printAllHelper(const std::pair< const std::string, Dictionary >& dictsEntry, std::ostream& out);

  struct areAllNamesInDicts
  {
    areAllNamesInDicts(Dictionaries& data, const std::vector< std::string > names):
      data_(data),
      names_(names)
    {}
    bool isDictionaryInDicts(std::string name);
    bool operator()();
    Dictionaries& data_;
    const std::vector< std::string > names_;
  };

  class mergeDictsIterator
  {
    const Dictionaries& data_;
    Dictionary& dest_;
    public:
      mergeDictsIterator(const Dictionaries& data, Dictionary& dest):
        data_(data),
        dest_(dest)
      {}
      mergeDictsIterator& operator=(const std::string& srcName);
      mergeDictsIterator& operator*();
      mergeDictsIterator& operator++();
    };

  class intersectDictsIterator
  {
    const Dictionaries& data_;
    Dictionary& dest_;
    std::vector< std::string > dictNames_;
    bool first_;
  public:
    intersectDictsIterator(const Dictionaries& data, Dictionary& dest, const std::vector< std::string >& names):
      data_(data),
      dest_(dest),
      dictNames_(names),
      first_(true)
    {}
    intersectDictsIterator& operator=(const std::string& srcName);
    intersectDictsIterator& operator*();
    intersectDictsIterator& operator++();
  };

  bool entryExistsInDict(const Dictionary& dict, const entryPair& entry);
  struct removeIntersectionsHelper
  {
    Dictionaries& data_;
    const std::vector< std::string >& dictNames_;
    removeIntersectionsHelper(Dictionaries& data, const std::vector< std::string >& names):
      data_(data),
      dictNames_(names)
    {}
    bool isEntryInOtherDicts(const entryPair& entry, const std::string& currentDictName) const;
    void operator()(const std::string& dictName);
  };

  struct entryNotInOtherDicts
  {
    const removeIntersectionsHelper& helper_;
    const std::string& currentDict_;
    entryNotInOtherDicts(const removeIntersectionsHelper& helper, const std::string& currentDict):
      helper_(helper),
      currentDict_(currentDict)
    {}

    bool operator()(const nonConstEntryPair& entry) const
    {
      return !helper_.isEntryInOtherDicts(entry, currentDict_);
    }
  };

  bool isDictContainingEntry(const Dictionaries& data, const std::string& dictName,
    const entryPair& entry, const std::string& currentDict);
}

#endif
