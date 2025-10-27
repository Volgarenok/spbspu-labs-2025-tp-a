#include "commands.hpp"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>

#include "utils.hpp"

namespace {
  constexpr char errorInvalidParams[] = "<INVALID PARAMS>";
  constexpr char errorDictionaryExists[] = "<DICTIONARY ALREADY EXISTS>";
  constexpr char errorInvalidDictionary[] = "<INVALID DICTIONARY>";
  constexpr char errorNoDictionary[] = "<NO DICTIONARIES>";
  constexpr char errorInvalidWord[] = "<INVALID WORD>";
  constexpr char errorNewWordExists[] = "<NEW WORD EXISTS>";
  constexpr char errorInvalidNumber[] = "<INVALID NUMBER>";
  constexpr char errorTranslationExists[] = "<TRANSLATION EXISTS>";
  constexpr char errorInvalidTranslation[] = "<INVALID TRANSLATION>";
  constexpr char errorRemovedLastTransloation[] = "<REMOVED LAST TRANSLATION. WORD REMOVED>";
}

namespace zholobov {

  void DictionaryCleaner(Dictionaries::value_type& entry)
  {
    entry.second.clear();
  }

  struct DictImporter {
    Dictionary& dictionary;
    size_t imported = 0;
    size_t dropped = 0;

    void operator()(const Dictionary::value_type& entry)
    {
      const Words& words = entry.second;
      if (words.empty()) {
        ++dropped;
        return;
      }
      auto result = dictionary.insert(entry);
      if (!result.second) {
        auto it = result.first;
        it->second.merge(Words{words});
        it->second.sort();
        it->second.unique();
      } else {
        ++imported;
      }
    }
  };

  struct DictionaryNamePrinter {
    std::ostream& out;
    void operator()(const Dictionaries::value_type& entry)
    {
      out << entry.first << "\n";
    }
  };

  struct DictionaryWordsPrinter {
    std::ostream& out;
    void operator()(const Dictionary::value_type& entry)
    {
      out << entry.first << "\n";
    }
  };

  struct DictionaryTranslationsPrinter {
    std::ostream& out;
    void operator()(const Dictionaries::value_type& entry)
    {
      out << entry.second;
    }
  };

  struct DictionaryWordTranslator {
    const Word& word;
    std::set< Word >& translations;
    void operator()(const Dictionaries::value_type& entry)
    {
      auto wit = entry.second.find(word);
      if (wit != entry.second.end()) {
        translations.insert(wit->second.begin(), wit->second.end());
      }
    }
  };

  struct IsDictionaryExists {
    const Dictionaries& dictionaries;
    bool operator()(const Word& name)
    {
      return (dictionaries.find(name) != dictionaries.end());
    }
  };

  struct UnionWordAndTranslations {
    std::map< Word, std::set< Word > >& temp;
    void operator()(const Dictionary::value_type& entry)
    {
      auto& it = temp[entry.first];
      it.insert(entry.second.cbegin(), entry.second.cend());
    }
  };

  struct DictionariesUnion {
    const Dictionaries& dictionaries;
    std::map< Word, std::set< Word > >& temp;
    void operator()(const Word& name)
    {
      const auto it = dictionaries.find(name);
      const Dictionary& dict = it->second;
      std::for_each(dict.cbegin(), dict.cend(), UnionWordAndTranslations{temp});
    }
  };

  struct WordsInserter {
    Dictionary& target;
    void operator()(const std::map< Word, std::set< Word > >::value_type& item)
    {
      target.insert(std::make_pair(
          item.first,
          std::list< Word >(item.second.begin(), item.second.end())));
    }
  };

  struct KeysInserter {
    std::set< Word >& keys;
    void operator()(const Dictionary::value_type& item)
    {
      keys.insert(item.first);
    }
  };

  struct KeysIntersector {
    std::set< Word >& keys;
    const Dictionaries& dictionaries;
    void operator()(const Word& dictName)
    {
      const auto it = dictionaries.find(dictName);
      const auto& dictionary = it->second;
      std::set< Word > newKeys;
      std::for_each(dictionary.cbegin(), dictionary.cend(), KeysInserter{newKeys});
      std::set< Word > unionKeys;
      std::set_intersection(keys.cbegin(), keys.cend(), newKeys.cbegin(), newKeys.cend(),
          std::inserter(unionKeys, unionKeys.end()));
      keys.swap(unionKeys);
    }
  };

