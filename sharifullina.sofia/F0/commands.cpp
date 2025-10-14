#include "commands.hpp"
#include <fstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <iomanip>

namespace
{
  bool dictExists(const std::string & name, const sharifullina::DictCollection & dicts)
  {
    return dicts.find(name) != dicts.end();
  }

  bool wordExists(const std::string & dictName, const std::string & word, const sharifullina::DictCollection & dicts)
  {
    auto dictIt = dicts.find(dictName);
    if (dictIt == dicts.end())
    {
      return false;
    }
    return dictIt->second.find(word) != dictIt->second.end();
  }

  struct PrintDictName
  {
    std::ostream & out;
    void operator()(const std::pair< std::string, sharifullina::Dictionary > & dict) const
    {
      out << dict.first << '\n';
    }
  };

  struct PrintWord
  {
    std::ostream & out;
    void operator()(const std::string & word) const
    {
      out << word << ' ';
    }
  };

  struct PrintWordWithTranslations
  {
    std::ostream & out;
    void operator()(const std::pair< std::string, std::set< std::string > > & wordPair) const
    {
      out << wordPair.first << ' ';
      std::for_each(wordPair.second.cbegin(), wordPair.second.cend(), PrintWord{out});
      out << '\n';
    }
  };

  void readTranslations(std::istream & in, std::set< std::string > & translations)
  {
    std::string translation;
    while (in >> translation)
    {
      translations.insert(translation);
    }
  }

  sharifullina::Dictionary readDictionary(std::istream & in)
  {
    sharifullina::Dictionary dict;
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }

      size_t firstSpace = line.find(' ');
      if (firstSpace == std::string::npos)
      {
        continue;
      }
      std::string word = line.substr(0, firstSpace);
      std::string translationsStr = line.substr(firstSpace + 1);

      std::set< std::string > translations;
      size_t start = 0;
      size_t end = 0;

      while ((end = translationsStr.find(' ', start)) != std::string::npos)
      {
        if (end > start)
        {
          translations.insert(translationsStr.substr(start, end - start));
        }
        start = end + 1;
      }

      if (start < translationsStr.length())
      {
        translations.insert(translationsStr.substr(start));
      }

      if (!translations.empty())
      {
        dict[word] = translations;
      }
    }
    return dict;
  }

  struct MergeDictProcessor
  {
    sharifullina::Dictionary & newDict;
    void operator()(const std::pair< std::string, std::set< std::string > > & wordPair)
    {
      newDict[wordPair.first].insert(wordPair.second.cbegin(), wordPair.second.cend());
    }
  };

  struct StatDictProcessor
  {
    size_t & totalTranslations;
    void operator()(const std::pair< std::string, std::set< std::string > > & wordPair)
    {
      totalTranslations += wordPair.second.size();
    }
  };

  struct SubtractDictProcessor
  {
    const std::vector< std::string > & dictNames;
    const sharifullina::DictCollection & dicts;
    sharifullina::Dictionary & newDict;

    void operator()(const std::pair< std::string, std::set< std::string > > & wordPair)
    {
      bool foundInOthers = false;
      for (size_t i = 1; i < dictNames.size() && !foundInOthers; ++i)
      {
        const auto & otherDict = dicts.at(dictNames[i]);
        if (otherDict.find(wordPair.first) != otherDict.end())
        {
          foundInOthers = true;
        }
      }
      if (!foundInOthers)
      {
        newDict[wordPair.first] = wordPair.second;
      }
    }
  };

  struct SymdiffDictProcessor
  {
    const std::vector< std::string > & dictNames;
    const sharifullina::DictCollection & dicts;
    std::unordered_map< std::string, int > & wordCount;
    std::unordered_map< std::string, std::set< std::string > > & wordTranslations;

    void operator()(const std::pair< std::string, std::set< std::string > > & wordPair)
    {
      wordCount[wordPair.first]++;
      wordTranslations[wordPair.first].insert(wordPair.second.cbegin(), wordPair.second.cend());
    }
  };

  struct SymdiffFinalProcessor
  {
    const std::unordered_map< std::string, int > & wordCount;
    const std::unordered_map< std::string, std::set< std::string > > & wordTranslations;
    sharifullina::Dictionary & newDict;

    void operator()(const std::pair< std::string, int > & countPair)
    {
      if (countPair.second == 1)
      {
        newDict[countPair.first] = wordTranslations.at(countPair.first);
      }
    }
  };
}

