#include "commands.hpp"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <set>
#include <iterator>


namespace
{
  struct TranslationPrinter
  {
    std::ostream & out;
    bool first = true;

    void operator()(const std::string & tr)
    {
      if (!first) out << ",";
      out << " " << tr;
      first = false;
    }
  };

  struct TranslationFormatter
  {
    mutable bool first = true;

    std::string operator()(const std::string & tr) const
    {
      if (first)
      {
        first = false;
        return " " + tr;
      }
      return ", " + tr;
    }
  };

  struct DictPrinter
  {
    std::ostream & out;

    void operator()(const std::pair<std::string, std::vector<std::string>> & entry)
    {
      out << entry.first << ":";
      TranslationFormatter formatter;
      std::transform(entry.second.begin(), entry.second.end(), std::ostream_iterator<std::string>(out), std::ref(formatter));
      out << "\n";
    }
  };

  struct Empty {};

  struct DictWriter
  {
    std::ostream & out;

    struct AddSpace
    {
      std::ostream & out;
      Empty operator()(Empty accum, const std::string & word) const
      {
        out << " " << word;
        return accum;
      }
    };

    void operator()(const std::pair<std::string, std::vector<std::string>> & entry)
    {
      out << entry.first;
      if (!entry.second.empty())
      {
        out << " " << entry.second[0];
        std::accumulate(std::next(entry.second.begin()), entry.second.end(), Empty{}, AddSpace{ out });
      }
      out << "\n";
    }
  };

  struct FullDictWriter
  {
    std::ostream & out;
    const std::string & name;

    struct AccumulateWriter
    {
      std::ostream & out;
      Empty operator()(Empty, const std::pair<std::string, std::vector<std::string>> & entry) const
      {
        DictWriter{ out }(entry);
        return Empty{};
      }
    };

    void operator()(const bocharov::dict_t & dict) const
    {
      out << name << "\n";
      std::accumulate(dict.begin(), dict.end(), Empty{}, AccumulateWriter{ out });
      out << "\n";
    }
  };

  struct FreqComparator
  {
    bool operator()(const std::pair<std::string, int> & a, const std::pair<std::string, int> & b) const
    {
      return a.second > b.second || (a.second == b.second && a.first < b.first);
    }
  };

  struct RusRemover
  {
    const std::set<std::string> & rusSet;
    bool operator()(const std::string & tr) const
    {
      return rusSet.find(tr) != rusSet.end();
    }
  };

  struct DictMergerAccumulator
  {
    const bocharov::dict_dict_t & dicts;

    struct InsertIfNotExists
    {
      bocharov::dict_t operator()(bocharov::dict_t acc, const std::pair<std::string, std::vector<std::string>> & entry) const
      {
        if (acc.find(entry.first) == acc.end())
        {
          acc.insert(entry);
        }
        return acc;
      }
    };

    bocharov::dict_t operator()(bocharov::dict_t acc, const std::string & dictName) const
    {
      const bocharov::dict_t & dict = dicts.at(dictName);
      return std::accumulate(dict.begin(), dict.end(), std::move(acc), InsertIfNotExists{});
    }
  };

  struct AccumulatePrinter
  {
    std::ostream & out;
    Empty operator()([[maybe_unused]] Empty accum, const std::pair<std::string, std::vector<std::string>> & entry) const
    {
      DictPrinter{ out }(entry);
      return Empty{};
    }
  };

  struct CallPrinter
  {
    std::reference_wrapper<TranslationPrinter> operator()(std::reference_wrapper<TranslationPrinter> printerWrapper,
      const std::string & tr) const
    {
      printerWrapper.get()(tr);
      return printerWrapper;
    }
  };

  struct AccumulateIfContains
  {
    const std::string & target;
    bocharov::list_t operator()(bocharov::list_t acc, const bocharov::dict_t::value_type & entry) const
    {
      const auto & translations = entry.second;
      if (std::find(translations.begin(), translations.end(), target) != translations.end())
      {
        acc.push_back(entry.first);
      }
      return acc;
    }
  };