  struct DictionariesIntersect {
    const Dictionaries& dictionaries;
    const std::set< Word >& commonKeys;
    std::map< Word, std::set< Word > >& temp;
    void operator()(const Word& name)
    {
      const auto it = dictionaries.find(name);
      const Dictionary& dict = it->second;
      std::for_each(commonKeys.cbegin(), commonKeys.cend(),
          std::bind(&DictionariesIntersect::process, this, std::placeholders::_1, dict));
    }

  private:
    void process(const Word& key, const Dictionary& dict)
    {
      const auto it = dict.find(key);
      if (it != dict.cend()) {
        auto& translations = temp[key];
        translations.insert(it->second.cbegin(), it->second.cend());
      }
    }
  };

  struct WordFrequencyCalculator {
    const Dictionaries& dictionaries;
    std::map< Word, size_t >& wordFrequency;

    void operator()(const Word& dictName)
    {
      const auto& dict = dictionaries.at(dictName);
      for_each(dict.cbegin(), dict.cend(),
          std::bind(&WordFrequencyCalculator::incrementWordFrequency, this, std::placeholders::_1));
    }

  private:
    void incrementWordFrequency(const Dictionary::value_type& item)
    {
      wordFrequency[item.first]++;
    }
  };

  struct CollectRareWordsLessThanN {
    const Dictionaries& dictionaries;
    const std::map< Word, size_t >& wordFrequency;
    std::map< Word, std::set< Word > >& target;
    const size_t n;

    void operator()(const Word& dictName)
    {
      const auto& dict = dictionaries.at(dictName);
      std::for_each(dict.cbegin(), dict.cend(),
          std::bind(&CollectRareWordsLessThanN::process, this, std::placeholders::_1));
    }

  private:
    void process(const Dictionary::value_type& item)
    {
      if (wordFrequency.at(item.first) <= n) {
        std::set< Word >& s = target[item.first];
        s.insert(item.second.begin(), item.second.end());
      }
    }
  };

}

void zholobov::printHelp(std::ostream& out)
{
  out << "Commands:\n";
  out << "dict-create <dict>\n";
  out << "dict-remove <dict>\n";
  out << "dict-import <dict> <file>\n";
  out << "dict-export <dict> <file> [overwrite]\n";
  out << "dict-count\n";
  out << "dict-list\n";
  out << "dict-print-words <dict>\n";
  out << "dict-print-translations [<dict>]\n";
  out << "dict-clear [<dict>]\n";
  out << "add-word <dict> <word> <translation>\n";
  out << "remove-word <dict> <word>\n";
  out << "count-words <dict>\n";
  out << "add-translation <dict> <word> <translation>\n";
  out << "remove-translation <dict> <word> <translation>\n";
  out << "change-word <dict> <word> <new_word>\n";
  out << "translate-word <word>\n";
  out << "union <new_dict> <dict-1> [<dict-N> ...]\n";
  out << "intersect <new_dict> <dict-1> [<dict-N> ...]\n";
  out << "rare <N> <new_dict> <dict-1> [<dict-K> ...]\n";
}

void zholobov::cmdDictCreate(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 1) {
    out << errorInvalidParams << "\n";
    return;
  }
  const std::string& name = args.front();
  if (dictionaries.count(name)) {
    out << errorDictionaryExists << "\n";
  } else {
    dictionaries[name] = Dictionary();
  }
}

void zholobov::cmdDictRemove(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 1) {
    out << errorInvalidParams << "\n";
    return;
  }
  const std::string& name = args.front();
  if (!dictionaries.count(name)) {
    out << errorInvalidDictionary << "\n";
  } else {
    dictionaries.erase(name);
  }
}

void zholobov::cmdDictImport(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 2) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto argsIt = args.cbegin();
  const std::string& dictName = *argsIt++;
  const std::string& fileName = *argsIt;

  Dictionary& dict = dictionaries[dictName];

  std::ifstream fin(fileName);
  if (!fin) {
    out << "<0 TRANSLATIONS IMPORTED>.\n";
    return;
  }

  Dictionary importedDict;
  fin >> importedDict;

  DictImporter dictImporter{dict};
  std::for_each(importedDict.begin(), importedDict.end(), std::ref(dictImporter));

  out << "<" << dictImporter.imported << " TRANSLATIONS IMPORTED>.";
  if (dictImporter.dropped != 0) {
    out << " [" << dictImporter.dropped << " - DROPPED]";
  }
  out << "\n";
}