void sharifullina::createDict(std::istream & in, DictCollection & dicts)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("invalid arguments for createdict");
  }

  if (dictExists(name, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }

  dicts[name] = Dictionary{};
}

void sharifullina::deleteDict(std::istream & in, DictCollection & dicts)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("invalid arguments for deletedict");
  }

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::runtime_error("dictionary not found");
  }
  dicts.erase(it);
}

void sharifullina::listDicts(std::istream &, DictCollection & dicts)
{
  if (dicts.empty())
  {
    std::cout << "<EMPTY>\n";
    return;
  }
  std::for_each(dicts.cbegin(), dicts.cend(), PrintDictName{std::cout});
}

void sharifullina::addWord(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  if (!(in >> dictName >> word))
  {
    throw std::runtime_error("invalid arguments for addword");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found");
  }
  auto & dict = dicts[dictName];
  if (dict.find(word) != dict.end())
  {
    throw std::runtime_error("word already exists");
  }
  std::set< std::string > translations;
  readTranslations(in, translations);
  if (translations.empty())
  {
    throw std::runtime_error("no translations provided");
  }
  dict[word] = translations;
}

void sharifullina::addTranslation(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  std::string translation;
  if (!(in >> dictName >> word >> translation))
  {
    throw std::runtime_error("invalid arguments for addtranslation");
  }
  if (!wordExists(dictName, word, dicts))
  {
    throw std::runtime_error("dictionary or word not found");
  }
  auto & dict = dicts[dictName];
  auto wordIt = dict.find(word);
  wordIt->second.insert(translation);
}

void sharifullina::removeTranslation(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  std::string translation;
  if (!(in >> dictName >> word >> translation))
  {
    throw std::runtime_error("invalid arguments for removetranslation");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary, word or translation not found");
  }
  auto & dict = dicts[dictName];
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    throw std::runtime_error("dictionary, word or translation not found");
  }
  auto & translations = wordIt->second;
  auto transIt = translations.find(translation);
  if (transIt == translations.end())
  {
    throw std::runtime_error("dictionary, word or translation not found");
  }
  translations.erase(transIt);
  if (translations.empty())
  {
    dict.erase(wordIt);
  }
}

void sharifullina::deleteWord(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  if (!(in >> dictName >> word))
  {
    throw std::runtime_error("invalid arguments for deleteword");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary or word not found");
  }
  auto & dict = dicts[dictName];
  if (dict.erase(word) == 0)
  {
    throw std::runtime_error("dictionary or word not found");
  }
}

void sharifullina::findTranslations(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  if (!(in >> dictName >> word))
  {
    throw std::runtime_error("invalid arguments for findtranslations");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary or word not found");
  }
  const auto & dict = dicts.at(dictName);
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    throw std::runtime_error("dictionary or word not found");
  }
  std::copy(wordIt->second.cbegin(), wordIt->second.cend(), std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << '\n';
}

void sharifullina::listWords(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    throw std::runtime_error("invalid arguments for listwords");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found");
  }
  const auto & dict = dicts.at(dictName);
  if (dict.empty())
  {
    std::cout << "<EMPTY>\n";
    return;
  }
  std::for_each(dict.cbegin(), dict.cend(), PrintWordWithTranslations{std::cout});
}

