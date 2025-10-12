#include "commands.hpp"
#include "text_processor.hpp"
#include <algorithm>
#include <functional>
#include <vector>
#include <iterator>
#include <fstream>
#include <string>
#include <utility>

namespace shchadilov
{
  void help(std::ostream& out)
  {
    out << "List of commands\n";
    out << " - create <name>	- create new dictionary\n";
    out << " - remove <name>	- remove dictionary\n";
    out << " - list	- list all dictionaries\n";
    out << " - build <dict> <file>	- build dictionary from file\n";
    out << " - show <dict>	- show dictionary contents\n";
    out << " - clear <dict>	- clear dictionary\n";
    out << " - merge <source1> <source2> <result> - merge two dictionaries into a new dictionary\n";
    out << " - intersect <dict1> <dict2> <result>	- intersect two dictionaries into result\n";
    out << " - diff <dict1> <dict2> <result>	- words in dict1 but not in dict2\n";
    out << " - unionavg <dict1> <dict2> <result>	- union with averaged frequencies\n";
    out << " - compare <dict1> <dict2>	- compare two dictionaries and print stats\n";
    out << " - wordfreq <word> <dict>	- show frequency of a word in a dictioary\n";
    out << " - topwords <N> <dict>	- show top-N words in a dictioary\n";
    out << " - medianword <dict>	- show word(s) with median frequency\n";
    out << " - save <filename>	- save all dictionaries to file\n";
    out << " - load <filename>	- load all dictionaries from file\n";
  }

