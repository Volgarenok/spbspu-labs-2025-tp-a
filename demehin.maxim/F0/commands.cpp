#include "commands.hpp"
#include <functional>
#include <algorithm>
#include <vector>
#include <fstream>

namespace
{
  using namespace demehin;

  struct TranslationPrinter
  {
    std::ostream& out;
    bool isFirst = true;

    void operator()(const std::string& tr)
    {
      if (!isFirst)
      {
        out << ", ";
      }
      out << tr;
      isFirst = false;
    }
  };

  struct DictPrinter
  {
    std::ostream& out;

    void operator()(const std::pair< std::string, list_t >& unit)
    {
      out << unit.first << ": ";
      std::for_each(unit.second.begin(), unit.second.end(), TranslationPrinter{ out });
      out << "\n";
    }
  };

  struct RuFinder
  {
    const std::string& target;
    list_t& res;

    void operator()(const std::pair< std::string, list_t >& unit)
    {
      if (std::find(unit.second.begin(), unit.second.end(), target) != unit.second.end())
      {
        res.push_back(unit.first);
      }
    }
  };

  struct NameReader
  {
    std::istream& in;
    std::string operator()()
    {
      std::string str;
      in >> str;
      return str;
    }
  };

  struct TranslationsModifier
  {
    list_t& translations;
    bool isAddOperation;

    void operator()(const std::string& word)
    {
      if (isAddOperation)
      {
        translations.push_back(word);
      }
      else
      {
        translations.remove(word);
      }
    }
  };

  void modifyTranslations(std::istream& in, dict_t& dicts, bool isAddOp)
  {
    std::string dict_name, eng_word;
    size_t translations_cnt;
    in >> dict_name >> eng_word >> translations_cnt;

    auto& dict = dicts.at(dict_name);
    auto& translations = dict.at(eng_word);
    list_t words;
    NameReader reader{ in };
    TranslationsModifier mod{ translations, isAddOp };

    std::generate_n(std::back_inserter(words), translations_cnt, reader);
    std::for_each(words.begin(), words.end(), mod);
  }

  struct WordWriter
  {
    std::ostream& out;

    void operator()(const std::string& word) const
    {
      out << " " << word;
    }
  };

  struct DictUnitWriter
  {
    std::ostream& out;

    void operator()(const std::pair< std::string, list_t >& unit) const
    {
      out << unit.first;
      std::for_each(unit.second.begin(), unit.second.end(), WordWriter{ out });
      out << "\n";
    }
  };

  struct DictWriter
  {
    std::ostream& out;
    const std::string& dict_name;

    void operator()(const tree_t& dict) const
    {
      out << dict_name << "\n";
      std::for_each(dict.begin(), dict.end(), DictUnitWriter{ out });
      out << "\n";
    }
  };

  struct DictFilterWriter
  {
    const dict_t& dicts;
    std::ofstream& file;

    void operator()(const std::string& name) const
    {
      auto it = dicts.find(name);
      if (it != dicts.end())
      {
        DictWriter{ file, name }(it->second);
      }
    }
  };

  void processWriting(std::istream& in, const dict_t& dicts, std::ios_base::openmode mode)
  {
    std::string filename;
    size_t dicts_cnt;
    in >> filename >> dicts_cnt;

    std::ofstream file(filename, mode);
    if (!file)
    {
      throw std::logic_error("invalid file");
    }

    std::vector< std::string > dict_names(dicts_cnt);
    std::generate(dict_names.begin(), dict_names.end(), NameReader{ in });
    std::for_each(dict_names.begin(), dict_names.end(), DictFilterWriter{ dicts, file });
  }

  template< typename Operation >
  struct OperationApplier
  {
    const dict_t& dicts;
    tree_t& res;
    Operation op;

    void operator()(const std::string& name)
    {
      op(res, dicts.at(name));
    }
  };

