#include "commands.h"
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>
#include <functional>
#include "io-utils.h"
#include "details.h"

void orlova::printDictionary(std::istream& in, std::ostream& out, const Dictionaries& dicts)
{
  std::string name;
  in >> name;
  if (!in || dicts.find(name) == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dict = dicts.at(name);
  if (dict.empty())
  {
    out << "<DICTIONARY IS EMPTY>\n";
    return;
  }
  else
  {
    out << dicts.at(name);
  }
}

void orlova::addEmptyDictionary(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string name;
  in >> name;
  if (!in || dicts.find(name) != dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  dicts[name] = Dictionary();
  out << "<DICTIONARY ADDED>\n";
}

void orlova::addTranslation(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName, englishWord, russianWord;
  in >> dictName >> englishWord >> russianWord;
  auto it = dicts.find(dictName);
  if (!details::dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (it->second.find(englishWord) != it->second.end())
  {
    it->second[englishWord].push_back(russianWord);
  }
  else
  {
    std::list< std::string > russianWords;
    russianWords.push_back(russianWord);
    it->second[englishWord] = russianWords;
  }
  out << "<TRANSLATION ADDED>\n";
}

void orlova::addDictionary(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName;
  std::string fileName;
  in >> dictName >> fileName;
  if (details::dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Dictionary newDict;
  details::WordPairGeneratorIterator generator(file);
  std::copy(generator, details::WordPairGeneratorIterator{}, std::inserter(newDict, newDict.begin()));
  dicts[dictName] = newDict;
  file.close();
  out << "<DICTIONARY ADDED FROM FILE>\n";
}

void orlova::find(std::istream& in, std::ostream& out, const Dictionaries& dicts)
{
  std::string dictName;
  std::string englishWord;
  in >> dictName >> englishWord;
  if (!details::dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const auto& dict = dicts.at(dictName);
  auto it = dict.find(englishWord);
  if (it == dict.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  details::printTranslations(out, it->second);
}

void orlova::merge(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName1;
  std::string dictName2;
  std::string newDictName;
  in >> dictName1 >> dictName2 >> newDictName;
  if (!details::dictionaryExists(dicts, dictName1) || !details::dictionaryExists(dicts, dictName2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto& dict1 = dicts.at(dictName1);
  auto& dict2 = dicts.at(dictName2);
  if (dict1.empty() && dict2.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  Dictionary newDict;
  std::copy(dict1.begin(), dict1.end(), std::inserter(newDict, newDict.begin()));
  details::DictMerger merger(newDict);
  std::transform(dict2.begin(), dict2.end(), std::inserter(newDict, newDict.end()), merger);
  dicts[newDictName] = newDict;
  out << "<DICTIONARIES MERGED>\n";
}

void orlova::clear(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName;
  in >> dictName;
  if (!details::dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto& dict = dicts.at(dictName);
  if (dict.empty())
  {
    out << "<ALREADY EMPTY>\n";
    return;
  }
  dict.clear();
  out << "<DICTIONARY CLEARED>\n";
}

void orlova::removeWord(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName;
  std::string englishWord;
  in >> dictName >> englishWord;
  if (!details::dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto& dict = dicts.at(dictName);
  auto it = dict.find(englishWord);
  if (it == dict.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  dict.erase(it);
  out << "<WORD REMOVED>\n";
}

void orlova::append(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName;
  std::string englishWord;
  std::string newTranslation;
  in >> dictName >> englishWord >> newTranslation;
  if (!details::dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto& dict = dicts.at(dictName);
  auto it = dict.find(englishWord);
  if (it == dict.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Translations translation;
  translation.push_back(newTranslation);
  it->second = translation;
  out << "<TRANSLATION UPDATED>\n";
}

void orlova::dictionarySize(std::istream& in, std::ostream& out, const Dictionaries& dicts)
{
  std::string dictName;
  in >> dictName;
  if (!details::dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const auto& dict = dicts.at(dictName);
  out << dict.size();
}

void orlova::intersectionOfDicts(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName1;
  std::string dictName2;
  std::string newDictName;
  in >> dictName1 >> dictName2 >> newDictName;
  if (!details::dictionaryExists(dicts, dictName1) || !details::dictionaryExists(dicts, dictName2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dict1 = dicts.at(dictName1);
  auto dict2 = dicts.at(dictName2);
  if (dict1.empty() || dict2.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  Dictionary temp, newDict;
  details::CompareKeys comp;
  auto inserter = std::inserter(newDict, newDict.end());
  std::set_intersection(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), std::inserter(temp, temp.end()), comp);
  std::transform(temp.begin(), temp.end(), inserter, std::bind(details::intersectListDict, _1, dict2));
  dicts[newDictName] = newDict;
  out << "<SUCCESSFULLY INTERSECTED>";
}

void orlova::deleteDictionary(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName;
  in >> dictName;

  if (!details::dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  dicts.erase(dictName);
  out << "<DICTIONARY DELETED>\n";
}

void orlova::nonrepeatingWords(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName1;
  std::string dictName2;
  std::string newDictName;
  in >> dictName1 >> dictName2 >> newDictName;
  if (!details::dictionaryExists(dicts, dictName1) || !details::dictionaryExists(dicts, dictName2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (details::dictionaryExists(dicts, newDictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const auto& dict1 = dicts.at(dictName1);
  const auto& dict2 = dicts.at(dictName2);
  std::list< std::string > keys1, keys2, unique_keys;
  std::transform(dict1.begin(), dict1.end(), std::inserter(keys1, keys1.begin()), details::KeyExtractor{});
  std::transform(dict2.begin(), dict2.end(), std::inserter(keys2, keys2.begin()), details::KeyExtractor{});
  std::set_symmetric_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::inserter(unique_keys, unique_keys.begin()));
  if (unique_keys.empty())
  {
    out << "<DICTIONARIES ARE SIMILAR>\n";
    return;
  }
  Dictionary newDict;
  details::PairTransformer transformer(dict1, dict2);
  std::transform(unique_keys.begin(), unique_keys.end(), std::inserter(newDict, newDict.begin()), transformer);
  dicts[newDictName] = newDict;
  out << "<NONREPEATING WORDS COLLECTED>\n";
}

void orlova::residual(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName1;
  std::string dictName2;
  std::string newDictName;
  in >> dictName1 >> dictName2 >> newDictName;
  if (!details::dictionaryExists(dicts, dictName1) || !details::dictionaryExists(dicts, dictName2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (details::dictionaryExists(dicts, newDictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const auto& dict1 = dicts.at(dictName1);
  const auto& dict2 = dicts.at(dictName2);
  std::list< std::string > keys1, keys2, difference;
  std::transform(dict1.begin(), dict1.end(), std::inserter(keys1, keys1.begin()), details::KeyExtractor{});
  std::transform(dict2.begin(), dict2.end(), std::inserter(keys2, keys2.begin()), details::KeyExtractor{});
  std::set_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::inserter(difference, difference.begin()));
  if (difference.empty())
  {
    out << "<DICTIONARIES ARE SIMILAR>\n";
    return;
  }
  Dictionary newDict;
  details::ResidualTransform transformer(dict1);
  std::transform(difference.begin(), difference.end(), std::inserter(newDict, newDict.begin()), transformer);
  dicts[newDictName] = newDict;
  out << "<RESIDUAL DICTIONARY CREATED>\n";
}