  void create(DictionarySet& set, std::istream& in)
  {
    std::string name = "";;
    in >> name;
    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }
    if (set.count(name) > 0)
    {
      throw std::logic_error("<DICTIOARY EXISTS>\n");
    }
    set[name];
    std::cout << "Dictionary '" << name << "' created.\n";
  }

  void dismiss(DictionarySet& set, std::istream& in)
  {
    std::string name = "";;
    in >> name;
    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }
    if (set.count(name) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }
    set.erase(name);
    std::cout << "Dictionary '" << name << "' removed.\n";
  }

  struct ExtractKey
  {
    std::string operator()(const std::pair< const std::string, FrequencyDictionary >& pair) const
    {
      return pair.first;
    }
  };

  void list_dicts(const DictionarySet& set, std::ostream& out)
  {
    if (set.empty())
    {
      out << "No dictionaries available\n";
      return;
    }

    out << "Available dictionaries:\n";
    std::transform(set.cbegin(), set.cend(), std::ostream_iterator< std::string >(out, "\n"), ExtractKey{});
  }

  struct WordAdder
  {
    FrequencyDictionary& dict_;

    void operator()(const std::string& word) const
    {
      ++dict_[word];
    }
  };

  void build(DictionarySet& set, std::istream& in)
  {
    std::string dictName = "";
    std::string fileName = "";
    in >> dictName >> fileName;

    std::ifstream file(fileName);
    if (!file.is_open() || file.peek() == EOF)
    {
      throw std::logic_error("<INVALID FILE>\n");
    }

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }
    if (set.count(dictName) == 0)
    {
      set[dictName];
    }

    const std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::vector< std::string > words = extractWords(content);
    WordAdder adder
    {
      set.at(dictName)
    };
    std::for_each(words.begin(), words.end(), adder);
    std::cout << "Dictionary '" << dictName << "' built. Added " << words.size() << " words.\n";
  }

  struct PairToString
  {
    std::string operator()(const std::pair<std::string, std::size_t>& p) const
    {
      return p.first + ": " + std::to_string(p.second);
    }
  };

  std::ostream& operator<<(std::ostream& out, const FrequencyDictionary& dict)
  {
    if (dict.empty())
    {
      return out;
    }

    std::vector< std::pair< std::string, std::size_t > > sortedWords(dict.begin(), dict.end());
    std::sort(sortedWords.begin(), sortedWords.end());

    std::vector< std::string > lines;
    lines.reserve(sortedWords.size());
    std::transform(sortedWords.begin(), sortedWords.end(), std::back_inserter(lines), PairToString());

    std::copy(lines.begin(), lines.end(), std::ostream_iterator<std::string>(out, "\n"));
    return out;
  }

  struct NameExtractor
  {
    std::string operator()(const std::pair< const std::string, FrequencyDictionary >& p) const
    {
      return p.first;
    }
  };

  struct DictPrinter
  {
    const DictionarySet& set_;
    std::ostream& out_;

    DictPrinter(const DictionarySet& s, std::ostream& o):
      set_(s),
      out_(o)
    {}

    void operator()(const std::string& name) const
    {
      out_ << "Dictionary: " << name << "\n";
      out_ << set_.at(name);
      out_ << "---\n";
    }
  };

  std::ostream& operator<<(std::ostream& out, const DictionarySet& set)
  {
    if (set.empty())
    {
      out << "No dictionaries available\n";
      return out;
    }

    std::vector< std::string > names;
    names.reserve(set.size());
    std::transform(set.begin(), set.end(), std::back_inserter(names), NameExtractor());
    std::sort(names.begin(), names.end());
    auto bound_printer = std::bind(&DictPrinter::operator(), DictPrinter(set, out), std::placeholders::_1);
    std::for_each(names.begin(), names.end(), bound_printer);

    return out;
  }

  void show(const DictionarySet& set, std::istream& in, std::ostream& out)
  {
    std::string name = "";
    in >> name;

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }

    if (set.count(name) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }
    const FrequencyDictionary& dict = set.at(name);
    out << "Dictionary '" << name << "' ( " << dict.size() << " unique words):\n";
    out << dict;
  }

  void clear(DictionarySet& set, std::istream& in)
  {
    std::string name = "";
    in >> name;
    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }
    if (set.count(name) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }
    set.at(name).clear();
    std::cout << "Dictionary '" << name << "' cleared.\n";
  }

  struct MergeInserter
  {
    FrequencyDictionary& target_;
    void operator()(const std::pair< std::string, std::size_t >& pair) const
    {
      target_[pair.first] += pair.second;
    }
  };

  void merge(DictionarySet& set, std::istream& in, std::ostream& out)
  {
    std::string source1 = "";
    std::string source2 = "";
    std::string resultDict = "";
    in >> source1 >> source2 >> resultDict;

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }
    if (set.count(source1) == 0 || set.count(source2) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }
    if (set.count(resultDict) > 0)
    {
      throw std::logic_error("<RESULT DICTIONARY ALREADY EXISTS>\n");
    }

    set[resultDict] = set.at(source1);
    std::for_each(set.at(source2).cbegin(), set.at(source2).cend(), MergeInserter{ set.at(resultDict) });

    out << "Dictionaries '" << source1 << "' and '" << source2
      << "' merged into new dictionary '" << resultDict << "'.\n";
    out << "New dictionary contains " << set.at(resultDict).size() << " unique words.\n";
  }

  struct IntersectChecker
  {
    const FrequencyDictionary& source_;

    bool operator()(const std::pair< std::string, size_t >& pair) const
    {
      return source_.count(pair.first) > 0;
    }
  };

  struct MinFrequencySetter
  {
    const FrequencyDictionary& dict1_;
    const FrequencyDictionary& dict2_;
    FrequencyDictionary& result_;

    void operator()(const std::pair< std::string, size_t >& pair) const
    {
      auto it = dict2_.find(pair.first);
      if (it != dict2_.end())
      {
        result_[pair.first] = std::min(pair.second, it->second);
      }
    }
  };

  void intersect(DictionarySet& set, std::istream& in)
  {
    std::string dict1 = "";
    std::string dict2 = "";
    std::string resultDict = "";
    in >> dict1 >> dict2 >> resultDict;

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }
    if (set.count(dict1) == 0 || set.count(dict2) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }

    set[resultDict].clear();

    MinFrequencySetter setter
    {
      set.at(dict1), set.at(dict2), set.at(resultDict)
    };
    std::for_each(set.at(dict1).cbegin(), set.at(dict1).cend(), setter);
    std::cout << "Dictionaries '" << dict1 << "' and '" << dict2 << "' intersected into '"
      << resultDict << "' with " << set.at(resultDict).size() << " unique words.\n";
  }

  struct WordPairWriter
  {
    std::ofstream& file_;

    WordPairWriter(std::ofstream& f):
      file_(f)
    {}

    void operator()(const std::pair< std::string, std::size_t >& wordPair) const
    {
      file_ << wordPair.first << " " << wordPair.second << "\n";
    }
  };

  struct DictionarySaver
  {
    std::ofstream& file_;

    void operator()(const std::pair< std::string, FrequencyDictionary >& dictPair) const
    {
      const std::string& dictName = dictPair.first;
      const FrequencyDictionary& dict = dictPair.second;

      file_ << dictName << " " << dict.size() << "\n";

      std::vector<std::pair< std::string, std::size_t >> words(dict.begin(), dict.end());
      std::sort(words.begin(), words.end());

      std::for_each(words.begin(), words.end(), WordPairWriter(file_));
    }
  };
  void save(const DictionarySet& set, std::istream& in)
  {
    std::string filename = "";
    in >> filename;

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }

    std::ofstream file(filename);
    if (!file.is_open())
    {
      throw std::logic_error("<UNABLE TO OPEN FILE>\n");
    }

    file << set.size() << "\n";
    const DictionarySaver saver
    {
      file
    };
    std::for_each(set.begin(), set.end(), saver);
  }

  class DictionaryLoader
  {
  public:
    DictionaryLoader(std::ifstream& file, DictionarySet& set):
      file_(file), set_(set)
    {}

    void operator()(size_t) const
    {
      std::string dictName = "";
      size_t wordCount;

      if (!(file_ >> dictName >> wordCount))
      {
        throw std::logic_error("<INVALID FILE FORMAT>\n");
      }

      FrequencyDictionary dict;
      std::string word = "";
      std::size_t frequency;

      for (size_t i = 0; i < wordCount; ++i)
      {
        if (!(file_ >> word >> frequency))
        {
          throw std::logic_error("<INVALID FILE FORMAT>\n");
        }
        dict[word] = frequency;
      }

      set_[dictName] = dict;
    }

  private:
    std::ifstream& file_;
    DictionarySet& set_;
  };

  struct IndexGenerator
  {
    DictionaryLoader& loader;
    size_t index = 0;

    size_t operator()()
    {
      loader(index++);
      return 0;
    }
  };

  void load(DictionarySet& set, std::istream& in)
  {
    std::string filename = "";
    in >> filename;

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }

    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::logic_error("<UNABLE TO OPEN FILE>\n");
    }

    size_t dictCount;
    if (!(file >> dictCount))
    {
      throw std::logic_error("<INVALID FILE FORMAT>\n");
    }

    const DictionaryLoader loader(file, set);
    for (size_t i = 0; i < dictCount; ++i)
    {
      loader(i);
    }
  }

  void wordfreq(const DictionarySet& set, std::istream& in, std::ostream& out)
  {
    std::string word = "";
    in >> word;
    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }

    std::string dictName = "";
    in >> dictName;

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }

    if (set.count(dictName) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }

    auto it = set.at(dictName).find(word);
    std::size_t freq;
    if (it != set.at(dictName).end())
    {
      freq = it->second;
    }
    else
    {
      freq = 0;
    }
    out << "Dictionary: " << dictName << " -> " << freq << "\n";
  }
  struct ByFrequencyDesc
  {
    bool operator()(const std::pair< std::string, std::size_t >& a, const std::pair< std::string, std::size_t >& b) const
    {
      if (a.second == b.second)
      {
        return a.first < b.first;
      }
      return a.second > b.second;
    }
  };

  struct PairPrinter
  {
    std::ostream& out_;

    PairPrinter(std::ostream& o):
      out_(o)
    {}

    void operator()(const std::pair< std::string, std::size_t >& p) const
    {
      out_ << p.first << ": " << p.second << "\n";
    }
  };

  struct TopWordsPrinter
  {
    const std::string& dictName_;
    const FrequencyDictionary& dict_;
    int N_;
    std::ostream& out_;

    TopWordsPrinter(const std::string& name, const FrequencyDictionary& d, int n, std::ostream& o): 
      dictName_(name), dict_(d), N_(n), out_(o)
    {}

    void operator()() const
    {
      out_ << "Dictionary: " << dictName_ << "\n";
      if (dict_.empty()) {
        out_ << "(empty)\n";
        return;
      }

      std::vector< std::pair< std::string, std::size_t > > words(dict_.begin(), dict_.end());
      std::sort(words.begin(), words.end(), ByFrequencyDesc());

      if (N_ >= 0 && words.size() > static_cast<std::size_t>(N_))
      {
        words.resize(N_);
      }

      std::for_each(words.begin(), words.end(), PairPrinter(out_));
    }
  };

  void topwords(const DictionarySet& set, std::istream& in, std::ostream& out)
  {
    int N;
    in >> N;
    if (!in || N < 0)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }

    std::string dictName = "";
    in >> dictName;

    if (set.count(dictName) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }

    const TopWordsPrinter printer(dictName, set.at(dictName), N, out);
    printer();
  }
  struct DiffFilter
  {
    const FrequencyDictionary& dict2;
    bool operator()(const std::pair< std::string, std::size_t >& p) const
    {
      return dict2.count(p.first) == 0;
    }
  };

  void diff(DictionarySet& set, std::istream& in, std::ostream& out)
  {
    std::string dict1 = "";
    std::string dict2 = "";
    std::string resultDict = "";
    in >> dict1 >> dict2 >> resultDict;

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }
    if (set.count(dict1) == 0 || set.count(dict2) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }

    set[resultDict].clear();

    DiffFilter filter{ set.at(dict2) };
    std::copy_if(
      set.at(dict1).begin(), set.at(dict1).end(),
      std::inserter(set.at(resultDict), set.at(resultDict).end()),
      filter);

    out << "Dictionary '" << resultDict << "' created with " << set.at(resultDict).size() << " words.\n";
  }
  struct UnionAvgInserter
  {
    FrequencyDictionary& result_;
    const FrequencyDictionary& other_;

    void operator()(const std::pair< std::string, std::size_t >& p) const
    {
      auto it = other_.find(p.first);
      if (it != other_.end())
      {
        result_[p.first] = (p.second + it->second) / 2;
      }
      else
      {
        result_[p.first] = p.second;
      }
    }
  };

  struct UnionAvgCompleter
  {
    FrequencyDictionary& result_;
    const FrequencyDictionary& base_;

    void operator()(const std::pair< std::string, std::size_t >& p) const
    {
      if (result_.count(p.first) == 0)
      {
        result_[p.first] = p.second;
      }
    }
  };

  void unionavg(DictionarySet& set, std::istream& in, std::ostream& out)
  {
    std::string dict1 = "";
    std::string dict2 = "";
    std::string resultDict = "";
    in >> dict1 >> dict2 >> resultDict;

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }
    if (set.count(dict1) == 0 || set.count(dict2) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }

    set[resultDict].clear();

    UnionAvgInserter inserter{ set[resultDict], set.at(dict2) };
    std::for_each(set.at(dict1).begin(), set.at(dict1).end(), inserter);

    UnionAvgCompleter completer{ set[resultDict], set.at(dict1) };
    std::for_each(set.at(dict2).begin(), set.at(dict2).end(), completer);

    out << "Dictionary '" << resultDict
      << "' created with " << set.at(resultDict).size() << " words.\n";
  }
  struct CompareResult
  {
    std::size_t common_;
    std::size_t unique1_;
    std::size_t unique2_;
    double jaccard_;
  };

  std::ostream& operator<<(std::ostream& out, const CompareResult& r)
  {
    out << "Common words: " << r.common_ << "\n";
    out << "Unique in dict1: " << r.unique1_ << "\n";
    out << "Unique in dict2: " << r.unique2_ << "\n";
    out << "Jaccard similarity: " << r.jaccard_ << "\n";
    return out;
  }

  struct CommonCounter
  {
    const FrequencyDictionary& other_;
    std::size_t& counter_;

    void operator()(const std::pair< std::string, std::size_t >& p) const
    {
      if (other_.count(p.first) > 0)
      {
        ++counter_;
      }
    }
  };

  void compare(const DictionarySet& set, std::istream& in, std::ostream& out)
  {
    std::string dict1 = "";
    std::string dict2 = "";
    in >> dict1 >> dict2;

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }
    if (set.count(dict1) == 0 || set.count(dict2) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }

    const auto& d1 = set.at(dict1);
    const auto& d2 = set.at(dict2);

    std::size_t common = 0;
    std::size_t unique1 = 0;
    std::size_t unique2 = 0;

    CommonCounter counter1{ d2, common };
    std::for_each(d1.begin(), d1.end(), counter1);

    DiffFilter filter1{ d2 };
    unique1 = std::count_if(d1.begin(), d1.end(), filter1);

    DiffFilter filter2{ d1 };
    unique2 = std::count_if(d2.begin(), d2.end(), filter2);

    std::size_t unionSize = common + unique1 + unique2;
    double jaccard;
    if (unionSize > 0)
    {
      jaccard = static_cast<double>(common) / unionSize;
    }
    else
    {
      jaccard = 0.0;
    }

    const CompareResult result
    {
      common, unique1, unique2, jaccard
    };
    out << result;
  }

  struct CompareByFrequency
  {
    bool operator()(const std::pair< std::string, std::size_t >& a,
      const std::pair< std::string, std::size_t >& b) const
    {
      if (a.second != b.second)
      {
        return a.second < b.second;
      }
      return a.first < b.first;
    }
  };

  struct PrintPair
  {
    std::ostream& out_;
    PrintPair(std::ostream& o):
      out_(o)
    {}
    void operator()(const std::pair< std::string, std::size_t >& p) const
    {
      out_ << p.first << " " << p.second << "\n";
    }
  };

  void medianword(const DictionarySet& set, std::istream& in, std::ostream& out)
  {
    std::string dictName = "";
    in >> dictName;

    if (!in)
    {
      throw std::logic_error("<INVALID ARGUMENT>\n");
    }

    if (set.count(dictName) == 0)
    {
      throw std::logic_error("<DICTIONARY DOES NOT EXIST>\n");
    }

    const FrequencyDictionary& dict = set.at(dictName);
    if (dict.empty())
    {
      out << "Dictionary is empty.\n";
      return;
    }

    std::vector< std::pair< std::string, std::size_t > > words;
    words.reserve(dict.size());
    std::copy(dict.begin(), dict.end(), std::back_inserter(words));
    std::sort(words.begin(), words.end(), CompareByFrequency{});

    size_t n = words.size();
    if (n % 2 == 1)
    {
      auto it = words.begin() + n / 2;
      PrintPair printer(out);
      printer(*it);
    }
    else
    {
      auto it1 = words.begin() + n / 2 - 1;
      auto it2 = words.begin() + n / 2;
      PrintPair printer(out);
      printer(*it1);
      printer(*it2);
    }
  }
}