  template< typename Operation >
  void processOperation(std::istream& in, dict_t& dicts, Operation op)
  {
    std::string new_name;
    size_t cnt;
    in >> new_name >> cnt;

    list_t dictNames;
    NameReader reader{ in };
    std::generate_n(std::back_inserter(dictNames), cnt, reader);

    auto it = dictNames.begin();
    tree_t res = dicts.at(*it);

    OperationApplier< Operation > applier{ dicts, res, op };
    std::for_each(++it, dictNames.end(), applier);

    dicts[new_name] = res;
  }

  struct UnionOperation
  {
    void operator()(tree_t& res, const tree_t& dict) const
    {
      res.insert(dict.begin(), dict.end());
    }
  };

  struct IntersectImpl
  {
    const tree_t& dict;
    tree_t& res;

    void operator()(const std::pair< std::string, list_t >& unit)
    {
      if (dict.count(unit.first) != 0)
      {
        res.insert(unit);
      }
    }
  };

  struct IntersectOperation
  {
    void operator()(tree_t& res, const tree_t& dict) const
    {
      tree_t new_res;
      IntersectImpl intersect{ dict, new_res };
      std::for_each(res.begin(), res.end(), intersect);
      res = std::move(new_res);
    }
  };

  struct ComplementImpl
  {
    const tree_t& dict;
    tree_t& res;

    void operator()(const std::pair< std::string, list_t >& unit)
    {
      if (dict.count(unit.first) == 0)
      {
        res.insert(unit);
      }
    }
  };

  struct ComplementOperation
  {
    void operator()(tree_t& res, const tree_t& dict) const
    {
      tree_t new_res;
      ComplementImpl complement{ dict, new_res };
      std::for_each(res.begin(), res.end(), complement);
      res = std::move(new_res);
    }
  };

  struct WordCollector
  {
    std::map< std::string, int >& freq;

    void operator()(const std::pair< std::string, list_t >& unit)
    {
      freq[unit.first]++;
    }
  };

  struct DictFreqProcessor
  {
    const dict_t& dicts;
    std::map< std::string, int >& freq;

    void operator()(const std::string& name)
    {
      const auto& dict = dicts.at(name);
      std::for_each(dict.begin(), dict.end(), WordCollector{ freq });
    }
  };

  struct FreqComparator
  {
    bool operator()(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
    {
      return a.second > b.second || (a.second == b.second && a.first < b.first);
    }
  };

  struct ResPrinter
  {
    std::ostream& out;
    int limit;
    int cnt = 0;