  struct WordReader
  {
    std::istream & in;
    std::string operator()() const
    {
      std::string word;
      in >> word;
      return word;
    }
  };

  struct FullDictWriterWrapper
  {
    std::ostream & out;
    const bocharov::dict_dict_t & dicts;

    Empty operator()(Empty accum, const std::string & dictName) const
    {
      FullDictWriter{ out, dictName }(dicts.at(dictName));
      return accum;
    }
  };

  struct EntryIncrementer
  {
    std::map<std::string, int> & freqs_;

    Empty operator()(Empty accum, const bocharov::dict_t::value_type & entry) const
    {
      freqs_[entry.first]++;
      return accum;
    }
  };

  struct DictProcessor
  {
    const bocharov::dict_dict_t & dicts;
    std::map<std::string, int> & freqs;

    Empty operator()(Empty accum, const std::string & dictName) const
    {
      const bocharov::dict_t & dict = dicts.at(dictName);
      return std::accumulate(dict.begin(), dict.end(), accum, EntryIncrementer{ freqs });
    }
  };

  struct DictNameReader
  {
    std::istream & in;
    std::string operator()() const
    {
      std::string s;
      in >> s;
      return s;
    }
  };

  struct CheckDictExists
  {
    const bocharov::dict_dict_t & dicts;
    bool operator()(bool valid, const std::string & dictName) const
    {
      return valid && (dicts.find(dictName) != dicts.end());
    }
  };

  using TempMap = std::map<std::string, std::pair<int, std::vector<std::string>>>;

  struct TempMapAccumulator
  {
    TempMap operator()(TempMap accum, const bocharov::dict_t::value_type & entry) const
    {
      const std::string& sln = entry.first;
      const bocharov::list_t & translations = entry.second;
      auto it = accum.find(sln);
      if (it == accum.end())
      {
        accum[sln] = std::make_pair(1, translations);
      }
      else
      {
        it->second.first++;
        it->second.second.insert(it->second.second.end(), translations.begin(), translations.end());
      }
      return accum;
    }
  };

  struct DictAccumulator
  {
    const bocharov::dict_dict_t & dicts;
    explicit DictAccumulator(const bocharov::dict_dict_t & dictsRef):
      dicts(dictsRef)
    {}

    TempMap operator()(TempMap accum, const std::string& dictName) const
    {
      const bocharov::dict_t & dict = dicts.at(dictName);
      return std::accumulate(dict.begin(), dict.end(), std::move(accum), TempMapAccumulator{});
    }
  };

  struct BuildNewDict
  {
    int K;
    explicit BuildNewDict(int k):
      K(k)
    {}

    bocharov::dict_t operator()(bocharov::dict_t acc, const TempMap::value_type & entry) const
    {
      if (entry.second.first <= K)
      {
        std::set<std::string> s(entry.second.second.begin(), entry.second.second.end());
        acc[entry.first] = bocharov::list_t(s.begin(), s.end());
      }
      return acc;
    }
  };

  struct TransformPair
  {
    std::pair< std::string, int > operator()(const std::pair< const std::string, int > & p) const
    {
      return { p.first, p.second };
    }
  };

  struct PairToString
  {
    std::string operator()(const std::pair< std::string, int > & p) const
    {
      return p.first + ' ' + std::to_string(p.second);
    }
  };

  struct DictExists
  {
    const bocharov::dict_dict_t & dicts;
    bool operator()(const std::string & dictName) const
    {
      return dicts.find(dictName) != dicts.end();
    }
  };

}


