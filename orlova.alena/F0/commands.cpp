#include "commands.h"
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>

namespace
{
  std::string readWord(std::istream& in)
  {
    std::string word;
    in >> word;
    return word;
  }

  std::string readLine(std::istream& in)
  {
    std::string line;
    std::getline(in, line);
    return line;
  }

  bool dictionaryExists(const orlova::Dictionaries& dicts, const std::string& name)
  {
    return dicts.find(name) != dicts.end();
  }

  template <typename T>
  void printTranslations(std::ostream& out, const orlova::Translation& translations)
  {
    if (translations.empty())
    {
      return;
    }

    auto it = translations.begin();
    out << *it;
    std::advance(it, 1);

    struct TranslationTransform
    {
      std::string& operator()(const std::string& trans)
      {
        std::string{ out << ", ";
        return trans;
        }
      }
    };

    std::transform(it, translations.end(), std::ostream_iterator<std::string>(out, ""), TranslationTransformer{});
  }
}

void orlova::addEmptyDictionary(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string name = readWord(in);
  if (!in || dicts.find(name) != dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  dicts[name] = Dictionary();
}

void orlova::addDictionary(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName = readWord(in);
  std::string fileName = readWord(in);

  if (dictionaryExists(dicts, dictName))
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
  std::string englishWord, russianWord;

  while (file >> englishWord >> russianWord)
  {
    newDict[englishWord].push_back(russianWord);
  }

  dicts[dictName] = newDict;
  file.close();
}

void orlova::find(std::istream& in, std::ostream& out, const Dictionaries& dicts)
{
  std::string dictName = readWord(in);
  std::string englishWord = readWord(in);

  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const auto& dict = dicts.at(dictName);
  auto it = dict.find(englishWord);
  if (it == dict.end())
  {
    out << "<INVALID COMMAND>\n";
  }

  printTranslations(out, it->second);
}

template < typename Pair >
void orlova::merge(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dict1Name = readWord(in);
  std::string dict2Name = readWord(in);
  std::string newDictName = readWord(in);

  if (!dictionaryExists(dicts, dict1Name) || !dictionaryExists(dicts, dict2Name))
  {
    out << "<INVALID COMMAND>\n";
  }

  auto it = dicts.find(dict1Name);
  auto& dict1 = it->second;
  auto it = dicts.find(dict2Name);
  auto& dict2 = it->second;

  if (dict1.empty() && dict2.empty())
  {
    out << "<EMPTY>\n";
  }

  Dictionary newDict;

  struct DictTransformer
  {
    Pair operator()(const Pair& p) const
    {
      return p;
    }
  };

  std::transform(dict1.begin(), dict1.end(), std::inserter(newDict, newDict.end()), DictTransformer());

  struct DictMerger
  {
    Dictionary* newDict;
    std::pair<std::string, Translation> operator()(const std::pair<std::string, Translation>& pair) const
    {
      auto it = newDict->find(pair.first);
      if (it != newDict->end())
      {
        it->second.insert(it->second.end(), pair.second.begin(), pair.second.end());
        return *it;
      }
      else
      {
        (*newDict)[pair.first] = pair.second;
        return { pair.first, pair.second };
      }
    }
  };

  dicts[newDictName] = newDict;
}

void orlova::clear(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName = readWord(in);

  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(dictName);
  auto& dict = it->second;
  if (dict.empty())
  {
    out << "<ALREADY EMPTY>\n";
    return;
  }

  dict.clear();
}

void orlova::removeWord(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName = readWord(in);
  std::string englishWord = readWord(in);

  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  auto& dict = dictIt->second;
  auto it = dict.find(englishWord);
  if (it == dict.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  dict.erase(it);
}

void orlova::append(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName = readWord(in);
  std::string englishWord = readWord(in);
  std::string newTranslation = readWord(in);

  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  auto& dict = dictIt->second;
  auto it = dict.find(englishWord);
  if (it == dict.end())
  {
    out << "<INVALID COMMAND>\n";
  }
  it->second.clear();
  it->second.push_back(newTranslation);
}

void orlova::dictionarySize(std::istream& in, std::ostream& out, const Dictionaries& dicts)
{
  std::string dictName = readWord(in);

  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const auto& dict = dicts.at(dictName);
  out << dict.size();
}

template < typename Pair >
void orlova::intersectionOfDicts(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dict1Name = readWord(in);
  std::string dict2Name = readWord(in);
  std::string newDictName = readWord(in);

  if (!dictionaryExists(dicts, dict1Name) || !dictionaryExists(dicts, dict2Name))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  const auto& dict1 = it1->second;
  const auto& dict2 = it2->second;

  if (dict1.empty() || dict2.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  Dictionary newDict;

  std::set< std::string > keys1, keys2, intersection;
  std::transform(dict1.begin(), dict1.end(), std::inserter(keys1, keys1.end()), DictTransformer{});
  std::transform(dict2.begin(), dict2.end(), std::inserter(keys2, keys2.end()), DictTransformer{});
  std::set_intersection(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(),
  std::inserter(intersection, intersection.end()));

  struct DictIntersection
  {
    Pair operator()(const std::string& key) const
    {
      std::pair<std::string, Translation>
      {
        Translation combined = dict1.at(key);
        combined.push_back(dict2.at(key).begin(), dict2.at(key).end());
        return { key, combined };
      }
    }
  };

  std::transform(intersection.begin(), intersection.end(), std::inserter(*newDict, newDict->end()), DictIntersection{});
  dicts[newDictName] = newDict;
}

void orlova::delete_(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName = readWord(in);

  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  dicts.erase(dictName);
}

template < typename Pair >
void orlova::nonrepeatingWords(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dict1Name = readWord(in);
  std::string dict2Name = readWord(in);
  std::string newDictName = readWord(in);

  if (!dictionaryExists(dicts, dict1Name) || !dictionaryExists(dicts, dict2Name) || dictionaryExists(dicts, newDictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  const auto& dict1 = it1->second;
  const auto& dict2 = it2->second;

  struct DictTransformer
  {
    Pair operator()(const Pair& p) const
    {
      return p;
    }
  };

  std::set<std::string> keys1, keys2, unique_keys;
  std::transform(dict1.begin(), dict1.end(), std::inserter(keys1, keys1.end()), DictTransformer{});
  std::transform(dict2.begin(), dict2.end(), std::inserter(keys2, keys2.end()), DictTransformer{});

  std::set_symmetric_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::inserter(unique_keys, unique_keys.end()));

  if (unique_keys.empty())
  {
    out << "<DICTIONARIES ARE SIMILAR>\n";
    return;
  }

  Dictionary newDict;

  struct PairTransformer
  {
    Pair operator()(const std::string& key)
    {
      std::pair< std::string, Translation >
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
    }
  };

  std::transform(unique_keys.begin(), unique_keys.end(), std::inserter(*newDict, newDict->end()), PairTransformer{});

  dicts[newDictName] = newDict;
}

template < typename Pair >
void orlova::residual(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dict1Name = readWord(in);
  std::string dict2Name = readWord(in);
  std::string newDictName = readWord(in);

  if (!dictionaryExists(dicts, dict1Name) || !dictionaryExists(dicts, dict2Name) || dictionaryExists(dicts, newDictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  const auto& dict1 = it1->second;
  const auto& dict2 = it2->second;

  struct DictTransformer
  {
    Pair operator()(const Pair& p) const
    {
      return p;
    }
  };

  std::set<std::string> keys1, keys2, difference;
  std::transform(dict1.begin(), dict1.end(), std::inserter(keys1, keys1.end()), DictTransformer{});
  std::transform(dict2.begin(), dict2.end(), std::inserter(keys2, keys2.end()), DictTransformer{});
  std::set_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::inserter(difference, difference.end()));

  if (difference.empty())
  {
    out << "<DICTIONARIES ARE SIMILAR>\n";
    return;
  }

  Dictionary newDict;;

  struct ResidualTransform
  {
    Pair operator()(const std::string& key) const
    {
      std::pair<std::string, Translation>
      {
        return { key, dict1.at(key) };
      }
    }
  };

  std::transform(difference.begin(), difference.end(), std::inserter(*newDict, newDict->end()), ResidualTransform{});

  dicts[newDictName] = newDict;
}
