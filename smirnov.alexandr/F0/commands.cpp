#include "commands.hpp"
#include <algorithm>
#include <iterator>
#include <fstream>
#include <numeric>

namespace smirnov
{
  struct AddWithSpace
  {
    std::string operator()(const std::string & acc, const std::string & tr) const
    {
      return acc.empty() ? tr : acc + " " + tr;
    }
  };

  struct Joiner
  {
    std::string operator()(const std::vector<std::string> & vec) const
    {
      return std::accumulate(vec.begin(), vec.end(), std::string(), AddWithSpace());
    }
  };

  struct EntryToStr
  {
    std::string operator()(const Dict::value_type & entry) const
    {
      std::string result = entry.first + " -";
      if (!entry.second.empty())
      {
        Joiner joiner;
        result += " " + joiner(entry.second);
      }
      return result;
    }
  };

  struct PrefixMatcher
  {
    const std::string & prefix;
    explicit PrefixMatcher(const std::string & p):
      prefix(p)
    {}
    bool operator()(const std::pair< const std::string, std::vector< std::string > > & entry) const
    {
      return entry.first.compare(0, prefix.size(), prefix) == 0;
    }
  };

  struct TranslationFinder
  {
    const std::string & translation;
    explicit TranslationFinder(const std::string & t):
      translation(t)
    {}
    bool operator()(const std::string & tr) const
    {
      return tr == translation;
    }
  };

  struct WordFinder
  {
    const std::string & word;
    explicit WordFinder(const std::string & w):
      word(w)
    {}
    bool operator()(const std::pair< const std::string, std::vector< std::string > > & entry) const
    {
      return entry.first == word;
    }
  };

  struct NotInResult
  {
    const Dict & result;
    explicit NotInResult(const Dict & r):
      result(r)
    {}
    bool operator()(const Dict::value_type & entry) const
    {
      return result.find(entry.first) == result.end();
    }
  };

  struct InResult
  {
    const Dict & dict2;
    explicit InResult(const Dict & d):
      dict2(d)
    {}
    bool operator()(const std::pair< const std::string, std::vector< std::string > > & entry) const
    {
      return dict2.find(entry.first) != dict2.end();
    }
  };

  struct LineToEntry
  {
    Dict & dict;
    explicit LineToEntry(Dict & d):
      dict(d)
    {}
    void operator()(const std::string & line) const
    {
      size_t dashPos = line.find(" - ");
      if (dashPos == std::string::npos)
      {
        return;
      }
      std::string word = line.substr(0, dashPos);
      std::string translationsStr = line.substr(dashPos + 3);
      std::vector< std::string > translations;
      size_t start = 0;
      size_t end = translationsStr.find(' ');
      while (end != std::string::npos)
      {
        std::string translation = translationsStr.substr(start, end - start);
        if (!translation.empty())
        {
          translations.push_back(translation);
        }
        start = end + 1;
        end = translationsStr.find(' ', start);
      }
      std::string lastTranslation = translationsStr.substr(start);
      if (!lastTranslation.empty())
      {
        translations.push_back(lastTranslation);
      }
      dict[word] = translations;
    }
  };
}

void smirnov::createCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string name;
  in >> name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(name) != dicts.end())
  {
    out << "The dictionary with name " << name << " already exists.\n";
    return;
  }
  dicts[name] = Dict();
  out << name << " is successfully created.\n";
}

void smirnov::addCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word, translation;
  in >> dictName >> word >> translation;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  Dict & dict = dictIt->second;
  auto wordIt = std::find_if(dict.begin(), dict.end(), WordFinder(word));
  if (wordIt != dict.end())
  {
    if (std::find_if(wordIt->second.begin(), wordIt->second.end(), TranslationFinder(translation)) != wordIt->second.end())
    {
      out << "The word " << word << " already exists in " << dictName << "\n";
      return;
    }
    wordIt->second.push_back(translation);
  }
  else
  {
    dict[word] = {translation};
  }
}

