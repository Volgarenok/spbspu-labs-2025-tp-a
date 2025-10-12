#include "commands.hpp"
#include <string>
#include <numeric>
#include <iterator>
#include <deque>

namespace
{
  using namespace khoroshilov;

  struct NameReader
  {
    std::istream& in_;
    std::string operator()() const
    {
      std::string word;
      in_ >> word;
      return word;
    }
  };

  struct TranslationPrinter
  {
    std::ostream& out_;
    mutable bool first_ = true;
    int operator()(const std::string& tr) const
    {
      if (!first_)
      {
        out_ << ", ";
      }
      out_ << tr;
      first_ = false;
      return 0;
    }
  };

  struct KeyExtractor
  {
    std::string operator()(const tree_t::value_type& pair) const
    {
      return pair.first;
    }
  };

  struct DictEntryPrinter
  {
    std::ostream& out_;
    int operator()(const tree_t::value_type& entry) const
    {
      out_ << entry.first << ": ";
      std::vector<int> dummy;
      std::transform(entry.second.begin(), entry.second.end(),std::back_inserter(dummy), TranslationPrinter{ out_ });
      out_ << "\n";
      return 0;
    }
  };

  struct WordPrinter
  {
    std::ostream& out_;
    void operator()(const std::string& word) const
    {
      out_ << " " << word;
    }
  };

  struct LengthComparator
  {
    bool operator()(const tree_t::value_type& a, const tree_t::value_type& b) const
    {
      return a.first.size() < b.first.size();
    }
  };

  struct PrefixChecker
  {
    std::string prefix_;
    bool operator()(const std::string& word) const
    {
      return word.compare(0, prefix_.size(), prefix_) == 0;
    }
  };

  struct RangeChecker
  {
    std::string start_;
    std::string end_;
    bool operator()(const std::string& word) const
    {
      return word >= start_ && word <= end_;
    }
  };

  struct TranslationCounter
  {
    size_t operator()(size_t count, const std::string&) const
    {
      return count + 1;
    }
  };

  struct DictEntryCopier
  {
    std::ostream& out_;
    int operator()(const tree_t::value_type& entry) const
    {
      out_ << entry.first << " ";
      std::copy(entry.second.begin(),  entry.second.end(),std::ostream_iterator<std::string>(out_, " "));
      out_ << "\n";
      return 0;
    }
  };

  struct DictSaver
  {
    std::ostream& out_;
    int operator()(const dict_t::value_type& dict_pair) const
    {
      out_ << dict_pair.first << "\n";
      std::vector<int> dummy;
      std::transform(dict_pair.second.begin(), dict_pair.second.end(),std::back_inserter(dummy), DictEntryCopier{ out_ });
      out_ << "\n";
      return 0;
    }
  };

  struct ComplementPredicate
  {
    const tree_t& dict_;
    bool operator()(const tree_t::value_type& entry) const
    {
      return dict_.find(entry.first) == dict_.end();
    }
  };

  struct ComplementAccumulator
  {
    const dict_t& dicts_;
    tree_t operator()(tree_t result, const std::string& dict_name) const
    {
      const tree_t& dict = dicts_.at(dict_name);
      tree_t temp;
      std::copy_if(result.begin(), result.end(), std::inserter(temp, temp.end()),ComplementPredicate{dict});
      return temp;
    }
  };

  struct IntersectPredicate
  {
    const tree_t& dict_;
    bool operator()(const tree_t::value_type& entry) const
    {
      return dict_.find(entry.first) != dict_.end();
    }
  };

  struct IntersectAccumulator
  {
    const dict_t& dicts_;
    tree_t operator()(tree_t result, const std::string& dict_name) const
    {
      const tree_t& dict = dicts_.at(dict_name);
      tree_t temp;
      std::copy_if(result.begin(), result.end(),std::inserter(temp, temp.end()), IntersectPredicate{dict});
      return temp;
    }
  };

  struct MaxLenFinder
  {
    size_t operator()(size_t max, const tree_t::value_type& entry) const
    {
      return std::max(max, entry.first.size());
    }
  };

  struct LongestPredicate
  {
    size_t max_len_;
    bool operator()(const tree_t::value_type& entry) const
    {
      return entry.first.size() == max_len_;
    }
  };

