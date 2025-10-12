#include "commands.h"
#include <cctype>
#include <fstream>
#include <iterator>
#include <numeric>
#include <vector>

namespace
{
  using namespace kharlamov;
  struct DictEntryFormatter
  {
    std::string operator()(const std::pair<const std::string, std::string>& entry) const
    {
      return entry.first + " - " + entry.second;
    }
  };

  struct PrefixMatcher
  {
    std::string prefix;
    bool operator()(const std::pair<std::string, std::string>& entry) const
    {
      return entry.first.find(prefix) == 0;
    }
  };

  struct PairComparator
  {
    bool operator()(const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) const
    {
      return a.first < b.first;
    }
  };

  struct EntryFormatter
  {
    std::string operator()(const std::pair<std::string, std::string>& entry) const
    {
      return entry.first + " - " + entry.second;
    }
  };

  struct PatternMatcher
  {
    std::string pattern;

    bool operator()(const std::pair<std::string, std::string>& entry) const
    {
      return isMatch(entry.first, 0, pattern, 0);
    }

    bool isMatch(const std::string& word, size_t wordPos, const std::string& pat, size_t patPos) const
    {
      if (patPos == pat.length())
      {
        return wordPos == word.length();
      }

      if (pat[patPos] == '*')
      {
        return isMatch(word, wordPos, pat, patPos + 1) || (wordPos < word.length() && isMatch(word, wordPos + 1, pat, patPos));
      }

      if (wordPos < word.length() && (pat[patPos] == '?' || pat[patPos] == word[wordPos]))
      {
        return isMatch(word, wordPos + 1, pat, patPos + 1);
      }

      return false;
    }
  };

  struct PairSorter
  {
    bool operator()(const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) const
    {
      return a.first < b.first;
    }
  };

  struct KeyExtractor
  {
    std::string operator()(const std::pair<std::string, dictionary>& item) const
    {
      return item.first;
    }
  };

  struct DictIdComparator
  {
    bool operator()(const std::string& a, const std::string& b) const
    {
      return std::stoi(a) < std::stoi(b);
    }
  };

  struct DictInfoFormatter
  {
    const dictionaries& dicts;

    std::string operator()(const std::string& id) const
    {
      return id + ": " + std::to_string(dicts.at(id).size()) + " words";
    }
  };

  struct WordPairComparator
  {
    bool operator()(const std::pair<const std::string, std::string>& a, const std::pair<const std::string, std::string>& b) const
    {
      return a.first < b.first;
    }
  };

  struct PairEqual
  {
    bool operator()(const std::pair<const std::string, std::string>& a, const std::pair<const std::string, std::string>& b) const
    {
      return a.first == b.first && a.second == b.second;
    }
  };

  struct ResultsCollector
  {
    std::vector<std::pair<std::string, std::pair<std::string, std::string>>>& results;
    const std::string& pattern;

    void operator()(const dictionaries::value_type& dictPair) const
    {
      const dictionary& dict = dictPair.second;
      PatternMatcher matcher{ pattern };

      dictionary::const_iterator it = dict.begin();
      dictionary::const_iterator end = dict.end();

      if (it == end) return;

      if (matcher(*it))
      {
        results.emplace_back(dictPair.first, *it);
      }
      operator()(dictionaries::value_type(dictPair.first, dictionary(++it, end)));
    }
  };

  struct ResultFormatter
  {
    std::string operator()(const std::pair<std::string, std::pair<std::string, std::string>>& item) const
    {
      return "[" + item.first + "] " + item.second.first + " - " + item.second.second;
    }
  };

  struct TotalCounter
  {
    size_t operator()(size_t total, const dictionaries::value_type& dict) const
    {
      return total + dict.second.size();
    }
  };

  struct DictToString
  {
    std::string operator()(const dictionaries::value_type& dict) const
    {
      return dict.first + ": " + std::to_string(dict.second.size());
    }
  };

  struct ConflictChecker
  {
    const dictionary& dict1;
    explicit ConflictChecker(const dictionary& dict) : dict1(dict) {}

    bool operator()(const std::pair<const std::string, std::string>& wordPair) const
    {
      auto it = dict1.find(wordPair.first);
      return it != dict1.end() && it->second != wordPair.second;
    }
  };
}

