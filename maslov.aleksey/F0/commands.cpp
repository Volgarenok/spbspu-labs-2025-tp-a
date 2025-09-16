#include "commands.hpp"
#include <iterator>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <functional>

namespace
{
  bool isAlpha(char c)
  {
    return std::isalpha(c);
  }

  char toLowercase(char c)
  {
    return std::tolower(c);
  }

  bool comparatorAscending(const maslov::Word & a, const maslov::Word & b)
  {
    return a.second < b.second;
  }

  bool comparatorDescending(const maslov::Word & a, const maslov::Word & b)
  {
    return a.second > b.second;
  }

  void sortWords(std::vector< maslov::Word > & words, const std::string & order)
  {
    if (order == "ascending")
    {
      std::sort(words.begin(), words.end(), comparatorAscending);
    }
    else
    {
      std::sort(words.begin(), words.end(), comparatorDescending);
    }
  }

  bool comparatorFrequency(const maslov::Word & word, int low, int high)
  {
    return word.second >= low && word.second <= high;
  }

  struct PrintWord
  {
    std::ostream & out_;
    void operator()(const maslov::Word & word) const
    {
      out_ << word.first << ' ' << word.second << '\n';
    }
  };

  struct PrintDicts
  {
    std::ostream & out_;
    void operator()(const std::pair< std::string, maslov::Dict > & dict) const
    {
      out_ << dict.first << ' ' << dict.second.size() << '\n';
      std::for_each(dict.second.cbegin(), dict.second.cend(), PrintWord{out_});
    }
  };

  bool intersectWord(const maslov::Dict & dict, const maslov::Word & word)
  {
    return dict.find(word.first) != dict.end();
  }

  maslov::Word frequencyUpdate(const maslov::Dict & dict, const maslov::Word & word)
  {
    auto it = dict.find(word.first);
    return {word.first, std::min(word.second, it->second)};
  }

  void mergeWords(maslov::Dict & dict, const maslov::Word & word)
  {
    auto & freq = dict[word.first];
    freq += word.second;
  }

  struct PrintNameDicts
  {
    std::ostream & out_;
    void operator()(const std::pair< std::string, maslov::Dict > & dict) const
    {
      out_ << dict.first << ' ' << '\n';
    }
  };

  void readRawWord(maslov::Dict & dict, const std::string & word)
  {
    std::string realWord;
    std::copy_if(word.cbegin(), word.cend(), std::back_inserter(realWord), isAlpha);
    std::transform(realWord.begin(), realWord.end(), realWord.begin(), toLowercase);
    if (!realWord.empty())
    {
      dict[realWord]++;
    }
  }

  maslov::Word readWordFreq(std::istream & in)
  {
    maslov::Word word;
    in >> word.first >> word.second;
    return word;
  }

  maslov::Dict readDict(std::istream & in)
  {
    size_t wordCount;
    in >> wordCount;
    maslov::Dict dict;
    auto reader = std::bind(readWordFreq, std::ref(in));
    std::generate_n(std::inserter(dict, dict.end()), wordCount, reader);
    return dict;
  }

  std::pair< std::string, maslov::Dict > readNamedDict(std::istream & in)
  {
    std::pair< std::string, maslov::Dict > namedDict;
    in >> namedDict.first;
    namedDict.second = readDict(in);
    return namedDict;
  }
}

