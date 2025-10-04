#include "commands.hpp"
#include <algorithm>
#include <vector>
#include <string>
#include <ostream>
#include <stdexcept>
#include "dictionary_manager.hpp"

namespace shramko
{
  bool compareByFreqDesc(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
  {
    if (a.second != b.second)
    {
      return a.second > b.second;
    }
    return a.first < b.first;
  }

  bool compareByFreqAsc(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
  {
    if (a.second != b.second)
    {
      return a.second < b.second;
    }
    return a.first < b.first;
  }

  bool compareByKey(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
  {
    return a.first < b.first;
  }
}

void create(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 1)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string name = args[0];
  if (dm.createDict(name))
  {
    os << "OK: Dictionary '" << name << "' created\n";
  }
  else
  {
    os << "DICT EXISTS\n";
  }
}

void add(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 2 && args.size() != 3)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string dict_name = args[0];
  std::string word = args[1];
  int freq = 1;
  if (args.size() == 3)
  {
    std::string freq_part = args[2];
    if (freq_part.substr(0, 5) != "freq:")
    {
      os << "INVALID COMMAND\n";
      return;
    }
    try
    {
      freq = std::stoi(freq_part.substr(5));
    }
    catch (...)
    {
      os << "INVALID COMMAND\n";
      return;
    }
  }
  if (dm.addWord(dict_name, word, freq))
  {
    os << "OK: Added '" << word << "' with freq=" << freq << "\n";
  }
  else
  {
    os << "DICT NOT FOUND\n";
  }
}

void increment(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 2 && args.size() != 3)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string dict_name = args[0];
  std::string word = args[1];
  int by = 1;
  if (args.size() == 3)
  {
    std::string by_part = args[2];
    if (by_part.substr(0, 3) != "by:")
    {
      os << "INVALID COMMAND\n";
      return;
    }
    try
    {
      by = std::stoi(by_part.substr(3));
    }
    catch (...)
    {
      os << "INVALID COMMAND\n";
      return;
    }
  }
  if (dm.addWord(dict_name, word, by))
  {
    os << "OK: Incremented '" << word << "' by " << by << "\n";
  }
  else
  {
    os << "DICT NOT FOUND\n";
  }
}

void search(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 2)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string dict_name = args[0];
  std::string word = args[1];
  int freq = 0;
  if (dm.getFreq(dict_name, word, freq))
  {
    os << "FOUND: freq=" << freq << "\n";
  }
  else
  {
    os << "NOT FOUND\n";
  }
}

void delete_(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 2)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string dict_name = args[0];
  std::string word = args[1];
  if (dm.removeWord(dict_name, word))
  {
    os << "OK: Deleted '" << word << "'\n";
  }
  else
  {
    os << "WORD NOT FOUND\n";
  }
}

void dump(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 1)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string dict_name = args[0];
  const auto* dict = dm.getDict(dict_name);
  if (!dict)
  {
    os << "DICT NOT FOUND\n";
    return;
  }
  os << "dump: ";
  for (const auto& p : *dict)
  {
    os << p.first << ":" << p.second << " ";
  }
  os << "\n";
}

void top(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 2)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string dict_name = args[0];
  std::string n_str = args[1];
  int n = 0;
  try
  {
    n = std::stoi(n_str);
  }
  catch (...)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  if (n <= 0)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  const auto* dict = dm.getDict(dict_name);
  if (!dict || dict->empty())
  {
    os << "EMPTY\n";
    return;
  }
  std::vector< std::pair< std::string, int > > items(dict->begin(), dict->end());
  std::sort(items.begin(), items.end(), shramko::compareByFreqDesc);
  os << "top " << n << ": ";
  size_t count = 0;
  for (const auto& p : items)
  {
    if (count >= static_cast< size_t >(n))
    {
      break;
    }
    os << p.first << ":" << p.second << " ";
    count++;
  }
  os << "\n";
}