void zholobov::cmdDictExport(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (!(args.size() == 2 || args.size() == 3)) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto argsIt = args.cbegin();
  const std::string& dictName = *argsIt++;
  const std::string& fileName = *argsIt++;
  bool overwrite = ((argsIt != args.cend()) && (*argsIt == "overwrite"));

  auto it = dictionaries.find(dictName);
  if (it == dictionaries.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }

  std::ifstream check(fileName);
  if (check && !overwrite) {
    out << "FILE EXISTS\n";
    return;
  }

  std::ofstream fout(fileName, std::ios::trunc);
  if (!fout) {
    return;
  }
  fout << it->second;
}

void zholobov::cmdDictCount(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 0) {
    out << errorInvalidParams << "\n";
    return;
  }
  out << dictionaries.size() << '\n';
}

void zholobov::cmdDictList(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 0) {
    out << errorInvalidParams << "\n";
    return;
  }
  if (dictionaries.size() == 0) {
    out << errorNoDictionary << "\n";
  }
  DictionaryNamePrinter printer{out};
  for_each(dictionaries.begin(), dictionaries.end(), printer);
}

void zholobov::cmdDictPrintWords(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 1) {
    out << errorInvalidParams << "\n";
    return;
  }
  const std::string& name = args.front();
  auto it = dictionaries.find(name);
  if (it == dictionaries.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }

  std::for_each(it->second.begin(), it->second.end(), DictionaryWordsPrinter{out});
}

void zholobov::cmdDictPrintTranslations(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (!(args.size() == 0 || args.size() == 1)) {
    out << errorInvalidParams << "\n";
    return;
  }
  if (args.size() == 1) {
    auto it = dictionaries.find(args.front());
    if (it == dictionaries.end()) {
      out << errorInvalidDictionary << "\n";
      return;
    }
    out << it->second;
  } else {
    if (dictionaries.empty()) {
      out << errorNoDictionary << "\n";
      return;
    }

    std::for_each(dictionaries.begin(), dictionaries.end(), DictionaryTranslationsPrinter{out});
  }
}

void zholobov::cmdDictClear(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (!(args.size() == 0 || args.size() == 1)) {
    out << errorInvalidParams << "\n";
    return;
  }
  if (args.size() == 1) {
    auto it = dictionaries.find(args.front());
    if (it == dictionaries.end()) {
      out << errorInvalidDictionary << "\n";
      return;
    }
    it->second.clear();
  } else {
    std::for_each(dictionaries.begin(), dictionaries.end(), DictionaryCleaner);
  }
}

void zholobov::cmdAddWord(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 3) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto argsIt = args.cbegin();
  const std::string& name = *argsIt++;
  auto it = dictionaries.find(name);
  if (it == dictionaries.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }
  const std::string& word = *argsIt++;
  const std::string& translation = *argsIt;
  it->second[word].push_back(translation);
}

void zholobov::cmdRemoveWord(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 2) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto argsIt = args.cbegin();
  const std::string& name = *argsIt++;
  auto it = dictionaries.find(name);
  if (it == dictionaries.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }
  const std::string& word = *argsIt;
  if (!it->second.erase(word)) {
    out << errorInvalidWord << "\n";
  }
}

void zholobov::cmdCountWords(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 1) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto it = dictionaries.find(args.front());
  if (it == dictionaries.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }
  out << it->second.size() << '\n';
}

void zholobov::cmdAddTranslation(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 3) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto argsIt = args.cbegin();
  const std::string& name = *argsIt++;
  auto dit = dictionaries.find(name);
  if (dit == dictionaries.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }
  const std::string& word = *argsIt++;
  auto wit = dit->second.find(word);
  if (wit == dit->second.end()) {
    out << errorInvalidWord << "\n";
    return;
  }

  const std::string& translation = *argsIt;

  if (std::find(wit->second.begin(), wit->second.end(), translation) != wit->second.end()) {
    out << errorTranslationExists << "\n";
    return;
  }
  wit->second.push_back(translation);
}

