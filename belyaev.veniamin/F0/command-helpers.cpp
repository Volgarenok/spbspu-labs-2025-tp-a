#include "command-helpers.hpp"
#include <functional>
#include <algorithm>

belyaev::Dictionary* belyaev::searchDictByName(Dictionaries& data, const std::string& name)
{
  auto result = data.dicts.find(name);
  if (result == data.dicts.end())
  {
    return nullptr;
  }
  return &(result->second);
}

const belyaev::Dictionary* belyaev::searchDictByName(const Dictionaries& data, const std::string& name)
{
  auto result = data.dicts.find(name);
  if (result == data.dicts.end())
  {
    return nullptr;
  }
  return &(result->second);
}

bool belyaev::insertEntry(Dictionary& currentDictionary, std::string russianWord, std::string translation)
{
  if (isRuWordInDictionary(currentDictionary, getItOfWordInDictByRu(currentDictionary, russianWord)))
  {
    return false;
  }
  currentDictionary.dict[russianWord] = translation;
  return true;
}

bool belyaev::removeEntry(Dictionary& currentDictionary, std::string russianWord)
{
  if (!isRuWordInDictionary(currentDictionary, getItOfWordInDictByRu(currentDictionary, russianWord)))
  {
    return false;
  }
  currentDictionary.dict.erase(russianWord);
  return true;
}


bool belyaev::isTranslationInEntry(const std::pair< const std::string, std::string > entry, const std::string& translation)
{
  return entry.second == translation;
}

belyaev::dictionaryIterator belyaev::getItOfWordInDictByRu(const Dictionary& dictionary, const std::string& ruWord)
{
  return dictionary.dict.find(ruWord);
}

belyaev::dictionaryIterator belyaev::getItOfWordInDictByEn(const Dictionary& dictionary, const std::string& enWord)
{
  using namespace std::placeholders;
  auto isTranslationInEntryBind = std::bind(isTranslationInEntry, _1, enWord);
  return std::find_if(dictionary.dict.begin(), dictionary.dict.end(), isTranslationInEntryBind);
}

bool belyaev::isRuWordInDictionary(const Dictionary& dictionary, dictionaryIterator itRuWord)
{
  return itRuWord != dictionary.dict.end();
}

bool belyaev::isEnWordInDictionary(const Dictionary& dictionary, dictionaryIterator itEnWord)
{
  return itEnWord != dictionary.dict.end();
}

bool belyaev::pairContainsRuChars(const entryPair entry, std::string subStringRu)
{
  return entry.first.find(subStringRu) != std::string::npos;
}

bool belyaev::pairContainsEnChars(const entryPair entry, std::string subStringEn)
{
  return entry.second.find(subStringEn) != std::string::npos;
}

const belyaev::Dictionary& belyaev::printAllHelper(const std::pair< const std::string, Dictionary >& dictsEntry, std::ostream& out)
{
  out << "Dictionary " << dictsEntry.first << ":\n";
  return dictsEntry.second;
}

bool belyaev::areAllNamesInDicts::isDictionaryInDicts(std::string name)
{
  return searchDictByName(data_, name) != nullptr;
}

bool belyaev::areAllNamesInDicts::operator()()
{
  using namespace std::placeholders;
  auto isDictionaryInDictsBind = std::bind(&areAllNamesInDicts::isDictionaryInDicts, this, _1);
  return std::any_of(names_.begin(), names_.end(), isDictionaryInDictsBind);
}

belyaev::mergeDictsIterator& belyaev::mergeDictsIterator::operator=(const std::string& srcName)
{
  const Dictionary* src = searchDictByName(data_, srcName);
  std::copy(src->dict.begin(), src->dict.end(), std::inserter(dest_.dict, dest_.dict.end()));
  return *this;
}

belyaev::mergeDictsIterator& belyaev::mergeDictsIterator::operator*()
{
  return *this;
}

belyaev::mergeDictsIterator& belyaev::mergeDictsIterator::operator++()
{
  return *this;
}

belyaev::intersectDictsIterator& belyaev::intersectDictsIterator::operator=(const std::string& srcName)
{
  using namespace std::placeholders;

  const Dictionary* src = searchDictByName(data_, srcName);
  auto inserter = std::inserter(dest_.dict, dest_.dict.end());
  if (first_)
  {
    std::copy(src->dict.begin(), src->dict.end(), inserter);
    first_ = false;
  }
  else
  {
    std::vector< nonConstEntryPair > tempDict;
    std::copy(dest_.dict.begin(), dest_.dict.end(), std::back_inserter(tempDict));

    dest_.dict.clear();
    auto entryExistsChecker = std::bind(entryExistsInDict, std::cref(*src), _1);
    std::copy_if(tempDict.begin(), tempDict.end(), inserter, entryExistsChecker);
  }
  return *this;
}

belyaev::intersectDictsIterator& belyaev::intersectDictsIterator::operator*()
{
  return *this;
}

belyaev::intersectDictsIterator& belyaev::intersectDictsIterator::operator++()
{
  return *this;
}

bool belyaev::entryExistsInDict(const Dictionary& dict, const entryPair& entry)
{
  auto it = dict.dict.find(entry.first);
  return it != dict.dict.end() && it->second == entry.second;
}

void belyaev::removeIntersectionsHelper::operator()(const std::string& dictName)
{
  Dictionary* dict = searchDictByName(data_, dictName);
  if (dict == nullptr) return;

  std::vector< nonConstEntryPair > dictCopy;
  std::copy(dict->dict.begin(), dict->dict.end(), std::back_inserter(dictCopy));

  entryNotInOtherDicts keepPredicate(*this, dictName);

  auto partitionPoint = std::stable_partition(dictCopy.begin(), dictCopy.end(), keepPredicate);

  dict->dict.clear();
  std::copy(dictCopy.begin(), partitionPoint, std::inserter(dict->dict, dict->dict.end()));
}

bool belyaev::isDictContainingEntry(const Dictionaries& data, const std::string& dictName,
  const entryPair& entry, const std::string& currentDictName)
{
  if (dictName == currentDictName)
  {
    return false;
  }
  const Dictionary* dict = searchDictByName(data, dictName);
  if (dict == nullptr)
  {
    return false;
  }
  return entryExistsInDict(*dict, entry);
}

bool belyaev::removeIntersectionsHelper::isEntryInOtherDicts(const entryPair& entry, const std::string& currentDictName) const
{
  using namespace std::placeholders;
  auto checker = std::bind(isDictContainingEntry, std::cref(data_), _1, std::cref(entry), std::cref(currentDictName));
  return std::any_of(dictNames_.begin(), dictNames_.end(), checker);
}
