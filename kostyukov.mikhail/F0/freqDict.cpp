#include "freqDict.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "functors.hpp"
#include "scopeGuard.hpp"

namespace kostyukov
{
  void printHelp(std::ostream& out)
  {
    out << "Available commands:\n";
    out << "  create <dict_name>          - Creates a new empty dictionary.\n";
    out << "  load <dict_name> <filename> - Loads text from file into a dictionary.\n";
    out << "  getfreq <dict_name> <word>  - Shows relative frequency of a word.\n";
    out << "  top <dict_name> <N>         - Shows N most frequent words.\n";
    out << "  bottom <dict_name> <N>      - Shows N least frequent words.\n";
    out << "  listdicts                   - Shows names of all created dictionaries.\n";
    out << "  delete <dict_name>          - Deletes an existing dictionary.\n";
    out << "  merge <new> <d1> <d2>       - Merges d1 and d2 into a new dictionary.\n";
    out << "  intersect <new> <d1> <d2> <p> - Intersects d1 and d2, where p is 'min' or 'max' for counts.\n";
    out << "  clear <dict_name>           - Clears all words from a dictionary.\n";
    out << "  findsame <d1> <d2>          - Finds common words in two dictionaries.\n";
    out << "  finduniq <d1> <d2>          - Finds unique words in two dictionaries.\n";
    out << "  removebatch <d> <p> <N>   - Removes words where p is 'freq_less' or 'freq_more' than N%.\n";
    out << "  rangefreq <d> <min> <max> - Shows words with frequency in [min, max] %.\n";
    out << "  outrangefreq <d> <min> <max> - Shows words with frequency outside [min, max] %.\n";
    return;
  }

  bool isValidName(const std::string& name)
  {
    if (name.empty() || !std::isalpha(name[0]))
    {
      return false;
    }
    return std::find_if(name.begin(), name.end(), IsInvalidChar{}) == name.end();
  }