void smirnov::translateCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word;
  in >> dictName >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  const Dict & dict = dictIt->second;
  auto wordIt = std::find_if(dict.begin(), dict.end(), WordFinder(word));
  if (wordIt == dict.end())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  std::copy(wordIt->second.begin(), wordIt->second.end(), std::ostream_iterator< std::string >(out, " "));
  out << "\n";
}

void smirnov::removeCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word;
  in >> dictName >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  Dict & dict = dictIt->second;
  auto wordIt = std::find_if(dict.begin(), dict.end(), WordFinder(word));
  if (wordIt == dict.end())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  dict.erase(wordIt);
  out << "The word " << word << " successfully deleted from " << dictName << "\n";
}

void smirnov::printCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  const Dict & dict = dictIt->second;
  if (dict.empty())
  {
    out << dictName << " is empty.\n";
    return;
  }
  std::vector< std::string > lines(dict.size());
  std::transform(dict.begin(), dict.end(), lines.begin(), EntryToStr());
  std::copy(lines.begin(), lines.end(), std::ostream_iterator< std::string >(out, "\n"));
}

void smirnov::saveCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, fileName;
  in >> dictName >> fileName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (fileName.size() < 4 || fileName.substr(fileName.size() - 4) != ".txt")
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  std::ofstream file(fileName);
  if (!file)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  file << dictName << "\n";
  std::vector< std::string > lines(dictIt->second.size());
  std::transform(dictIt->second.begin(), dictIt->second.end(), lines.begin(), EntryToStr());
  std::copy(lines.begin(), lines.end(), std::ostream_iterator< std::string >(file, "\n"));
}

void smirnov::mergeCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const Dict & first = it1->second;
  const Dict & second = it2->second;
  Dict result;
  std::copy(first.begin(), first.end(), std::inserter(result, result.end()));
  std::copy_if(second.begin(), second.end(), std::inserter(result, result.end()), NotInResult(result));
  dicts[newName] = std::move(result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::deleteCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  dicts.erase(it);
}

void smirnov::editCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word, newTranslation;
  in >> dictName >> word >> newTranslation;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  Dict & dict = dictIt->second;
  auto wordIt = std::find_if(dict.begin(), dict.end(), WordFinder(word));
  if (wordIt == dict.end())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  wordIt->second.clear();
  wordIt->second.push_back(newTranslation);
}

void smirnov::renameCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string oldName, newName;
  in >> oldName >> newName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto oldIt = dicts.find(oldName);
  if (oldIt == dicts.end())
  {
    out << "The dictionary with name " << oldName << " doesn't exist.\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  dicts[newName] = std::move(oldIt->second);
  dicts.erase(oldIt);
}

void smirnov::moveCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string fromDict, toDict, word;
  in >> fromDict >> toDict >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto fromIt = dicts.find(fromDict);
  auto toIt = dicts.find(toDict);
  if (fromIt == dicts.end() || toIt == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Dict & from = fromIt->second;
  Dict & to = toIt->second;
  auto wordIt = std::find_if(from.begin(), from.end(), WordFinder(word));
  if (wordIt == from.end())
  {
    out << "The word " << word << " doesn't exist in " << fromDict << "\n";
    return;
  }
  if (std::find_if(to.begin(), to.end(), WordFinder(word)) != to.end())
  {
    out << "The word " << word << " already exists in " << toDict << "\n";
    return;
  }
  to[word] = std::move(wordIt->second);
  from.erase(wordIt);
  out << "The word " << word << " moved from " << fromDict << " to " << toDict << "\n";
}