void sharifullina::mergeDicts(std::istream & in, DictCollection & dicts)
{
  std::string newDictName;
  int count = 0;
  if (!(in >> newDictName >> count))
  {
    throw std::runtime_error("invalid arguments for merge");
  }
  if (count < 2)
  {
    throw std::runtime_error("invalid count");
  }
  std::vector< std::string > dictNames;
  std::string dictName;
  for (int i = 0; i < count; ++i)
  {
    if (!(in >> dictName))
    {
      throw std::runtime_error("invalid count");
    }
    dictNames.push_back(dictName);
  }
  for (const auto & name : dictNames)
  {
    if (!dictExists(name, dicts))
    {
      throw std::runtime_error("dictionary not found");
    }
  }
  if (dictExists(newDictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  Dictionary newDict;
  for (const auto & name : dictNames)
  {
    const auto & sourceDict = dicts.at(name);
    std::for_each(sourceDict.cbegin(), sourceDict.cend(), MergeDictProcessor{newDict});
  }
  dicts[newDictName] = newDict;
}

void sharifullina::findCommon(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  int count = 0;
  if (!(in >> dictName >> count))
  {
    throw std::runtime_error("invalid arguments for findcommon");
  }
  if (count < 1)
  {
    throw std::runtime_error("invalid count");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary or word(s) not found");
  }
  std::vector< std::string > words;
  std::string word;
  for (int i = 0; i < count; ++i)
  {
    if (!(in >> word))
    {
      throw std::runtime_error("invalid count");
    }
    words.push_back(word);
  }
  const auto & dict = dicts.at(dictName);
  for (const auto & w : words)
  {
    if (dict.find(w) == dict.end())
    {
      throw std::runtime_error("dictionary or word(s) not found");
    }
  }
  std::set< std::string > commonTranslations = dict.at(words[0]);
  for (size_t i = 1; i < words.size(); ++i)
  {
    const auto & currentTranslations = dict.at(words[i]);
    std::set< std::string > temp;
    std::set_intersection(
      commonTranslations.cbegin(), commonTranslations.cend(),
      currentTranslations.cbegin(), currentTranslations.cend(),
      std::inserter(temp, temp.begin())
    );
    commonTranslations = std::move(temp);
  }
  if (commonTranslations.empty())
  {
    std::cout << "<EMPTY>\n";
    return;
  }
  std::for_each(commonTranslations.cbegin(), commonTranslations.cend(), PrintWord{std::cout});
  std::cout << '\n';
}

void sharifullina::saveDict(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string filename;
  if (!(in >> dictName >> filename))
  {
    throw std::runtime_error("invalid arguments for save");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found or file error");
  }
  std::ofstream file(filename);
  if (!file)
  {
    throw std::runtime_error("dictionary not found or file error");
  }
  const auto & dict = dicts.at(dictName);
  std::for_each(dict.cbegin(), dict.cend(), PrintWordWithTranslations{file});
}

void sharifullina::loadDict(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string filename;
  if (!(in >> dictName >> filename))
  {
    throw std::runtime_error("invalid arguments for load");
  }
  if (dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("file not found or invalid format");
  }
  Dictionary newDict = readDictionary(file);
  if (newDict.empty())
  {
    throw std::runtime_error("file not found or invalid format");
  }
  dicts[dictName] = newDict;
}

void sharifullina::statDict(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    throw std::runtime_error("invalid arguments for stat");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found");
  }
  const auto & dict = dicts.at(dictName);
  size_t totalWords = dict.size();
  size_t totalTranslations = 0;
  std::for_each(dict.cbegin(), dict.cend(), StatDictProcessor{totalTranslations});
  double avgTranslations = totalWords > 0 ? static_cast< double >(totalTranslations) / totalWords : 0.0;
  std::cout << "Words: " << totalWords << "\n";
  std::cout << "Translations: " << totalTranslations << "\n";
  std::cout << "Average translations per word: " << avgTranslations << "\n";
}

void sharifullina::subtractDicts(std::istream & in, DictCollection & dicts)
{
  std::string newDictName;
  int count = 0;
  if (!(in >> newDictName >> count))
  {
    throw std::runtime_error("invalid arguments for subtract");
  }
  if (count < 2)
  {
    throw std::runtime_error("invalid count");
  }
  std::vector< std::string > dictNames;
  std::string dictName;
  for (int i = 0; i < count; ++i)
  {
    if (!(in >> dictName))
    {
      throw std::runtime_error("invalid count");
    }
    dictNames.push_back(dictName);
  }
  for (const auto & name : dictNames)
  {
    if (!dictExists(name, dicts))
    {
      throw std::runtime_error("dictionary not found");
    }
  }
  if (dictExists(newDictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  const auto & firstDict = dicts.at(dictNames[0]);
  Dictionary newDict;
  std::for_each(firstDict.cbegin(), firstDict.cend(), SubtractDictProcessor{dictNames, dicts, newDict});
  dicts[newDictName] = newDict;
}

void sharifullina::symdiffDicts(std::istream & in, DictCollection & dicts)
{
  std::string newDictName;
  int count = 0;
  if (!(in >> newDictName >> count))
  {
    throw std::runtime_error("invalid arguments for symdiff");
  }
  if (count < 2)
  {
    throw std::runtime_error("invalid count");
  }
  std::vector< std::string > dictNames;
  std::string dictName;
  for (int i = 0; i < count; ++i)
  {
    if (!(in >> dictName))
    {
      throw std::runtime_error("invalid count");
    }
    dictNames.push_back(dictName);
  }
  for (const auto & name : dictNames)
  {
    if (!dictExists(name, dicts))
    {
      throw std::runtime_error("dictionary not found");
    }
  }
  if (dictExists(newDictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  Dictionary newDict;
  std::unordered_map< std::string, int > wordCount;
  std::unordered_map< std::string, std::set< std::string > > wordTranslations;
  for (const auto & name : dictNames)
  {
    const auto & dict = dicts.at(name);
    std::for_each(dict.cbegin(), dict.cend(), SymdiffDictProcessor{dictNames, dicts, wordCount, wordTranslations});
  }
  std::for_each(wordCount.cbegin(), wordCount.cend(), SymdiffFinalProcessor{wordCount, wordTranslations, newDict});
  dicts[newDictName] = newDict;
}

void sharifullina::loadFile(const std::string & filename, DictCollection & dicts)
{
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("file not found or invalid format");
  }
  Dictionary newDict = readDictionary(file);
  if (!newDict.empty())
  {
    dicts["default"] = newDict;
  }
}

void sharifullina::printHelp(std::ostream & out)
{
  out << std::left;
  out << "Available commands:\n" << '\n';
  constexpr size_t cmdWidth = 30;
  constexpr size_t numWidth = 3;
  out << std::setw(numWidth) << "1." << std::setw(cmdWidth);
  out << "createdict <name>" << "create a new dictionary\n";

  out << std::setw(numWidth) << "2." << std::setw(cmdWidth);
  out << "deletedict <name>" << "delete a dictionary\n";

  out << std::setw(numWidth) << "3." << std::setw(cmdWidth);
  out << "listdicts" << "list all dictionaries\n";

  out << std::setw(numWidth) << "4." << std::setw(cmdWidth);
  out << "addword <dict> <word> <trans...>" << "add word with translations\n";

  out << std::setw(numWidth) << "5." << std::setw(cmdWidth);
  out << "addtranslation <dict> <word> <trans>" << "add translation to word\n";

  out << std::setw(numWidth) << "6." << std::setw(cmdWidth);
  out << "removetranslation <dict> <word> <trans>" << "remove translation\n";

  out << std::setw(numWidth) << "7." << std::setw(cmdWidth);
  out << "deleteword <dict> <word>" << "delete word\n";

  out << std::setw(numWidth) << "8." << std::setw(cmdWidth);
  out << "findtranslations <dict> <word>" << "find word translations\n";

  out << std::setw(numWidth) << "9." << std::setw(cmdWidth);
  out << "listwords <dict>" << "list all words in dictionary\n";

  out << std::setw(numWidth) << "10." << std::setw(cmdWidth);
  out << "merge <new> <count> <dicts...>" << "merge dictionaries\n";

  out << std::setw(numWidth) << "11." << std::setw(cmdWidth);
  out << "findcommon <dict> <count> <words...>" << "find common translations\n";

  out << std::setw(numWidth) << "12." << std::setw(cmdWidth);
  out << "save <dict> <file>" << "save dictionary to file\n";

  out << std::setw(numWidth) << "13." << std::setw(cmdWidth);
  out << "load <dict> <file>" << "load dictionary from file\n";

  out << std::setw(numWidth) << "14." << std::setw(cmdWidth);
  out << "stat <dict>" << "show dictionary statistics\n";

  out << std::setw(numWidth) << "15." << std::setw(cmdWidth);
  out << "subtract <new> <count> <dicts...>" << "dictionary subtraction\n";

  out << std::setw(numWidth) << "16." << std::setw(cmdWidth);
  out << "symdiff <new> <count> <dicts...>" << "symmetric difference\n";
}

void sharifullina::printError(const std::string & message)
{
  std::cout << "<ERROR: " << message << ">\n";
}
