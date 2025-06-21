#include "commands.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <string>
#include <regex>
#include "dictionary_manager.hpp"

std::ostream& operator<<(std::ostream& os, const std::pair< const std::string, int >& p)
{
  return os << p.first << " " << p.second;
}

namespace smirnov
{
  void addWord(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string word;
    is >> word;
    if (word.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }

    if (dm.getCurrentDict() == nullptr)
    {
      os << "<INVALID COMMAND>\n";
      return;
    }

    int freq_val = 1;
    std::string freq_str;
    if (is.peek() != EOF && is.peek() != '\n' && is >> freq_str)
    {
      try
      {
        freq_val = std::stoi(freq_str);
        if (freq_val <= 0)
        {
          os << "<INVALID COMMAND>\n";
          return;
        }
      }
      catch (const std::invalid_argument&)
      {
        os << "<INVALID COMMAND>\n";
        return;
      }
      catch (const std::out_of_range&)
      {
        os << "<INVALID COMMAND>\n";
        return;
      }
    }

    if (!dm.addWord(word, freq_val))
    {
      os << "<INVALID COMMAND>\n";
    }
  }

  void removeWord(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string word;
    is >> word;
    if (word.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (dm.getCurrentDict() == nullptr)
    {
      os << "<INVALID COMMAND>\n";
      return;
    }

    if (!dm.removeWord(word))
    {
      os << "<WORD NOT FOUND>\n";
    }
  }

  void freq(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string word;
    is >> word;
    if (word.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (dm.getCurrentDict() == nullptr)
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    int frequency;
    if (!dm.getFreq(word, frequency))
    {
      os << "<WORD NOT FOUND>\n";
    }
    else
    {
      os << frequency << "\n";
    }
  }

  void createDictCmd(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string name;
    is >> name;
    if (name.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (!dm.createDict(name))
    {
      os << "<INVALID COMMAND>\n";
    }
  }

  void deleteDictCmd(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string name;
    is >> name;
    if (name.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (!dm.deleteDict(name))
    {
      os << "<INVALID COMMAND>\n";
    }
  }

  void pushDictCmd(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string name;
    is >> name;
    if (name.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (!dm.pushDict(name))
    {
      os << "<INVALID COMMAND>\n";
    }
  }

  void popDictCmd(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    if (!dm.popDict())
    {
      os << "<INVALID COMMAND>\n";
    }
  }

  void stackListCmd(std::istream&, DictionaryManager& dm, std::ostream& os)
  {
    const auto& stack = dm.getStack();
    if (stack.empty())
    {
      os << "<EMPTY>\n";
    }
    else
    {
      std::for_each(stack.begin(), stack.end(),
        [&os](const std::string& name) {
          os << name << "\n";
        });
    }
  }

  void list(std::istream&, DictionaryManager& dm, std::ostream& os)
  {
    const auto* dict = dm.getCurrentDict();
    if (!dict)
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (dict->empty())
    {
      os << "<EMPTY>\n";
      return;
    }

    std::vector< std::pair< std::string, int > > items(dict->begin(), dict->end());
    std::sort(items.begin(), items.end(),
      [](const auto& a, const auto& b) { return a.first < b.first; });

    std::for_each(items.begin(), items.end(), [&](const auto& p) {
      os << p.first << " " << p.second << "\n";
      });
  }

  void mostFreq(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    int n;
    if (!(is >> n) || n <= 0)
    {
      os << "<INVALID COMMAND>\n";
      return;
    }

    const auto* dict = dm.getCurrentDict();
    if (!dict || dict->empty())
    {
      os << "<EMPTY>\n";
      return;
    }

    std::vector< std::pair< std::string, int > > items(dict->begin(), dict->end());
    std::sort(items.begin(), items.end(),
      [](const auto& a, const auto& b) {
        if (a.second != b.second)
        {
          return a.second > b.second;
        }
        return a.first < b.first;
      });

    size_t count = 0;
    std::for_each(items.begin(), items.end(),
      [&](const auto& p) {
        if (count >= (size_t)n)
        {
          return;
        }
        os << p.first << " " << p.second << "\n";
        count++;
      });
  }

  void leastFreq(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    int n;
    if (!(is >> n) || n <= 0)
    {
      os << "<INVALID COMMAND>\n";
      return;
    }

    const auto* dict = dm.getCurrentDict();
    if (!dict || dict->empty())
    {
      os << "<EMPTY>\n";
      return;
    }

    std::vector< std::pair< std::string, int > > items(dict->begin(), dict->end());
    std::sort(items.begin(), items.end(),
      [](const auto& a, const auto& b) {
        if (a.second != b.second)
        {
          return a.second < b.second;
        }
        return a.first < b.first;
      });

      size_t count = 0;
      std::for_each(items.begin(), items.end(),
      [&](const auto& p) {
        if (count >= (size_t)n)
        {
          return;
        }
        os << p.first << " " << p.second << "\n";
        count++;
      });
  }

  void mergeCmd(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string otherName, newDictName;
    is >> otherName >> newDictName;
    if (otherName.empty() || newDictName.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (!dm.mergeDictionaries(otherName, newDictName))
    {
      os << "<INVALID COMMAND>\n";
    }
  }

  void intersectCmd(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string otherName, newDictName;
    is >> otherName >> newDictName;
    if (otherName.empty() || newDictName.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (!dm.intersectDictionaries(otherName, newDictName))
    {
      os << "<INVALID COMMAND>\n";
    }
  }

  void loadCmd(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string filename;
    is >> filename;

    if (filename.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }

    if (!dm.loadFromFile(filename))
    {
      os << "<FILE NOT FOUND>\n";
    }
    else
    {
      os << "<LOAD SUCCESS>\n";
    }
  }

  void saveCmd(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string filename;
    is >> filename;
    if (filename.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (!dm.saveToFile(filename))
    {
      os << "<INVALID COMMAND>\n";
    }
  }

  void filterByPatternCmd(std::istream& is, DictionaryManager& dm, std::ostream& os)
  {
    std::string regex_str, resultDictName;
    is >> regex_str >> resultDictName;
    if (regex_str.empty() || resultDictName.empty())
    {
      os << "<INVALID COMMAND>\n";
      return;
    }

    try
    {
      if (!dm.filterByPattern(regex_str, resultDictName))
      {
        os << "<INVALID COMMAND>\n";
      }
    }
    catch (const std::regex_error&)
    {
      os << "<INVALID REGEX>\n";
    }
  }
}

std::map< std::string, smirnov::CommandFunction > smirnov::createCommandMap()
{
  return
  {
    {"add", smirnov::addWord},
    {"remove", smirnov::removeWord},
    {"freq", smirnov::freq},
    {"createdict", smirnov::createDictCmd},
    {"deletedict", smirnov::deleteDictCmd},
    {"pushd", smirnov::pushDictCmd},
    {"popd", smirnov::popDictCmd},
    {"stacklist", smirnov::stackListCmd},
    {"list", smirnov::list},
    {"mostfreq", smirnov::mostFreq},
    {"leastfreq", smirnov::leastFreq},
    {"merge", smirnov::mergeCmd},
    {"intersect", smirnov::intersectCmd},
    {"load", smirnov::loadCmd},
    {"save", smirnov::saveCmd},
    {"filter_by_pattern", smirnov::filterByPatternCmd}
  };
}