namespace bocharov
{
  void printHelp(std::ostream & out)
  {
    out << "Available commands:\n"
        << "createdict <dictname> - create empty dictionary\n"
        << "deletedict <dictname> - delete dictionary\n"
        << "printdict <dictname> - print dictionary content\n"
        << "gettranslationsln <dictname> <sln> - get translations for slang word\n"
        << "gettranslationru <dictname> <ru> - get slang words for russian word\n"
        << "deletesln <dictname> <sln> - delete slang word\n"
        << "addsln <dictname> <sln> <N> <ru1> ... <ruN> - add slang word with translations\n"
        << "addru <dictname> <sln> <N> <ru1> ... <ruN> - add translations to existing slang\n"
        << "deleteru <dictname> <sln> <N> <ru1> ... <ruN> - delete translations\n"
        << "writedicts <filename> <N> <dictname1> ... <dictnameN> - write dictionaries to file\n"
        << "union <newdict> <N> <dictname1> ... <dictnameN> - union of dictionaries\n"
        << "mostcommon <N> <K> <dictname1> ... <dictnameK> - print most common words\n"
        << "rewritefile <filename> <N> <dictname1> ... <dictnameN> - rewrite file with dictionaries\n"
        << "rare <K> <result dict> <N> <dict1> <dict2> ... <dictn> - create dictionary with words present in at most K of N dictionaries\n"
        << "help - show help\n";
  }

  void createDict(std::istream & in, dict_dict_t & dicts)
  {
    std::string dictname;
    in >> dictname;
    if (dicts.find(dictname) != dicts.end())
    {
      throw std::invalid_argument("INVALID COMMAND");
    }
    dicts[dictname] = dict_t();
  }

  void deleteDict(std::istream& in, dict_dict_t & dicts)
  {
    std::string dictname;
    in >> dictname;
    if (dicts.erase(dictname) == 0)
    {
      throw std::invalid_argument("INVALID COMMAND");
    }
  }

  void printDict(std::istream & in, std::ostream & out, const dict_dict_t & dicts)
  {
    std::string dictname;
    in >> dictname;
    const dict_t & dict = dicts.at(dictname);
    std::accumulate(dict.begin(), dict.end(), Empty{}, AccumulatePrinter{ out });
  }

  void getTranslationSln(std::istream & in, std::ostream & out, const dict_dict_t & dicts)
  {
    std::string dictname, sln;
    in >> dictname >> sln;
    const dict_t & dict = dicts.at(dictname);
    const list_t & translations = dict.at(sln);

    out << sln << ":";

    TranslationPrinter printer{ out };
    printer.first = true;

    std::accumulate(translations.begin(), translations.end(), std::ref(printer), CallPrinter{});
    out << "\n";
  }

  void getTranslationRu(std::istream & in, std::ostream & out, const dict_dict_t & dicts)
  {
    std::string dictname, ru;
    in >> dictname >> ru;
    const dict_t & dict = dicts.at(dictname);

    list_t slangWords = std::accumulate(dict.begin(), dict.end(), list_t{}, AccumulateIfContains{ ru });

    out << ru << ":";
    TranslationPrinter printer{ out };
    printer.first = true;

    if (!slangWords.empty())
    {
      auto printerRef = std::ref(printer);
      std::accumulate(slangWords.begin(), slangWords.end(), printerRef, CallPrinter{});
    }
    out << "\n";
  }

  void deleteSln(std::istream & in, dict_dict_t & dicts)
  {
    std::string dictname, sln;
    in >> dictname >> sln;
    dict_t & dict = dicts.at(dictname);
    if (dict.erase(sln) == 0)
    {
      throw std::invalid_argument("INVALID COMMAND");
    }
  }

  void addSln(std::istream & in, dict_dict_t & dicts)
  {
    std::string dictname, sln;
    size_t n;
    in >> dictname >> sln >> n;

    dict_t & dict = dicts.at(dictname);
    if (dict.find(sln) != dict.end())
    {
      throw std::invalid_argument("INVALID COMMAND");
    }

    list_t translations;
    translations.reserve(n);
    std::generate_n(std::back_inserter(translations), n, WordReader{ in });
    dict[sln] = translations;
  }

  void addRu(std::istream & in, dict_dict_t & dicts)
  {
    std::string dictname, sln;
    size_t n;
    in >> dictname >> sln >> n;

    dict_t & dict = dicts.at(dictname);
    list_t & translations = dict.at(sln);

    std::generate_n(std::back_inserter(translations), n, WordReader{ in });
  }

