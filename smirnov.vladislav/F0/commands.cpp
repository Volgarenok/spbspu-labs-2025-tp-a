#include "commands.hpp"
#include <algorithm>
#include <iterator>
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
  struct PrintNamePredicate
  {
    std::ostream& os_;
    PrintNamePredicate(std::ostream& os) :
      os_(os)
    {}
    void operator()(const std::string& name) const
    {
        os_ << name << "\n";
    }
  };

  using CommandFunction = std::function< void(const std::vector< std::string >&, DictionaryManager&, std::ostream&) >;

  struct AddWordCaller
  {
    void operator()(const std::vector< std::string >& args, DictionaryManager& dm_param, std::ostream& os_param) const
    {
      if (args.empty() || args.size() > 2)
      {
        os_param << "<INVALID COMMAND>\n";
        return;
      }
      std::string word = args[0];
      std::string freq_str = (args.size() > 1) ? args[1] : "";
      addWord(word, freq_str, dm_param, os_param);
    }
  };

  struct OneArgCommandCaller
  {
    void (*cmd_func_)(const std::string&, DictionaryManager&, std::ostream&);
    OneArgCommandCaller(void (*func)(const std::string&, DictionaryManager&, std::ostream&)) :
      cmd_func_(func)
    {}
    void operator()(const std::vector< std::string >& args, DictionaryManager& dm_param, std::ostream& os_param) const
    {
      if (args.size() != 1)
      {
        os_param << "<INVALID COMMAND>\n";
        return;
      }
      cmd_func_(args[0], dm_param, os_param);
    }
  };

  struct TwoArgCommandCaller
  {
    void (*cmd_func_)(const std::string&, const std::string&, DictionaryManager&, std::ostream&);
    TwoArgCommandCaller(void (*func)(const std::string&, const std::string&, DictionaryManager&, std::ostream&)) :
        cmd_func_(func)
    {}
    void operator()(const std::vector< std::string >& args, DictionaryManager& dm_param, std::ostream& os_param) const
    {
      if (args.size() != 2)
      {
        os_param << "<INVALID COMMAND>\n";
        return;
      }
      cmd_func_(args[0], args[1], dm_param, os_param);
    }
  };

  struct ZeroArgCommandCaller
  {
    void (*cmd_func_)(DictionaryManager&, std::ostream&);
    ZeroArgCommandCaller(void (*func)(DictionaryManager&, std::ostream&)) :
        cmd_func_(func)
    {}
    void operator()(const std::vector< std::string >& args, DictionaryManager& dm_param, std::ostream& os_param) const
    {
      if (!args.empty())
      {
        os_param << "<INVALID COMMAND>\n";
        return;
      }
      cmd_func_(dm_param, os_param);
    }
  };

  bool comparePairByFirst(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
  {
    return a.first < b.first;
  }

  struct PrintPairPredicate
  {
    std::ostream& os_;
    PrintPairPredicate(std::ostream& os) :
      os_(os)
    {}
    void operator()(const std::pair< std::string, int >& p) const
    {
      os_ << p.first << " " << p.second << "\n";
    }
  };

  bool comparePairByFreqDesc(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
  {
    if (a.second != b.second)
    {
      return a.second > b.second;
    }
    return a.first < b.first;
  }

  struct PrintLimitedPairPredicate
  {
    std::ostream& os_;
    size_t& count_;
    size_t limit_;
    PrintLimitedPairPredicate(std::ostream& os, size_t& count, size_t limit) :
      os_(os),
      count_(count),
      limit_(limit)
    {}
    void operator()(const std::pair< std::string, int >& p) const
    {
      if (count_ >= limit_)
      {
        return;
      }
      os_ << p.first << " " << p.second << "\n";
      count_++;
    }
  };

  bool comparePairByFreqAsc(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
  {
    if (a.second != b.second)
    {
      return a.second < b.second;
    }
    return a.first < b.first;
  }

  void addWord(const std::string& word, const std::string& freq_str, DictionaryManager& dm, std::ostream& os)
  {
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
    if (!freq_str.empty())
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

  void removeWord(const std::string& word, DictionaryManager& dm, std::ostream& os)
  {
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

  void freq(const std::string& word, DictionaryManager& dm, std::ostream& os)
  {
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

  void createDictCmd(const std::string& name, DictionaryManager& dm, std::ostream& os)
  {
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

  void deleteDictCmd(const std::string& name, DictionaryManager& dm, std::ostream& os)
  {
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

  void pushDictCmd(const std::string& name, DictionaryManager& dm, std::ostream& os)
  {
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

  void popDictCmd(DictionaryManager& dm, std::ostream& os)
  {
    if (!dm.popDict())
    {
      os << "<INVALID COMMAND>\n";
    }
  }

  void stackListCmd(DictionaryManager& dm, std::ostream& os)
  {
    const auto& stack = dm.getStack();
    if (stack.empty())
    {
      os << "<EMPTY>\n";
    }
    else
    {
      std::for_each(stack.begin(), stack.end(), PrintNamePredicate(os));
    }
  }

  void list(DictionaryManager& dm, std::ostream& os)
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
    std::sort(items.begin(), items.end(), comparePairByFirst);
    std::for_each(items.begin(), items.end(), PrintPairPredicate(os));
  }

  void mostFreq(const std::string& n_str, DictionaryManager& dm, std::ostream& os)
  {
    int n;
    try
    {
      n = std::stoi(n_str);
    }
    catch (const std::exception&)
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if ( n <= 0)
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
    std::sort(items.begin(), items.end(), comparePairByFreqDesc);
    size_t count = 0;
    std::for_each(items.begin(), items.end(), PrintLimitedPairPredicate(os, count, static_cast< size_t >(n)));
  }

  void leastFreq(const std::string& n_str, DictionaryManager& dm, std::ostream& os)
  {
    int n;
    try
    {
      n = std::stoi(n_str);
    }
    catch (const std::exception&)
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (n <= 0)
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
    std::sort(items.begin(), items.end(), comparePairByFreqAsc);
    size_t count = 0;
    std::for_each(items.begin(), items.end(), PrintLimitedPairPredicate(os, count, static_cast< size_t >(n)));
  }

  void mergeDictionariesCmd(const std::string& otherName, const std::string& newDictName, DictionaryManager& dm, std::ostream& os)
  {
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

  void intersectDictionariesCmd(const std::string& otherName, const std::string& newDictName, DictionaryManager& dm, std::ostream& os)
  {
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

  void loadFromFileCmd(const std::string& filename, DictionaryManager& dm, std::ostream& os)
  {
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

  void saveToFileCmd(const std::string& filename, DictionaryManager& dm, std::ostream& os)
  {
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

  void filterByPatternCmd(const std::string& regex_str, const std::string& resultDictName, DictionaryManager& dm, std::ostream& os)
  {
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

  std::map< std::string, CommandFunction > createCommandMap(DictionaryManager& dm, std::ostream& os)
  {
    std::map< std::string, CommandFunction > commandMap;

    commandMap["add"] = CommandFunction(AddWordCaller());
    commandMap["remove"] = CommandFunction(OneArgCommandCaller(removeWord));
    commandMap["freq"] = CommandFunction(OneArgCommandCaller(freq));
    commandMap["createdict"] = CommandFunction(OneArgCommandCaller(createDictCmd));
    commandMap["deletedict"] = CommandFunction(OneArgCommandCaller(deleteDictCmd));
    commandMap["pushd"] = CommandFunction(OneArgCommandCaller(pushDictCmd));
    commandMap["mostfreq"] = CommandFunction(OneArgCommandCaller(mostFreq));
    commandMap["leastfreq"] = CommandFunction(OneArgCommandCaller(leastFreq));
    commandMap["load"] = CommandFunction(OneArgCommandCaller(loadFromFileCmd));
    commandMap["save"] = CommandFunction(OneArgCommandCaller(saveToFileCmd));
    commandMap["merge"] = CommandFunction(TwoArgCommandCaller(mergeDictionariesCmd));
    commandMap["intersect"] = CommandFunction(TwoArgCommandCaller(intersectDictionariesCmd));
    commandMap["filter_by_pattern"] = CommandFunction(TwoArgCommandCaller(filterByPatternCmd));
    commandMap["popd"] = CommandFunction(ZeroArgCommandCaller(popDictCmd));
    commandMap["stacklist"] = CommandFunction(ZeroArgCommandCaller(stackListCmd));
    commandMap["list"] = CommandFunction(ZeroArgCommandCaller(list));

    return commandMap;
  }
}