void kharlamov::doDownload(std::istream& in, std::ostream& out, dictionaries& dicts)
{
  std::string dictId;
  std::string filename;

  if (!(in >> dictId >> filename))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (dicts.find(dictId) == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  std::ifstream file(filename);
  if (!file)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  dictionary newDict;
  std::string word;
  std::string separator;
  std::string translation;

  while (file >> word >> separator >> translation)
  {
    if (separator != "-")
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    newDict[word] = translation;

    while (file.peek() != '\n' && file.peek() != EOF)
    {
      file.get();
    }
    if (file.peek() == '\n') file.get();
  }

  if (newDict.empty() || !file.eof())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  dicts[dictId] = newDict;
  out << "OK\n";
}

void kharlamov::doSave(std::istream& in, std::ostream& out, const dictionaries& dicts)
{
  std::string dictId;
  std::string filename;

  if (!(in >> dictId >> filename))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dictIt = dicts.find(dictId);
  if (dictIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  std::ofstream file(filename);
  if (!file)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const dictionary& dict = dictIt->second;
  std::transform(dict.begin(), dict.end(), std::ostream_iterator<std::string>(file, "\n"), DictEntryFormatter());

  if (!file)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void kharlamov::doAdd(std::istream& in, std::ostream& out, dictionaries& dicts)
{
  std::string dictId;
  std::string word;
  std::string translation;

  if (!(in >> dictId >> word))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  char firstChar;
  in.get(firstChar);
  if (firstChar == ' ' && in.peek() == '"')
  {
    in.get();
    std::getline(in, translation, '"');
  }
  else
  {
    in.unget();
    if (!(in >> translation))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
  }

  auto dictIt = dicts.find(dictId);
  if (dictIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  dictionary& dict = dictIt->second;
  if (dict.find(word) != dict.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  dict[word] = translation;
  out << "OK\n";
}

void kharlamov::doRemove(std::istream& in, std::ostream& out, dictionaries& dicts)
{
  std::string dictId;
  std::string word;

  if (!(in >> dictId >> word))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dictIt = dicts.find(dictId);
  if (dictIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  dictionary& dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  dict.erase(wordIt);
  out << "OK\n";
}

void kharlamov::doTranslate(std::istream& in, std::ostream& out, const dictionaries& dicts)
{
  std::string dictId;
  std::string word;

  if (!(in >> dictId >> word))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dictIt = dicts.find(dictId);
  if (dictIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  const dictionary& dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    out << "<NO TRANSLATION>\n";
    return;
  }

  out << wordIt->second << '\n';
}

void kharlamov::doFind(std::istream& in, std::ostream& out, const dictionaries& dicts)
{
  std::string dictId;
  std::string prefix;

  if (!(in >> dictId >> prefix))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dictIt = dicts.find(dictId);
  if (dictIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  const dictionary& dict = dictIt->second;
  std::vector<std::pair<std::string, std::string>> matches;

  PrefixMatcher matcher{ prefix };
  std::copy_if(dict.begin(), dict.end(), std::back_inserter(matches), matcher);


  if (matches.empty())
  {
    out << "<NO MATCHES>\n";
    return;
  }

  PairComparator comp;
  std::sort(matches.begin(), matches.end(), comp);

  EntryFormatter formatter;
  std::transform(matches.begin(), matches.end(), std::ostream_iterator<std::string>(out, "\n"), formatter);
}

void kharlamov::doSearch(std::istream& in, std::ostream& out, const dictionaries& dicts)
{
  std::string dictId;
  std::string pattern;

  if (!(in >> dictId >> pattern))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dictIt = dicts.find(dictId);
  if (dictIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  const dictionary& dict = dictIt->second;
  std::vector<std::pair<std::string, std::string>> matches;

  PatternMatcher matcher;
  matcher.pattern = pattern;
  std::copy_if(dict.begin(), dict.end(), std::back_inserter(matches), matcher);

  if (matches.empty())
  {
    out << "<NO MATCHES>\n";
    return;
  }

  PairSorter sorter;
  std::sort(matches.begin(), matches.end(), sorter);

  EntryFormatter formatter;
  std::transform(matches.begin(), matches.end(), std::ostream_iterator<std::string>(out, "\n"), formatter);
}

void kharlamov::doCount(std::istream& in, std::ostream& out, const dictionaries& dicts)
{
  std::string dictId;

  if (!(in >> dictId))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dictIt = dicts.find(dictId);
  if (dictIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  size_t wordCount = dictIt->second.size();

  out << "Words: " << wordCount << '\n';
}

void kharlamov::doList(std::istream& in, std::ostream& out, const dictionaries& dicts)
{
  std::string dictId;

  if (!(in >> dictId))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dictIt = dicts.find(dictId);
  if (dictIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  const dictionary& dict = dictIt->second;
  if (dict.empty())
  {
    out << "<EMPTY DICTIONARY>\n";
    return;
  }

  EntryFormatter formatter;
  std::transform(dict.begin(), dict.end(), std::ostream_iterator<std::string>(out, "\n"), formatter);
}

void kharlamov::doClear(std::istream& in, std::ostream& out, dictionaries& dicts)
{
  std::string dictId;

  if (!(in >> dictId))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dictIt = dicts.find(dictId);
  if (dictIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  dictIt->second.clear();

  out << "Dictionary " << dictId << " cleared\n";
}

void kharlamov::doCreatedict(std::ostream& out, dictionaries& dicts)
{
  static size_t nextId = 0;
  size_t newId = nextId++;
  dicts[std::to_string(newId)] = dictionary();
  out << "Dictionary " << newId << " created\n";
}

void kharlamov::doListdicts(std::ostream& out, const dictionaries& dicts)
{
  if (dicts.empty())
  {
    out << "No dictionaries available\n";
    return;
  }

  std::vector<std::string> dictIds;
  KeyExtractor extractor;
  std::transform(dicts.begin(), dicts.end(), std::back_inserter(dictIds), extractor);

  DictIdComparator comparator;
  std::sort(dictIds.begin(), dictIds.end(), comparator);

  DictInfoFormatter formatter{ dicts };
  std::transform(dictIds.begin(), dictIds.end(), std::ostream_iterator<std::string>(out, "\n"), formatter);
}

void kharlamov::doCopy(std::istream& in, std::ostream& out, dictionaries& dicts)
{
  std::string fromId, toId, word;

  if (!(in >> fromId >> toId >> word))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto fromIt = dicts.find(fromId);
  if (fromIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  auto toIt = dicts.find(toId);
  if (toIt == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  const dictionary& fromDict = fromIt->second;
  auto wordIt = fromDict.find(word);
  if (wordIt == fromDict.end())
  {
    out << "<NO TRANSLATION>\n";
    return;
  }

  dictionary& toDict = toIt->second;
  if (toDict.find(word) != toDict.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  toDict[word] = wordIt->second;
  out << "OK\n";
}

void kharlamov::doMerge(std::istream& in, std::ostream& out, dictionaries& dicts)
{
  std::string newId, dictId1, dictId2;

  if (!(in >> newId >> dictId1 >> dictId2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dict1It = dicts.find(dictId1);
  auto dict2It = dicts.find(dictId2);
  if (dict1It == dicts.end() || dict2It == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  const dictionary& dict1 = dict1It->second;
  const dictionary& dict2 = dict2It->second;

  bool conflictExists = std::any_of(dict2.begin(), dict2.end(), ConflictChecker(dict1)
  );

  if (conflictExists)
  {
    out << "<CONFLICT>\n";
    return;
  }

  dictionary mergedDict = dict1;
  mergedDict.insert(dict2.begin(), dict2.end());

  dicts[newId] = std::move(mergedDict);
  out << "OK\n";
}

void kharlamov::doDiff(std::istream& in, std::ostream& out, dictionaries& dicts)
{
  std::string newId, dictId1, dictId2;

  if (!(in >> newId >> dictId1 >> dictId2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dict1It = dicts.find(dictId1);
  auto dict2It = dicts.find(dictId2);
  if (dict1It == dicts.end() || dict2It == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  dictionary newDict;
  const dictionary& dict1 = dict1It->second;
  const dictionary& dict2 = dict2It->second;

  WordPairComparator comp;
  std::set_difference(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), std::inserter(newDict, newDict.begin()), comp);

  if (newDict.empty())
  {
    out << "<EMPTY DIFF>\n";
    return;
  }

  dicts[newId] = std::move(newDict);
  out << "OK\n";
}

void kharlamov::doIntersect(std::istream& in, std::ostream& out, dictionaries& dicts)
{
  std::string newId, dictId1, dictId2;

  if (!(in >> newId >> dictId1 >> dictId2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dict1It = dicts.find(dictId1);
  auto dict2It = dicts.find(dictId2);
  if (dict1It == dicts.end() || dict2It == dicts.end())
  {
    out << "<INVALID DICT>\n";
    return;
  }

  dictionary intersection;
  const dictionary& dict1 = dict1It->second;
  const dictionary& dict2 = dict2It->second;

  PairComparator comp;
  std::set_intersection(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), std::inserter(intersection, intersection.begin()),comp);

  if (intersection.empty())
  {
    out << "<EMPTY INTERSECT>\n";
    return;
  }

  dicts[newId] = std::move(intersection);
  out << "OK\n";
}

void kharlamov::doGlobalsearch(std::istream& in, std::ostream& out, const dictionaries& dicts)
{
  std::string pattern;

  if (!(in >> pattern))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::vector<std::pair<std::string, std::pair<std::string, std::string>>> results;

  if (!dicts.empty())
  {
    dictionaries::const_iterator it = dicts.begin();
    ResultsCollector collector{ results, pattern };
    collector(*it);
  }

  if (results.empty())
  {
    out << "<NO MATCHES>\n";
    return;
  }

  std::sort(results.begin(), results.end());
  ResultFormatter formatter;
  std::transform(results.begin(), results.end(), std::ostream_iterator<std::string>(out, "\n"), formatter);
}

void kharlamov::doGlobalcount(std::ostream& out, const dictionaries& dicts)
{
  size_t total = std::accumulate(dicts.begin(), dicts.end(), size_t(0), TotalCounter());
  out << "Total words: " << total << "\n";
  out << "Per dictionary:\n";

  DictToString transformer;
  std::transform(dicts.begin(), dicts.end(), std::ostream_iterator<std::string>(out, "\n"), transformer);
}