  struct LongestAccumulator
  {
    const dict_t& dicts_;
    tree_t operator()(tree_t result, const std::string& dict_name) const
    {
      const tree_t& dict = dicts_.at(dict_name);
      if (dict.empty())
      {
        return result;
      }
      size_t max_len = std::accumulate(dict.begin(), dict.end(), 0, MaxLenFinder{});
      tree_t temp_dict;
      std::copy_if(dict.begin(), dict.end(),std::inserter(temp_dict, temp_dict.end()),LongestPredicate{max_len});
      result.insert(temp_dict.begin(), temp_dict.end());
      return result;
    }
  };

  struct MaxCountFinder
  {
    size_t operator()(size_t max, const tree_t::value_type& entry) const
    {
      return std::max(max, entry.second.size());
    }
  };

  struct MeaningfulPredicate
  {
    size_t max_count_;
    bool operator()(const tree_t::value_type& entry) const
    {
      return entry.second.size() == max_count_;
    }
  };

  struct MeaningfulAccumulator
  {
    const dict_t& dicts_;
    tree_t operator()(tree_t result, const std::string& dict_name) const
    {
      const tree_t& dict = dicts_.at(dict_name);
      if (dict.empty())
      {
        return result;
      }
      size_t max_count = std::accumulate(dict.begin(), dict.end(), 0, MaxCountFinder{});
      tree_t temp_dict;
      std::copy_if(dict.begin(), dict.end(),std::inserter(temp_dict, temp_dict.end()), MeaningfulPredicate{max_count});
      result.insert(temp_dict.begin(), temp_dict.end());
      return result;
    }
  };
}

namespace khoroshilov
{
  void printHelp(std::ostream& out)
  {
    out << "Anglo-Russian Dictionary Commands:\n"
        << "1. help - Show help\n"
        << "2. load <file> - Load dictionaries\n"
        << "3. save <file> - Save dictionaries\n"
        << "4. dictcreate <name> - Create dictionary\n"
        << "5. dictrm <name> - Remove dictionary\n"
        << "6. lsdict <name> - List dictionary\n"
        << "7. engtranslate <dict> <word> - Translate word\n"
        << "8. addenglish <dict> <word> <N> [translations] - Add English word\n"
        << "9. addru <dict> <word> <N> [translations] - Add Russian translations\n"
        << "10. rmenglish <dict> <word> - Remove English word\n"
        << "11. rmru <dict> <word> <translation> - Remove Russian translation\n"
        << "12. maxlen <dict> - Find longest word\n"
        << "13. alfrange <dict> <start> <end> - Alphabetical range\n"
        << "14. engcount <dict> - Count English words\n"
        << "15. prefix <dict> <prefix> - Words with prefix\n"
        << "16. clear <dict> - Clear dictionary\n"
        << "17. complement <new> <N> <dict1> ... - Dictionary complement\n"
        << "18. intersect <new> <N> <dict1> ... - Dictionary intersection\n"
        << "19. longest <new> <N> <dict1> ... - Longest words\n"
        << "20. meancount <dict> <word> - Translation count\n"
        << "21. meaningful <new> <N> <dict1> ... - Most meaningful words\n";
  }

  void load(std::istream& in, dict_t& dicts)
  {
    std::string filename;
    in >> filename;
    std::ifstream file(filename);
    if (!file)
    {
      throw std::runtime_error("<FILE ERROR>");
    }
    std::string line;
    std::string current_dict;
    tree_t current_tree;

    while (std::getline(file, line))
    {
      if (line.empty())
      {
        if (!current_dict.empty())
        {
          dicts.insert(std::make_pair(current_dict, current_tree));
          current_dict.clear();
          current_tree.clear();
        }
        continue;
      }

      if (current_dict.empty())
      {
        current_dict = line;
      }
      else
      {
        size_t pos = line.find(' ');
        std::string eng_word = line.substr(0, pos);

        list_t translations;
        std::string rest = (pos != std::string::npos) ? line.substr(pos + 1) : "";

        size_t start = 0;
        size_t end = rest.find(' ');
        while (end != std::string::npos)
        {
          translations.push_back(rest.substr(start, end - start));
          start = end + 1;
          end = rest.find(' ', start);
        }
        if (start < rest.size())
        {
          translations.push_back(rest.substr(start));
        }

        current_tree.insert(std::make_pair(eng_word, translations));
      }
    }

    if (!current_dict.empty())
    {
      dicts.insert(std::make_pair(current_dict, current_tree));
    }
  }