void maslov::createDictionary(std::istream & in, Dicts & dicts)
{
  std::string dictName;
  in >> dictName;
  if (dicts.find(dictName) != dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  dicts[dictName] = Dict{};
}

void maslov::showDictionary(std::ostream & out, const Dicts & dicts)
{
  if (dicts.empty())
  {
    out << '\n';
  }
  std::for_each(dicts.cbegin(), dicts.cend(), PrintNameDicts{out});
}

void maslov::loadText(std::istream & in, Dicts & dicts)
{
  std::string filename, dictName;
  in >> dictName >> filename;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  std::string word;
  using inputIt = std::istream_iterator< std::string >;
  using namespace std::placeholders;
  auto func = std::bind(readRawWord, std::ref(it->second), _1);
  std::for_each(inputIt{file}, inputIt{}, func);
}

void maslov::unionDictionary(std::istream & in, Dicts & dicts)
{
  std::string dictName1, dictName2, resultName;
  in >> resultName >> dictName1 >> dictName2;
  auto it1 = dicts.find(dictName1);
  auto it2 = dicts.find(dictName2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (dicts.find(resultName) != dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  auto & result = dicts[resultName];
  const auto & dict1 = it1->second;
  const auto & dict2 = it2->second;
  std::copy(dict1.cbegin(), dict1.cend(), std::inserter(result, result.end()));
  using namespace std::placeholders;
  auto func = std::bind(mergeWords, std::ref(result), _1);
  std::for_each(dict2.begin(), dict2.end(), func);
}

void maslov::intersectDictionary(std::istream & in, Dicts & dicts)
{
  std::string dictName1, dictName2, resultName;
  in >> resultName >> dictName1 >> dictName2;
  auto it1 = dicts.find(dictName1);
  auto it2 = dicts.find(dictName2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (dicts.find(resultName) != dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  auto & result = dicts[resultName];
  const auto & dict1 = it1->second;
  const auto & dict2 = it2->second;
  using namespace std::placeholders;
  auto func = std::bind(intersectWord, std::cref(dict2), _1);
  std::copy_if(dict1.cbegin(), dict1.cend(), std::inserter(result, result.end()), func);
  auto update = std::bind(frequencyUpdate,std::cref(dict2), _1);
  Dict temp;
  std::transform(result.begin(), result.end(), std::inserter(temp, temp.end()), update);
  result = std::move(temp);
}

void maslov::copyDictionary(std::istream & in, Dicts & dicts)
{
  std::string resultName, dictName;
  in >> resultName >> dictName;
  auto it = dicts.find(dictName);
  if (it == dicts.end() || dicts.find(resultName) != dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  dicts[resultName] = it->second;
}

void maslov::addWord(std::istream & in, Dicts & dicts)
{
  std::string dictName, wordName, frequency;
  in >> dictName >> wordName >> frequency;
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (it->second.find(wordName) != it->second.end())
  {
    throw std::runtime_error("<INVALID WORD>");
  }
  int num = std::stoi(frequency);
  if (num <= 0)
  {
    throw std::runtime_error("<INVALID NUMBER>");
  }
  it->second[wordName] = num;
}

void maslov::printSize(std::istream & in, std::ostream & out, const Dicts & dicts)
{
  std::string dictName;
  in >> dictName;
  auto it = dicts.find(dictName);
  if (it == dicts.cend())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  out << it->second.size() << '\n';
}

void maslov::cleanWord(std::istream & in, Dicts & dicts)
{
  std::string dictName, wordName;
  in >> dictName >> wordName;
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (it->second.find(wordName) == it->second.end())
  {
    throw std::runtime_error("<INVALID WORD>");
  }
  it->second.erase(wordName);
}

void maslov::cleanDictionary(std::istream & in, Dicts & dicts)
{
  std::string dictName;
  in >> dictName;
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  dicts.erase(dictName);
}

void maslov::printTopRare(std::istream & in, std::ostream & out, const Dicts & dicts, const std::string & order)
{
  std::string dictName;
  size_t number;
  in >> dictName >> number;
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.cend())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (number == 0 || number > dictIt->second.size())
  {
    throw std::runtime_error("<INVALID NUMBER>");
  }
  std::vector< Word > words;
  words.reserve(dictIt->second.size());
  const auto & dict = dictIt->second;
  std::copy(dict.cbegin(), dict.cend(), std::back_inserter(words));
  sortWords(words, order);
  std::for_each(words.cbegin(), words.cbegin() + number, PrintWord{out});
}

void maslov::printFrequency(std::istream & in, std::ostream & out, const Dicts & dicts)
{
  std::string dictName, wordName;
  in >> dictName >> wordName;
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.cend())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (dictIt->second.find(wordName) == dictIt->second.cend())
  {
    throw std::runtime_error("<INVALID WORD>");
  }
  out << dictIt->second.at(wordName) << '\n';
}

void maslov::createWordRange(std::istream & in, Dicts & dicts)
{
  std::string resultName, dictName;
  int freq1, freq2;
  in >> resultName >> dictName >> freq1 >> freq2;
  if (freq2 < freq1)
  {
    throw std::runtime_error("<INVALID INTERVAL>");
  }
  auto resultIt = dicts.find(resultName);
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end() || resultIt != dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  auto & result = dicts[resultName];
  using namespace std::placeholders;
  auto cmp = std::bind(comparatorFrequency, _1, freq1, freq2);
  const auto & dict = dictIt->second;
  std::copy_if(dict.cbegin(), dict.cend(), std::inserter(result, result.end()), cmp);
  if (result.empty())
  {
    dicts.erase(resultName);
    throw std::runtime_error("<EMPTY INTERVAL>");
  }
}

void maslov::saveDictionaries(std::istream & in, const Dicts & dicts)
{
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  file << dicts.size() << '\n';
  std::for_each(dicts.cbegin(), dicts.cend(), PrintDicts{file});
}

void maslov::loadFile(const std::string & filename, Dicts & dicts)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  size_t dictCount;
  file >> dictCount;
  auto func = std::bind(readNamedDict, std::ref(file));
  std::generate_n(std::inserter(dicts, dicts.end()), dictCount, func);
}

void maslov::printHelp(std::ostream & out)
{
  out << std::left;
  out << "Available commands:\n" << '\n';
  constexpr size_t cmdWidth = 50;
  constexpr size_t numWidth = 4;
  out << std::setw(numWidth) << "1." << std::setw(cmdWidth);
  out << "createdict <dict_name>" << "create a new dictionary\n";

  out << std::setw(numWidth) << "2." << std::setw(cmdWidth);
  out << "showdicts" << "shows all dictionary names\n";

  out << std::setw(numWidth) << "3." << std::setw(cmdWidth);
  out << "loadtext <dict_name> <file>" << "upload text from a file to the dictionary\n";

  out << std::setw(numWidth) << "4." << std::setw(cmdWidth);
  out << "union <result> <dict1> <dict2>" << "creates a new dictionary that combines the other two\n";

  out << std::setw(numWidth) << "5." << std::setw(cmdWidth);
  out << "intersect <result> <dict1> <dict2>" << "creates a new dictionary that intersects the other two\n";

  out << std::setw(numWidth) << "6." << std::setw(cmdWidth);
  out << "copy <result> <dict>" << "copies dictionary data\n";

  out << std::setw(numWidth) << "7." << std::setw(cmdWidth);
  out << "addword <dict_name> <word> <frequency>" << "adding a word to the dictionary\n";

  out << std::setw(numWidth) << "8." << std::setw(cmdWidth);
  out << "size <dict_name>" << "print the number of words in the dictionary\n";

  out << std::setw(numWidth) << "9." << std::setw(cmdWidth);
  out << "cleanword <dict_name> <word>" << "deleting a word from the dictionary\n";

  out << std::setw(numWidth) << "10." << std::setw(cmdWidth);
  out << "cleandict <dict_name>" << "deleting all words from the dictionary\n";

  out << std::setw(numWidth) << "11." << std::setw(cmdWidth);
  out << "top <dict_name> <number>" << "print the most frequently used words\n";

  out << std::setw(numWidth) << "12." << std::setw(cmdWidth);
  out << "rare <dict_name> <number>" << "print the least frequently used words\n";

  out << std::setw(numWidth) << "13." << std::setw(cmdWidth);
  out << "frequency <dict_name> <word>" << "print the word frequency\n";

  out << std::setw(numWidth) << "14." << std::setw(cmdWidth);
  out << "wordrange <result> <dict_name> <freq1> <freq2>";
  out << "creates a dictionary of words whose frequencies are in the range\n";

  out << std::setw(numWidth) << "15." << std::setw(cmdWidth);
  out << "save <file>" << "saves all dictionaries to a file\n";
}