void smirnov::copyCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string fromDict, toDict, word;
  in >> fromDict >> toDict >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto fromIt = dicts.find(fromDict);
  auto toIt = dicts.find(toDict);
  if (fromIt == dicts.end() || toIt == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const Dict & from = fromIt->second;
  Dict & to = toIt->second;
  auto wordIt = std::find_if(from.begin(), from.end(), WordFinder(word));
  if (wordIt == from.end())
  {
    out << "The word " << word << " doesn't exist in " << fromDict << "\n";
    return;
  }
  if (std::find_if(to.begin(), to.end(), WordFinder(word)) != to.end())
  {
    out << "The word " << word << " already exists in " << toDict << "\n";
    return;
  }
  to[word] = wordIt->second;
  out << "The word " << word << " copied from " << fromDict << " to " << toDict << "\n";
}

void smirnov::intersectCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const Dict & dict1 = it1->second;
  const Dict & dict2 = it2->second;
  Dict result;
  std::copy_if(dict1.begin(), dict1.end(), std::inserter(result, result.end()), InResult(dict2));
  dicts[newName] = std::move(result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::differenceCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const Dict & dict1 = it1->second;
  const Dict & dict2 = it2->second;
  Dict result;
  std::copy_if(dict1.begin(), dict1.end(), std::inserter(result, result.end()), NotInResult(dict2));
  dicts[newName] = std::move(result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::uniqueCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const Dict & dict1 = it1->second;
  const Dict & dict2 = it2->second;
  Dict result;
  std::copy_if(dict1.begin(), dict1.end(), std::inserter(result, result.end()), NotInResult(dict2));
  std::copy_if(dict2.begin(), dict2.end(), std::inserter(result, result.end()), NotInResult(dict1));
  dicts[newName] = std::move(result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::prefixCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dictName, prefix;
  in >> newName >> dictName >> prefix;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  const Dict & dict = dictIt->second;
  if (dict.empty())
  {
    out << dictName << " is empty.\n";
    return;
  }
  Dict result;
  std::copy_if(dict.begin(), dict.end(), std::inserter(result, result.end()), PrefixMatcher(prefix));
  if (result.empty())
  {
    out << "There aren't any words in " << dictName << " with prefix " << prefix << ".\n";
    return;
  }
  dicts[newName] = std::move(result);
}

void smirnov::importDictfromFile(Dicts & dicts, const std::string & filename)
{
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("Cannot open file\n");
  }
  std::string line;
  std::string dictName;
  Dict dict;
  bool hasDict = false;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }
    if (line.find(" - ") == std::string::npos)
    {
      if (hasDict)
      {
        dicts[dictName] = dict;
        dict.clear();
      }
      dictName = line;
      hasDict = true;
    }
    else
    {
      LineToEntry entryParser(dict);
      entryParser(line);
    }
  }
  if (hasDict)
  {
    dicts[dictName] = dict;
  }
}

void smirnov::helpCommand(std::ostream & out)
{
  out << "Available commands:\n";
  out << "1) create <dict> - Create a new dictionary\n";
  out << "2) add <dict> <key> <translation> - Add word to dictionary\n";
  out << "3) translate <dict> <key> - Translate a word\n";
  out << "4) remove <dict> <key> - Remove a word\n";
  out << "5) print <dict> - Print dictionary contents\n";
  out << "6) save <dict> <filename> - Save dictionary to file\n";
  out << "7) merge <resdict> <dict1> <dict2> - Merge two dictionaries\n";
  out << "8) delete <dict> - Delete a dictionary\n";
  out << "9) edit <dict> <key> <newtranslation> - Edit word translation\n";
  out << "10) rename <dict> <newname> - Rename dictionary\n";
  out << "11) move <dict1> <dict2> <key> - Move word between dictionaries\n";
  out << "12) copy <dict1> <dict2> <key> - Copy word between dictionaries\n";
  out << "13) intersect <resdict> <dict1> <dict2> - Intersect two dictionaries\n";
  out << "14) difference <resdict> <dict1> <dict2> - Difference of dictionaries\n";
  out << "15) unique <resdict> <dict1> <dict2> - Unique words from dictionaries\n";
  out << "16) prefix <newdict> <dict> <prefix> - Words with given prefix\n";
}