void zholobov::cmdRemoveTranslation(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 3) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto argsIt = args.cbegin();
  const std::string& name = *argsIt++;
  auto dit = dictionaries.find(name);
  if (dit == dictionaries.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }
  const std::string& word = *argsIt++;
  auto wit = dit->second.find(word);
  if (wit == dit->second.end()) {
    out << errorInvalidWord << "\n";
    return;
  }
  const std::string& translation = *argsIt;
  auto& lst = wit->second;
  auto lit = std::find(lst.begin(), lst.end(), translation);
  if (lit == lst.end()) {
    out << errorInvalidTranslation << "\n";
    return;
  }
  lst.erase(lit);
  if (lst.empty()) {
    dit->second.erase(wit);
    out << errorRemovedLastTransloation << "\n";
  }
}

void zholobov::cmdChangeWord(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 3) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto argsIt = args.cbegin();
  const std::string& name = *argsIt++;
  auto dit = dictionaries.find(name);
  if (dit == dictionaries.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }
  const std::string& word = *argsIt++;
  auto wit = dit->second.find(word);
  if (wit == dit->second.end()) {
    out << errorInvalidWord << "\n";
    return;
  }
  const std::string& newWord = *argsIt;
  if (dit->second.count(newWord)) {
    out << errorNewWordExists << "\n";
    return;
  }
  dit->second[newWord] = wit->second;
  dit->second.erase(wit);
}

void zholobov::cmdTranslateWord(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() != 1) {
    out << errorInvalidParams << "\n";
    return;
  }
  const std::string& word = args.front();
  std::set< Word > translations;

  std::for_each(dictionaries.begin(), dictionaries.end(), DictionaryWordTranslator{word, translations});

  if (translations.empty()) {
    out << errorInvalidWord << "\n";
    return;
  }

  Words translationsWords(translations.cbegin(), translations.cend());
  out << translationsWords << '\n';
}

void zholobov::cmdUnion(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() < 2) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto argsIt = args.cbegin();
  const std::string& newName = *argsIt++;

  if (std::find_if_not(argsIt, args.cend(), IsDictionaryExists{dictionaries}) != args.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }

  std::map< Word, std::set< Word > > temp;
  std::for_each(argsIt, args.cend(), DictionariesUnion{dictionaries, temp});

  Dictionary result;
  std::for_each(temp.cbegin(), temp.cend(), WordsInserter{result});

  dictionaries[newName] = std::move(result);
}

void zholobov::cmdIntersect(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() < 2) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto argsIt = args.cbegin();
  const std::string& newName = *argsIt++;

  if (std::find_if_not(argsIt, args.cend(), IsDictionaryExists{dictionaries}) != args.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }

  const std::string& firstDictName = *argsIt++;
  const Dictionary& firstDict = dictionaries.find(firstDictName)->second;
  std::set< Word > commonKeys;
  std::for_each(firstDict.cbegin(), firstDict.cend(), KeysInserter{commonKeys});
  std::for_each(argsIt, args.cend(), KeysIntersector{commonKeys, dictionaries});

  std::map< Word, std::set< Word > > temp;
  std::for_each(argsIt, args.cend(), DictionariesIntersect{dictionaries, commonKeys, temp});

  Dictionary result;
  std::for_each(temp.cbegin(), temp.cend(), WordsInserter{result});

  dictionaries[newName] = std::move(result);
}

void zholobov::cmdRare(std::istream& in, std::ostream& out, Dictionaries& dictionaries)
{
  Words args;
  in >> args;
  if (args.size() < 3) {
    out << errorInvalidParams << "\n";
    return;
  }
  auto argsIt = args.cbegin();
  const std::string& strN = *argsIt++;

  int n = 0;
  try {
    n = std::stoi(strN);
  } catch (...) {
    out << errorInvalidNumber << "\n";
    return;
  }

  if (n < 1) {
    out << errorInvalidNumber << "\n";
    return;
  }

  const std::string& newName = *argsIt++;

  if (std::find_if_not(argsIt, args.cend(), IsDictionaryExists{dictionaries}) != args.end()) {
    out << errorInvalidDictionary << "\n";
    return;
  }

  std::map< Word, size_t > wordFrequency;
  std::for_each(argsIt, args.cend(), WordFrequencyCalculator{dictionaries, wordFrequency});

  std::map< Word, std::set< Word > > temp;
  std::for_each(argsIt, args.cend(),
      CollectRareWordsLessThanN{dictionaries, wordFrequency, temp, static_cast< size_t >(n)});

  Dictionary result;
  std::for_each(temp.cbegin(), temp.cend(), WordsInserter{result});

  dictionaries[newName] = std::move(result);
}