    void operator()(const std::pair< std::string, int >& unit)
    {
      if (cnt++ < limit)
      {
        out << unit.first << " " << unit.second << "\n";
      }
    }
  };

}

void demehin::printHelp(std::ostream& out)
{
  out << "Commands system:\n";
  out << "1. createdict < dictname > - create empty dictionary\n";
  out << "2. deletedict < dictname > - delete excisting dictionary\n";
  out << "3. printdict < dictname > - print dict content\n";
  out << "4. gettranslationeng < dictname > < eng > - print translations of word eng\n";
  out << "5. gettranslationru < dictname > < ru > - print translations of word ru\n";
  out << "6. deleteeng < dictname > < eng > - delete word eng from dictionary\n";
  out << "7. addeng < dictname > < eng > < N > < ru-1 > ... < ru-n > - add word eng with its N translations\n";
  out << "8. addru < dictname > < eng > < N > < ru-1 > ... < ru-n > - add N translations to eng\n";
  out << "9. deleteru < dictname > < eng > < N > < ru-1 > ... < ru-n > - delete N translations of eng\n";
  out << "10. union < newdict > < N > < dictname-1 > ... < dictname-n > - union of N dictionaries\n";
  out << "11. complement < newdict > < N > < dictname-1 > ... < dictname-n > - complemention of N dictionaries\n";
  out << "12. intersect < newdict > < N > < dictname-1 > ... < dictname-n > - intersection of N dictionaries\n";
  out << "13. writedicts < filename > < N > < dictname-1 > ... < dictname-n > - save N dictionaries to file\n";
  out << "14. rewritefile < filename > < N > < dictname-1 > ... < dictname-n > - rewrite file with N dictionaries\n";
  out << "15. mostcommon < N > < K > < dictname-1 > ... < dictname-n > - print N most common words in K dictionaries\n";
}

void demehin::rewriteFile(std::istream& in, const dict_t& dicts)
{
  processWriting(in, dicts, std::ios::out);
}

void demehin::writeToFile(std::istream& in, const dict_t& dicts)
{
  processWriting(in, dicts, std::ios::app);
}

void demehin::createDict(std::istream& in, dict_t& dicts)
{
  std::string name;
  in >> name;
  tree_t dict;
  if (dicts.find(name) == dicts.end())
  {
    dicts.insert(std::make_pair(name, dict));
  }
  else
  {
    throw std::logic_error("existent dictionary");
  }
}

void demehin::deleteDict(std::istream& in, dict_t& dicts)
{
  std::string dict_name;
  in >> dict_name;
  if (dicts.find(dict_name) == dicts.end())
  {
    throw std::logic_error("non-existent dictionary");
  }

  dicts.erase(dict_name);
}

void demehin::printDict(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  std::string name;
  in >> name;
  auto dict = dicts.at(name);
  std::for_each(dict.begin(), dict.end(), DictPrinter{ out });
}

void demehin::getTranslationEng(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  std::string dict_name, eng_word;
  in >> dict_name >> eng_word;
  auto translations = dicts.at(dict_name).at(eng_word);
  out << eng_word << ": ";
  std::for_each(translations.begin(), translations.end(), TranslationPrinter{ out });
  out << "\n";
}

void demehin::getTranslationRu(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  std::string dict_name, ru_word;
  in >> dict_name >> ru_word;
  list_t res;
  auto dict = dicts.at(dict_name);
  std::for_each(dict.begin(), dict.end(), RuFinder{ ru_word, res });

  if (res.empty())
  {
    throw std::logic_error("no such russian word");
  }

  out << ru_word << ": ";
  std::for_each(res.begin(), res.end(), TranslationPrinter{ out });
  out << "\n";
}

void demehin::deleteEng(std::istream& in, dict_t& dicts)
{
  std::string dict_name, word;
  in >> dict_name >> word;
  auto& dict = dicts.at(dict_name);
  if (dict.erase(word) == false)
  {
    throw std::logic_error("no such english word");
  }
}

void demehin::addEng(std::istream& in, dict_t& dicts)
{
  std::string dict_name, eng_word;
  size_t cnt;
  in >> dict_name >> eng_word >> cnt;

  auto& dict = dicts.at(dict_name);
  if (dict.count(eng_word) > 0)
  {
    throw std::logic_error("word already exists");
  }

  list_t translations;
  NameReader reader{ in };
  std::generate_n(std::back_inserter(translations), cnt, reader);
  dict.insert(std::make_pair(eng_word, translations));
}

void demehin::deleteRu(std::istream& in, dict_t& dicts)
{
  modifyTranslations(in, dicts, false);
}

void demehin::addRu(std::istream& in, dict_t& dicts)
{
  modifyTranslations(in, dicts, true);
}

void demehin::makeUnion(std::istream& in, dict_t& dicts)
{
  processOperation(in, dicts, UnionOperation{ });
}

void demehin::makeIntersect(std::istream& in, dict_t& dicts)
{
  processOperation(in, dicts, IntersectOperation{ });
}

void demehin::makeComplement(std::istream& in, dict_t& dicts)
{
  processOperation(in, dicts, ComplementOperation{ });
}

void demehin::printMostCommons(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  int n, k;
  if (!(in >> n >> k))
  {
    throw std::logic_error("input error");
  }

  if (n <= 0 || k <= 0)
  {
    throw std::logic_error("invalid argument");
  }

  std::vector< std::string > names(k);
  std::generate(names.begin(), names.end(), NameReader{ in });

  std::map< std::string, int > freq;
  std::for_each(names.begin(), names.end(), DictFreqProcessor{ dicts, freq });

  std::vector< std::pair< std::string, int > > sorted(freq.begin(), freq.end());
  std::sort(sorted.begin(), sorted.end(), FreqComparator{ });
  std::for_each(sorted.begin(), sorted.end(), ResPrinter{ out, n });
}