  void save(std::istream& in, const dict_t& dicts)
  {
    std::string filename;
    in >> filename;
    std::ofstream file(filename);
    if (!file)
    {
      throw std::runtime_error("<FILE ERROR>");
    }
    std::vector<int> dummy;
    std::transform(dicts.begin(),dicts.end(), std::back_inserter(dummy), DictSaver{ file });
  }

  void dictcreate(std::istream& in, dict_t& dicts)
  {
    std::string name;
    in >> name;
    if (dicts.find(name) != dicts.end())
    {
      throw std::runtime_error("<ALREADY HAVE>");
    }
    dicts.insert(std::make_pair(name, tree_t()));
  }

  void dictrm(std::istream& in, dict_t& dicts)
  {
    std::string name;
    in >> name;
    if (dicts.erase(name) == 0)
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
  }

  void lsdict(std::istream& in, std::ostream& out, const dict_t& dicts)
  {
    std::string name;
    in >> name;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    std::vector<int> dummy;
    std::transform(it->second.begin(),it->second.end(), std::back_inserter(dummy), DictEntryPrinter{ out });
  }

  void engtranslate(std::istream& in, std::ostream& out, const dict_t& dicts)
  {
    std::string dict_name, eng_word;
    in >> dict_name >> eng_word;

    auto dict_it = dicts.find(dict_name);
    if (dict_it == dicts.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    auto word_it = dict_it->second.find(eng_word);
    if (word_it == dict_it->second.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    out << eng_word << ": ";
    std::vector<int> dummy;
    std::transform(word_it->second.begin(), word_it->second.end(), std::back_inserter(dummy), TranslationPrinter{ out });
    out << "\n";
  }

  void addenglish(std::istream& in, dict_t& dicts)
  {
    std::string dict_name, eng_word;
    size_t trans_count;
    in >> dict_name >> eng_word >> trans_count;

    auto& dict = dicts.at(dict_name);
    if (dict.find(eng_word) != dict.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    list_t translations;
    std::generate_n(std::back_inserter(translations), trans_count, NameReader{ in });
    dict.insert(std::make_pair(eng_word, translations));
  }

  void addru(std::istream& in, dict_t& dicts)
  {
    std::string dict_name, eng_word;
    size_t trans_count;
    in >> dict_name >> eng_word >> trans_count;

    auto& dict = dicts.at(dict_name);
    auto& translations = dict.at(eng_word);

    std::vector<std::string> new_trans(trans_count);
    std::generate(new_trans.begin(), new_trans.end(), NameReader{ in });
    std::copy(new_trans.begin(), new_trans.end(), std::back_inserter(translations));
  }

  void rmenglish(std::istream& in, dict_t& dicts)
  {
    std::string dict_name, eng_word;
    in >> dict_name >> eng_word;

    auto& dict = dicts.at(dict_name);
    if (dict.erase(eng_word) == 0)
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
  }

  void rmru(std::istream& in, dict_t& dicts)
  {
    std::string dict_name, eng_word, ru_word;
    in >> dict_name >> eng_word >> ru_word;

    auto& dict = dicts.at(dict_name);
    auto& translations = dict.at(eng_word);

    auto it = std::find(translations.begin(), translations.end(), ru_word);
    if (it == translations.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    translations.erase(it);
  }

  void maxlen(std::istream& in, std::ostream& out, const dict_t& dicts)
  {
    std::string dict_name;
    in >> dict_name;

    auto dict_it = dicts.find(dict_name);
    if (dict_it == dicts.end() || dict_it->second.empty())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    auto max_it = std::max_element(dict_it->second.begin(),dict_it->second.end(),LengthComparator{});
    out << dict_name << " " << max_it->first << " " << max_it->first.size() << "\n";
  }

  void alfrange(std::istream& in, std::ostream& out, const dict_t& dicts)
  {
    std::string dict_name, start, end;
    in >> dict_name >> start >> end;

    if (start > end)
    {
    std::swap(start, end);
    }
    auto dict_it = dicts.find(dict_name);
    if (dict_it == dicts.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    std::vector<std::string> words;
    std::transform(dict_it->second.begin(), dict_it->second.end(),std::back_inserter(words),KeyExtractor{});

    std::sort(words.begin(), words.end());
    auto low = std::lower_bound(words.begin(), words.end(), start);
    auto high = std::upper_bound(words.begin(), words.end(), end);

    std::copy(low, high, std::ostream_iterator<std::string>(out, "\n"));
  }

  void engcount(std::istream& in, std::ostream& out, const dict_t& dicts)
  {
    std::string dict_name;
    in >> dict_name;

    auto dict_it = dicts.find(dict_name);
    if (dict_it == dicts.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    out << dict_name << " " << dict_it->second.size() << "\n";
  }

  void prefix(std::istream& in, std::ostream& out, const dict_t& dicts)
  {
    std::string dict_name, prefix_str;
    in >> dict_name >> prefix_str;

    auto dict_it = dicts.find(dict_name);
    if (dict_it == dicts.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    std::vector<std::string> words;
    std::transform(dict_it->second.begin(), dict_it->second.end(),std::back_inserter(words),KeyExtractor{});

    std::vector<std::string> matched;
    std::copy_if(words.begin(), words.end(),std::back_inserter(matched),PrefixChecker{ prefix_str });

    if (matched.empty())
    {
      throw std::runtime_error("<NO WORDS FOUND>");
    }

    std::copy(matched.begin(),matched.end(),std::ostream_iterator<std::string>(out, "\n"));
  }

  void clear(std::istream& in, std::ostream& out, dict_t& dicts)
  {
    std::string dict_name;
    in >> dict_name;

    auto dict_it = dicts.find(dict_name);
    if (dict_it == dicts.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    if (dict_it->second.empty())
    {
      out << "<Empty Dictionary>\n";
    }
    else
    {
      dict_it->second.clear();
    }
  }

  void complement(std::istream& in, dict_t& dicts)
  {
    std::string new_name;
    size_t dict_count;
    in >> new_name >> dict_count;

    std::vector<std::string> dict_names(dict_count);
    std::generate(dict_names.begin(), dict_names.end(), NameReader{ in });

    if (dicts.find(new_name) != dicts.end())
    {
      throw std::runtime_error("<ALREADY HAVE>");
    }
    if (dict_names.empty())
    {
      dicts.insert({new_name, tree_t{}});
      return;
    }
    tree_t result = dicts.at(dict_names[0]);
    ComplementAccumulator op{dicts};
    result = std::accumulate(std::next(dict_names.begin()), dict_names.end(), result, op);

    dicts.insert(std::make_pair(new_name, result));
  }

  void intersect(std::istream& in, dict_t& dicts)
  {
    std::string new_name;
    size_t dict_count;
    in >> new_name >> dict_count;

    std::vector<std::string> dict_names(dict_count);
    std::generate(dict_names.begin(), dict_names.end(), NameReader{ in });

    if (dicts.find(new_name) != dicts.end())
    {
      throw std::runtime_error("<ALREADY HAVE>");
    }

    const tree_t& first_dict = dicts.at(dict_names[0]);
    tree_t result = first_dict;
    IntersectAccumulator op{dicts};
    result = std::accumulate(std::next(dict_names.begin()), dict_names.end(), result, op);
    dicts.insert(std::make_pair(new_name, result));
  }

  void longest(std::istream& in, dict_t& dicts)
  {
    std::string new_name;
    size_t dict_count;
    in >> new_name >> dict_count;

    std::vector<std::string> dict_names(dict_count);
    std::generate(dict_names.begin(), dict_names.end(), NameReader{ in });
    tree_t result;
    LongestAccumulator op{dicts};
    result = std::accumulate(dict_names.begin(),  dict_names.end(), tree_t{}, op);
    dicts.insert(std::make_pair(new_name, result));
  }

  void meancount(std::istream& in, std::ostream& out, const dict_t& dicts)
  {
    std::string dict_name, eng_word;
    in >> dict_name >> eng_word;

    auto dict_it = dicts.find(dict_name);
    if (dict_it == dicts.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    auto word_it = dict_it->second.find(eng_word);
    if (word_it == dict_it->second.end())
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    size_t count = std::accumulate(word_it->second.begin(), word_it->second.end(),0,TranslationCounter{});

    out << dict_name << " " << eng_word << " " << count << "\n";
  }

  void meaningful(std::istream& in, dict_t& dicts)
  {
    std::string new_name;
    size_t dict_count;
    in >> new_name >> dict_count;

    std::vector<std::string> dict_names(dict_count);
    std::generate(dict_names.begin(), dict_names.end(), NameReader{ in });

    if (dicts.find(new_name) != dicts.end())
    {
      throw std::runtime_error("<ALREADY HAVE>");
    }

    tree_t result;
    MeaningfulAccumulator op{dicts};
    result = std::accumulate(dict_names.begin(), dict_names.end(), tree_t{}, op);

    dicts.insert(std::make_pair(new_name, result));
  }
}