void bot(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 2)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string dict_name = args[0];
  std::string n_str = args[1];
  int n = 0;
  try
  {
    n = std::stoi(n_str);
  }
  catch (...)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  if (n <= 0)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  const auto* dict = dm.getDict(dict_name);
  if (!dict || dict->empty())
  {
    os << "EMPTY\n";
    return;
  }
  std::vector< std::pair< std::string, int > > items(dict->begin(), dict->end());
  std::sort(items.begin(), items.end(), shramko::compareByFreqAsc);
  os << "BOT " << n << ": ";
  size_t count = 0;
  for (const auto& p : items)
  {
    if (count >= static_cast< size_t >(n))
    {
      break;
    }
    os << p.first << ":" << p.second << " ";
    count++;
  }
  os << "\n";
}

void minfreq(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 2)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string dict_name = args[0];
  std::string min_str = args[1];
  int min_val = 0;
  try
  {
    min_val = std::stoi(min_str);
  }
  catch (...)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  const auto* dict = dm.getDict(dict_name);
  if (!dict)
  {
    os << "DICT NOT FOUND\n";
    return;
  }
  std::vector< std::pair< std::string, int > > items;
  for (const auto& p : *dict)
  {
    if (p.second >= min_val)
    {
      items.push_back(p);
    }
  }
  std::sort(items.begin(), items.end(), shramko::compareByKey);
  os << "minfreq " << min_val << ": ";
  for (const auto& p : items)
  {
    os << p.first << ":" << p.second << " ";
  }
  os << "\n";
}

void maxfreq(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 2)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string dict_name = args[0];
  std::string max_str = args[1];
  int max_val = 0;
  try
  {
    max_val = std::stoi(max_str);
  }
  catch (...)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  const auto* dict = dm.getDict(dict_name);
  if (!dict)
  {
    os << "DICT NOT FOUND\n";
    return;
  }
  std::vector< std::pair< std::string, int > > items;
  for (const auto& p : *dict)
  {
    if (p.second > max_val)
    {
      items.push_back(p);
    }
  }
  std::sort(items.begin(), items.end(), shramko::compareByKey);
  os << "maxfreq " << max_val << ": ";
  for (const auto& p : items)
  {
    os << p.first << ":" << p.second << " ";
  }
  os << "\n";
}

void median(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os)
{
  if (args.size() != 1)
  {
    os << "INVALID COMMAND\n";
    return;
  }
  std::string dict_name = args[0];
  const auto* dict = dm.getDict(dict_name);
  if (!dict)
  {
    os << "DICT NOT FOUND\n";
    return;
  }
  if (dict->empty())
  {
    os << "EMPTY DICT\n";
    return;
  }
  std::vector< int > freqs;
  for (const auto& p : *dict)
  {
    freqs.push_back(p.second);
  }
  std::sort(freqs.begin(), freqs.end());
  size_t sz = freqs.size();
  double med = 0.0;
  if (sz % 2 == 1)
  {
    med = freqs[sz / 2];
  }
  else
  {
    med = (freqs[sz / 2 - 1] + freqs[sz / 2]) / 2.0;
  }
  os << "median: " << med << "\n";
}

struct OneArgCaller
{
  void (*func)(const std::vector< std::string >&, DictionaryManager&, std::ostream&);
  OneArgCaller(void (*f)(const std::vector< std::string >&, DictionaryManager&, std::ostream&)) : func(f) {}
  void operator()(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os) const
  {
    func(args, dm, os);
  }
};

std::map< std::string, CommandFunction > createCommandMap()
{
  std::map< std::string, CommandFunction > commandMap;
  commandMap["create"] = OneArgCaller(create);
  commandMap["add"] = OneArgCaller(add);
  commandMap["increment"] = OneArgCaller(increment);
  commandMap["search"] = OneArgCaller(search);
  commandMap["delete"] = OneArgCaller(delete_);
  commandMap["dump"] = OneArgCaller(dump);
  commandMap["top"] = OneArgCaller(top);
  commandMap["bot"] = OneArgCaller(bot);
  commandMap["minfreq"] = OneArgCaller(minfreq);
  commandMap["maxfreq"] = OneArgCaller(maxfreq);
  commandMap["median"] = OneArgCaller(median);
  return commandMap;
}