  void createDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dictName;
    in >> dictName;
    if (!in || dictName.empty())
    {
      out << "<MISSING ARGUMENTS>\n";
      return;
    }
    if(!isValidName(dictName))
    {
      out << "INVALID DICTIONARY NAME>\n";
      return;
    }
    if (dicts.count(dictName))
    {
      out << "<DICTIONARY EXISTS>\n";
      return;
    }
    dicts.emplace(dictName, FrequencyDictionary{});
    out << "successfully created " << dictName << '\n';
  }

  void loadDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dictName;
    std::string filename;
    in >> dictName >> filename;
    if (!in || dictName.empty() || filename.empty())
    {
      out << "<MISSING ARGUMENTS>\n";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>\n";
      return;
    }
    std::fstream file(filename);
    if (!file)
    {
      out << "<FILE ERROR>\n";
      return;
    }
    FrequencyDictionary& dict = dicts[dictName];
    dict.counts.clear();
    dict.totalWords = 0;
    std::istream_iterator< std::string > start(file);
    std::istream_iterator< std::string > end;
    std::for_each(start, end, WordProcessor(dict));
    out << "successfully loaded " << dictName << '\n';
  }

  void getFreq(std::istream& in, std::ostream& out, const FreqDictManager& dicts)
  {
    std::string dictName;
    std::string word;
    in >> dictName >> word;
    if (!in || dictName.empty() || word.empty())
    {
      out << "<MISSING ARGUMENTS>\n";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>\n";
      return;
    }
    auto dictIter = dicts.find(dictName);
    if (dictIter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    word.erase(std::remove_if(word.begin(), word.end(), IsPunct{}), word.end());
    const FrequencyDictionary& dict = dictIter->second;
    auto wordIter = dict.counts.find(word);
    if (wordIter == dict.counts.end() || dict.totalWords == 0)
    {
      out << "0.0%\n";
      return;
    }
    double freq = (static_cast< double >(wordIter->second) / dict.totalWords) * 100.0;
    ScopeGuard scopeGrd(out);
    out << std::fixed << std::setprecision(1) << freq << "%\n";
  }

  void getTopOrBottom(std::istream& in, std::ostream& out, const FreqDictManager& dicts, bool isBottom)
  {
    std::string dictName;
    size_t n = 0;
    in >> dictName >> n;
    if (!in || dictName.empty() || n == 0)
    {
      out << "<MISSING ARGUMENT>\n";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>\n";
      return;
    }
    auto dictIter = dicts.find(dictName);
    if (dictIter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    const FrequencyDictionary& dict = dictIter->second;
    if (dict.counts.empty())
    {
      out << "<EMPTY DICTIONARY>\n";
      return;
    }
    std::vector< WordFreqPair > tempVec;
    tempVec.reserve(dict.counts.size());
    auto begin = dict.counts.begin();
    auto end = dict.counts.end();
    std::transform(begin, end, std::back_inserter(tempVec), MapToPairTransformer(dict.totalWords));
    size_t limit = std::min(n, tempVec.size());
    std::partial_sort(tempVec.begin(), tempVec.begin() + limit, tempVec.end(), FreqComparator(isBottom));
    std::for_each(tempVec.begin(), tempVec.begin() + limit, PairPrinter(out));
  }

  void top(std::istream& in, std::ostream& out, const FreqDictManager& dicts)
  {
    getTopOrBottom(in, out, dicts, false);
  }

  void bottom(std::istream& in, std::ostream& out, const FreqDictManager& dicts)
  {
    getTopOrBottom(in, out, dicts, true);
  }

  void listDicts(std::ostream& out, const FreqDictManager& dicts)
  {
    if (dicts.empty())
    {
      out << "<NO DICTIONARIES>";
      return;
    }
    std::for_each(dicts.begin(), dicts.end(), DictNamePrinter{ out });
  }

  void deleteDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dictName;
    in >> dictName;
    if (!in || dictName.empty())
    {
      out << "<MISSING ARGUMENTS>\n";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>\n";
      return;
    }
    auto iter = dicts.find(dictName);
    if (iter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    dicts.erase(iter);
    out << "successfully deleted " << dictName << '\n';
  }

  void merge(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string newDictName;
    std::string dict1Name;
    std::string dict2Name;
    in >> newDictName >> dict1Name >> dict2Name;
    if (!in || newDictName.empty() || dict1Name.empty() || dict2Name.empty())
    {
      out << "<MISSING ARGUMENTS>\n";
      return;
    }
    if (!isValidName(newDictName) || !isValidName(dict1Name) || !isValidName(dict2Name))
    {
      out << "<INVALID DICTIONARY NAME>\n";
      return;
    }
    if (dicts.count(newDictName))
    {
      out << "<DICTIONARY EXIST>\n";
      return;
    }
    auto iter1 = dicts.find(dict1Name);
    auto iter2 = dicts.find(dict2Name);
    if (iter1 == dicts.end() || iter2 == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    dicts.emplace(newDictName, iter1->second);
    FrequencyDictionary& newDict = dicts.at(newDictName);
    const FrequencyDictionary& dict2 = iter2->second;
    std::for_each(dict2.counts.begin(), dict2.counts.end(), MergeFunctor(newDict));
    out << "Dictionaries merged into " << newDictName << '\n';
  }

  void intersect(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string newDictName;
    std::string dict1Name;
    std::string dict2Name;
    std::string param;
    in >> newDictName >> dict1Name >> dict2Name >> param;
    if (!in || newDictName.empty() || dict1Name.empty() || dict2Name.empty() || param.empty())
    {
      out << "<MISSING ARGUMENT>\n";
      return;
    }
    if (!isValidName(newDictName) || !isValidName(dict1Name) || !isValidName(dict2Name))
    {
      out << "<INVALID DICTIONARY NAME>\n";
      return;
    }
    if (param != "min" && param != "max")
    {
      out << "<INVALID ARGUMENT>\n";
      return;
    }
    if (dicts.count(newDictName))
    {
      out << "<DICTIONARY EXISTS>\n";
      return;
    }
    auto iter1 = dicts.find(dict1Name);
    auto iter2 = dicts.find(dict2Name);
    if (iter1 == dicts.end() || iter2 == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    dicts.emplace(newDictName, FrequencyDictionary{});
    FrequencyDictionary& newDict = dicts.at(newDictName);
    const FrequencyDictionary& dict1 = iter1->second;
    const FrequencyDictionary& dict2 = iter2->second;
    if (dict1.counts.size() < dict2.counts.size())
    {
      std::for_each(dict1.counts.begin(), dict1.counts.end(), IntersectFunctor(newDict, dict2, param == "max"));
    }
    else
    {
      std::for_each(dict2.counts.begin(), dict2.counts.end(), IntersectFunctor(newDict, dict1, param == "max"));
    }
    out << "Dictionaries intersected into " << newDictName << '\n';
  }

  void clear(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dictName;
    in >> dictName;
    if (!in || dictName.empty())
    {
      out << "<MISSING ARGUMENT>\n";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>\n";
      return;
    }
    auto dictIter = dicts.find(dictName);
    if (dictIter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    dictIter->second.counts.clear();
    dictIter->second.totalWords = 0;
    out << "Dictionary " << dictName << " has been cleared\n";
  }

  void findUniq(std::istream& in, std::ostream& out, const FreqDictManager& dicts)
  {
    std::string name1, name2;
    in >> name1 >> name2;
    if (!in || name1.empty() || name2.empty())
    {
      out << "<MISSING ARGUMENTS>\n";
      return;
    }
    auto iter1 = dicts.find(name1);
    auto iter2 = dicts.find(name2);
    if (iter1 == dicts.end() || iter2 == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    std::vector< std::string > keys1;
    std::vector< std::string > keys2;
    auto begin1 = iter1->second.counts.begin();
    auto end1 = iter1->second.counts.end();
    auto begin2 = iter2->second.counts.begin();
    auto end2 = iter2->second.counts.end();
    std::transform(begin1, end1, std::back_inserter(keys1), KeyExtractor{});
    std::transform(begin2, end2, std::back_inserter(keys2), KeyExtractor{});
    std::vector< std::string > diff;
    std::set_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::back_inserter(diff));
    out << "Words only in " << name1 << ":\n";
    std::copy(diff.begin(), diff.end(), std::ostream_iterator< std::string >(out, "\n"));
    out << '\n';
    diff.clear();
    std::set_difference(keys2.begin(), keys2.end(), keys1.begin(), keys1.end(), std::back_inserter(diff));
    out << "Words only in " << name2 << ":\n";
    std::copy(diff.begin(), diff.end(), std::ostream_iterator< std::string >(out, "\n"));
  }

  void findSame(std::istream& in, std::ostream& out, const FreqDictManager& dicts)
  {
    std::string name1;
    std::string name2;
    in >> name1 >> name2;
    if (!in || name1.empty() || name2.empty())
    {
      out << "<MISSING ARGUMENTS>\n";
      return;
    }
    auto iter1 = dicts.find(name1);
    auto iter2 = dicts.find(name2);
    if (iter1 == dicts.end() || iter2 == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    std::vector< std::string > keys1;
    std::vector< std::string > keys2;
    std::transform(iter1->second.counts.begin(), iter1->second.counts.end(), std::back_inserter(keys1), KeyExtractor{});
    std::transform(iter2->second.counts.begin(), iter2->second.counts.end(), std::back_inserter(keys2), KeyExtractor{});
    std::vector< std::string > commonKeys;
    std::set_intersection(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::back_inserter(commonKeys));
    out << "Common words:\n";
    if (commonKeys.empty())
    {
      out << "No common words found\n";
      return;
    }
    SameWordPrinter printer = SameWordPrinter{ out, iter1->second, iter2->second, name1, name2 };
    std::for_each(commonKeys.begin(), commonKeys.end(), printer);
  }

  void removeBatch(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dictName;
    std::string param;
    double n = 0;
    in >> dictName >> param >> n;
    if (!in || dictName.empty() || param.empty() || n == 0)
    {
      out << "<MISSING ARGUMENT>\n";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>\n";
      return;
    }
    auto dictIter = dicts.find(dictName);
    if (dictIter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    if (n < 0 || (param != "freq_less" && param != "freq_more"))
    {
      out << "<INVALID ARGUMENT>\n";
      return;
    }
    FrequencyDictionary& dict = dictIter->second;
    if (dict.counts.empty())
    {
      out << "<EMPTY DICTIONARY>\n";
      return;
    }
    std::vector< std::pair< const std::string, size_t > > pairsToRemove;
    auto begin = dict.counts.begin();
    auto end = dict.counts.end();
    auto pred = RemoveBatchPredicate(dict.totalWords, param == "freq_less", n);
    std::copy_if(begin, end, std::back_inserter(pairsToRemove),pred);
    if (pairsToRemove.empty())
    {
      out << "No words to remove\n";
      return;
    }
    std::for_each(pairsToRemove.begin(), pairsToRemove.end(), DictDeleter(dict));
    out << "Removed " << pairsToRemove.size() << "words\n";
  }

  void getRange(std::istream& in, std::ostream& out, const FreqDictManager& dicts, bool inRange)
  {
    std::string dictName;
    double leftBorder = 0.0;
    double rightBorder = 0.0;
    in >> dictName >> leftBorder >> rightBorder;
    if (!in || dictName.empty())
    {
      out << "<MISSING ARGUMENTS>\n";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>\n";
      return;
    }
    if (leftBorder < 0.0 || leftBorder > 100.0 ||  rightBorder < 0 || rightBorder > 100.0)
    {
      out << "<INVALID ARGUMENT>\n";
      return;
    }
    if (leftBorder > rightBorder)
    {
      out << "<INVALID RANGE>\n";
      return;
    }
    auto dictIter = dicts.find(dictName);
    if (dictIter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    const FrequencyDictionary& dict = dictIter->second;
    if (dict.counts.empty())
    {
      out << "<EMPTY DICTIONARY>\n";
      return;
    }
    std::vector< WordFreqPair > allPairs;
    allPairs.reserve(dict.counts.size());
    auto begin = dict.counts.begin();
    auto end = dict.counts.end();
    std::transform(begin, end, std::back_inserter(allPairs), MapToPairTransformer(dict.totalWords));
    std::vector< WordFreqPair > filteredPairs;
    filteredPairs.reserve(allPairs.size());
    RangeFreqPredicate predicate(leftBorder, rightBorder);
    if (inRange)
    {
      std::copy_if(allPairs.begin(), allPairs.end(), std::back_inserter(filteredPairs), predicate);
    }
    else
    {
      std::copy_if(allPairs.begin(), allPairs.end(), std::back_inserter(filteredPairs), OutOfRangeFreqPredicate(leftBorder, rightBorder));
    }
    if (filteredPairs.empty())
    {
      out << "No words found in this range\n";
      return;
    }
    std::sort(filteredPairs.begin(), filteredPairs.end(), FreqComparator(false));
    std::for_each(filteredPairs.begin(), filteredPairs.end(), PairPrinter(out));
  }

  void rangeFreq(std::istream& in, std::ostream& out, const FreqDictManager& dicts)
  {
    getRange(in, out, dicts, true);
  }

  void outRangeFreq(std::istream& in, std::ostream& out, const FreqDictManager& dicts)
  {
    getRange(in, out, dicts, false);
  }
}