  void deleteRu(std::istream & in, dict_dict_t & dicts)
  {
    std::string dictname, sln;
    size_t n;
    in >> dictname >> sln >> n;

    dict_t & dict = dicts.at(dictname);
    list_t & translations = dict.at(sln);

    std::set<std::string> rusSet;
    std::generate_n(std::inserter(rusSet, rusSet.begin()), n, WordReader{ in });

    auto new_end = std::remove_if(translations.begin(), translations.end(), RusRemover{ rusSet });
    translations.erase(new_end, translations.end());
  }

  void writeDicts(std::istream & in, const dict_dict_t & dicts)
  {
    std::string filename;
    size_t n;
    in >> filename >> n;

    std::ofstream file(filename, std::ios::app);
    if (!file) throw std::runtime_error("INVALID FILE");

    std::vector<std::string> dictNames(n);
    std::generate_n(dictNames.begin(), n, WordReader{ in });

    std::accumulate(dictNames.begin(), dictNames.end(), Empty{}, FullDictWriterWrapper{ file, dicts });
  }

  void makeUnion(std::istream & in, dict_dict_t & dicts)
  {
    std::string newDictname;
    size_t n;
    in >> newDictname >> n;

    std::vector<std::string> dictNames(n);
    std::generate_n(dictNames.begin(), n, WordReader{ in });

    dict_t unionDict = std::accumulate(dictNames.begin(), dictNames.end(), dict_t{}, DictMergerAccumulator{ dicts });

    dicts[newDictname] = unionDict;
  }

  void printMostCommon(std::istream & in, std::ostream & out, const dict_dict_t & dicts)
  {
    int n, k;
    in >> n >> k;
    if (n <= 0 || k <= 0) throw std::invalid_argument("INVALID COMMAND");

    std::vector<std::string> dictNames;
    dictNames.reserve(k);
    std::generate_n(std::back_inserter(dictNames), k, WordReader{ in });

    std::map<std::string, int> frequencies;

    std::accumulate(dictNames.begin(), dictNames.end(), Empty{}, DictProcessor{ dicts, frequencies });

    std::vector<std::pair<std::string, int>> sorted;
    sorted.reserve(frequencies.size());
    std::transform(frequencies.begin(), frequencies.end(), std::back_inserter(sorted), TransformPair{});

    std::sort(sorted.begin(), sorted.end(), FreqComparator{});

    int num = std::min(n, static_cast<int>(sorted.size()));
    std::transform(sorted.begin(), sorted.begin() + num, std::ostream_iterator< std::string >(out, "\n"), PairToString{});
  }

  void rewriteFile(std::istream & in, const dict_dict_t & dicts)
  {
    std::string filename;
    size_t n;
    in >> filename >> n;

    std::ofstream file(filename);
    if (!file) throw std::runtime_error("INVALID FILE");

    std::vector<std::string> dictNames;
    dictNames.reserve(n);
    std::generate_n(std::back_inserter(dictNames), n, WordReader{ in });

    std::accumulate(dictNames.begin(), dictNames.end(), Empty{}, FullDictWriterWrapper{ file, dicts });
  }

  void rare(std::istream & in, dict_dict_t & dicts)
  {
    int K;
    in >> K;
    std::string newDictName;
    in >> newDictName;
    size_t n;
    in >> n;

    if (K < 0 || n <= 0 || K >= static_cast<int>(n))
    {
      throw std::invalid_argument("INVALID COMMAND");
    }

    std::vector<std::string> dictNames;
    std::generate_n(std::back_inserter(dictNames), n, DictNameReader{ in });

    bool allExist = std::all_of(dictNames.begin(), dictNames.end(), DictExists{ dicts });
    if (!allExist)
    {
      throw std::invalid_argument("INVALID COMMAND");
    }

    TempMap temp = std::accumulate(dictNames.begin(), dictNames.end(), TempMap{}, DictAccumulator{ dicts });

    dict_t newDict = std::accumulate(temp.begin(), temp.end(), dict_t{}, BuildNewDict{ K });

    if (dicts.find(newDictName) != dicts.end())
    {
      throw std::invalid_argument("INVALID COMMAND");
    }
    dicts[newDictName] = newDict;
  }
}
