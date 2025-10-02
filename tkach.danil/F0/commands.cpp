#include "commands.hpp"
#include <string>
#include <fstream>
#include <algorithm>
#include <set>
#include <vector>
#include <iterator>
#include <numeric>

namespace
{
  using tree_of_words = std::map< std::string, std::list< std::string > >;
  using tree_of_dict = std::map< std::string, tree_of_words >;

  class InsertIntoSet
  {
  public:
    InsertIntoSet(std::set< std::string >& s):
      set_(s)
    {}
    void operator()(const std::string& val) const
    {
      set_.insert(val);
    }
  private:
    std::set< std::string >& set_;
  };

  std::list< std::string > mergeTranslations(const std::list< std::string >& list1, const std::list< std::string >& list2)
  {
    std::set< std::string > tree_translations;
    std::for_each(list1.cbegin(), list1.cend(), InsertIntoSet{tree_translations});
    std::for_each(list2.cbegin(), list2.cend(), InsertIntoSet{tree_translations});
    std::list< std::string > translations;
    std::copy(tree_translations.cbegin(), tree_translations.cend(), std::back_inserter(translations));
    return translations;
  }

  class MergeWordsInDict
  {
  public:
    MergeWordsInDict(tree_of_words& temp):
      temp_dict_(&temp)
    {}
    void operator()(const std::pair< const std::string, std::list< std::string > >& pair) const
    {
      (*temp_dict_)[pair.first] = mergeTranslations((*temp_dict_)[pair.first], pair.second);
    }
  private:
    tree_of_words* temp_dict_;
  };

  class AccumulateDicts
  {
  public:
    tree_of_words operator()(tree_of_words acc_dict, const tree_of_words* next_dict) const
    {
      std::for_each(next_dict->cbegin(), next_dict->cend(), MergeWordsInDict{acc_dict});
      return acc_dict;
    }
  };

  class PushBackToList
  {
  public:
    PushBackToList(std::list< std::string >& list):
      list_(list)
    {}
    void operator()(const std::string& val) const
    {
      list_.push_back(val);
    }
  private:
    std::list< std::string >& list_;
  };

  bool findTranslation(const std::list< std::string >& list, const std::string& translation)
  {
    return std::find(list.cbegin(), list.cend(), translation) != list.cend();
  }

  class FindCommonElements
  {
  public:
    FindCommonElements(const std::list< std::string >& list2, std::list< std::string >& common_list):
      list2_(list2),
      common_list_(common_list)
    {}
    void operator()(const std::string& tr) const
    {
      if (findTranslation(list2_, tr) && !findTranslation(common_list_, tr))
      {
        common_list_.push_back(tr);
      }
    }
  private:
    const std::list< std::string >& list2_;
    std::list< std::string >& common_list_;
  };

  std::list< std::string > getCommonElements(const std::list< std::string >& list1, const std::list< std::string >& list2)
  {
    std::list< std::string > common_list;
    std::for_each(list1.cbegin(), list1.cend(), FindCommonElements{list2, common_list});
    return common_list;
  }

  class PrintStringWithLeadingSpace
  {
  public:
    PrintStringWithLeadingSpace(std::ostream& os):
      out_(os)
    {}
    void operator()(const std::string& s) const
    {
      out_ << " " << s;
    }
  private:
    std::ostream& out_;
  };

  class WriteWordEntry
  {
  public:
    WriteWordEntry(std::ostream& os):
      out(os)
    {}
    void operator()(const std::pair< const std::string, std::list< std::string > >& pair) const
    {
      out << pair.first << " " << pair.second.size();
      if (!pair.second.empty())
      {
        std::for_each(pair.second.cbegin(), pair.second.cend(), PrintStringWithLeadingSpace{out});
      }
      out << "\n";
    }
  private:
    std::ostream& out;
  };

  class WriteWordEntryWithoutSize
  {
  public:
    WriteWordEntryWithoutSize(std::ostream& os):
      out(os)
    {}
    void operator()(const std::pair< const std::string, std::list< std::string > >& pair) const
    {
      out << pair.first;
      if (!pair.second.empty())
      {
        std::for_each(pair.second.cbegin(), pair.second.cend(), PrintStringWithLeadingSpace{out});
      }
      out << "\n";
    }
  private:
    std::ostream& out;
  };

  void writeDictToFile(std::ostream& out, const std::string& dict_name, const tree_of_words& dict)
  {
    out << dict_name << " " << dict.size() << "\n";
    std::for_each(dict.cbegin(), dict.cend(), WriteWordEntry{out});
  }

  std::map< std::string, std::list< std::string > > mergeDicts(const std::list< const tree_of_words* >& source_dicts)
  {
    if (source_dicts.empty())
    {
      return tree_of_words{};
    }
    return std::accumulate(source_dicts.cbegin(), source_dicts.cend(), tree_of_words{}, AccumulateDicts{});
  }

  class CheckDictExists
  {
  public:
    explicit CheckDictExists(const tree_of_dict& dict_tree):
      dict_tree_(&dict_tree)
    {}
    bool operator()(const std::string& dict_name) const
    {
      if (dict_name.empty())
      {
        throw std::logic_error("<INVALID ARGUMENTS>");
      }
      return dict_tree_->count(dict_name) > 0;
    }
  private:
    const tree_of_dict* dict_tree_;
  };

  class ExportDictToFile
  {
  public:
    ExportDictToFile(std::ostream& os):
      out_(os)
    {}
    void operator()(const std::pair< const std::string, const tree_of_words >& pair) const
    {
      writeDictToFile(out_, pair.first, pair.second);
      out_ << "\n";
    }
  private:
    std::ostream& out_;
  };

  class ExportSpecificDicts
  {
  public:
    ExportSpecificDicts(std::ostream& os, const tree_of_dict& dict_tree):
      out_(os),
      dict_tree_(dict_tree)
    {}
    void operator()(const std::string& dict_name) const
    {
      if (dict_name.empty())
      {
        throw std::logic_error("<INVALID ARGUMENTS>");
      }
      auto it = dict_tree_.find(dict_name);
      writeDictToFile(out_, it->first, it->second);
      out_ << "\n";
    }
  private:
    std::ostream& out_;
    const tree_of_dict& dict_tree_;
  };

  struct IsStringEmpty
  {
    bool operator()(const std::string& s) const
    {
      return s.empty();
    }
  };

  void exportDictionaries(std::istream& in, const tree_of_dict& avltree, std::ios_base::openmode mode)
  {
    std::string filename;
    int number_of_dictionaries = 0;
    if (!(in >> filename) || filename.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    if (!(in >> number_of_dictionaries) || (number_of_dictionaries < 0))
    {
      throw std::logic_error("<INVALID NUMBER>");
    }
    std::ofstream outFile(filename, mode);
    if (!outFile.is_open())
    {
      throw std::logic_error("<EXPORT FAILED>");
    }
    if (number_of_dictionaries == 0)
    {
      if (avltree.empty())
      {
        return;
      }
      std::for_each(avltree.cbegin(), avltree.cend(), ExportDictToFile{outFile});
    }
    else
    {
      std::vector< std::string > dict_names;
      std::copy_n(std::istream_iterator< std::string >{in}, number_of_dictionaries, std::back_inserter(dict_names));
      if (dict_names.size() != static_cast< size_t >(number_of_dictionaries))
      {
        throw std::logic_error("<INVALID ARGUMENTS>");
      }
      if (std::any_of(dict_names.cbegin(), dict_names.cend(), IsStringEmpty{}) || in.eof())
      {
        throw std::logic_error("<INVALID ARGUMENTS>");
      }
      if (!std::all_of(dict_names.cbegin(), dict_names.cend(), CheckDictExists{avltree}))
      {
        throw std::logic_error("<INVALID DICTIONARY>");
      }
      std::for_each(dict_names.cbegin(), dict_names.cend(), ExportSpecificDicts{outFile, avltree});
    }
  }

  class ImportDictProcessor
  {
  public:
    ImportDictProcessor(tree_of_dict& target_avltree, const tree_of_dict& source_dict):
      target_avltree_(target_avltree),
      source_dict_(source_dict)
    {}
    void operator()(const std::string& dict_name) const
    {
      auto it = source_dict_.find(dict_name);
      if (it == source_dict_.end())
      {
        throw std::logic_error("<INVALID IMPORT>");
      }
      target_avltree_[dict_name] = mergeDicts({&source_dict_.at(dict_name), &target_avltree_[dict_name]});
    }
  private:
    tree_of_dict& target_avltree_;
    const tree_of_dict& source_dict_;
  };

  class PrintDictEntry
  {
  public:
    PrintDictEntry(std::ostream& os):
      out_(os)
    {}
    void operator()(const std::pair< const std::string, const tree_of_words >& dict_pair) const
    {
      out_ << dict_pair.first << "\n";
      if (!dict_pair.second.empty())
      {
        std::for_each(dict_pair.second.cbegin(), dict_pair.second.cend(), WriteWordEntryWithoutSize{out_});
      }
    }
  private:
    std::ostream& out_;
  };

  class AddTranslationToWord
  {
  public:
    AddTranslationToWord(const std::string& eng_word, const std::string& translation):
      eng_word_(eng_word),
      translation_(translation)
    {}
    void operator()(std::pair< const std::string, tree_of_words >& dict_pair) const
    {
      auto it2 = dict_pair.second.find(eng_word_);
      if (it2 != dict_pair.second.end())
      {
        it2->second = mergeTranslations(it2->second, {translation_});
      }
    }
  private:
    const std::string& eng_word_;
    const std::string& translation_;
  };

  class RemoveTranslationFromWord
  {
  public:
    RemoveTranslationFromWord(const std::string& eng_word, const std::string& translation):
      eng_word_(eng_word),
      translation_(translation)
    {}
    void operator()(std::pair< const std::string, tree_of_words >& dict_pair) const
    {
      auto it2 = dict_pair.second.find(eng_word_);
      if (it2 != dict_pair.second.end())
      {
        auto& translations_list = it2->second;
        translations_list.erase(std::remove(translations_list.begin(), translations_list.end(), translation_), translations_list.end());
      }
    }
  private:
    const std::string& eng_word_;
    const std::string& translation_;
  };

  class EraseWordFromTargetDict
  {
  public:
    EraseWordFromTargetDict(tree_of_words& target_dict):
      target_dict_(&target_dict)
    {}
    void operator()(const std::pair< const std::string, const std::list< std::string > >& pair) const
    {
      target_dict_->erase(pair.first);
    }
  private:
    tree_of_words* target_dict_;
  };

  class SubstractWordsFromDict
  {
  public:
    SubstractWordsFromDict(tree_of_words& target_dict):
      target_dict_(&target_dict)
    {}
    void operator()(const tree_of_words* dict_to_subtract) const
    {
      std::for_each(dict_to_subtract->cbegin(), dict_to_subtract->cend(), EraseWordFromTargetDict{*target_dict_});
    }
  private:
    tree_of_words* target_dict_;
  };

  class FindDictAndGetPtr
  {
  public:
    FindDictAndGetPtr(tree_of_dict& avltree):
      avltree_(avltree)
    {}
    tree_of_words* operator()(const std::string& dict_name) const
    {
      auto it = avltree_.find(dict_name);
      if (it == avltree_.end())
      {
        throw std::logic_error("<INVALID DICTIONARY>");
      }
      return &it->second;
    }
  private:
    tree_of_dict& avltree_;
  };

  class CollectTranslations
  {
  public:
    using pair_t = std::pair< std::set< std::string >, bool >;
    CollectTranslations(const tree_of_dict& avltree, const std::string& eng_word):
        avltree_(avltree),
        eng_word_(eng_word)
    {}
    pair_t operator()(pair_t curr_acc_pair, const std::string& dict_name) const
    {
      auto it = avltree_.find(dict_name);
      if (it == avltree_.cend())
      {
        throw std::logic_error("<INVALID DICTIONARY>");
      }
      auto it2 = it->second.find(eng_word_);
      if (it2 != it->second.cend())
      {
        std::copy(it2->second.cbegin(), it2->second.cend(), std::inserter(curr_acc_pair.first, curr_acc_pair.first.end()));
        return curr_acc_pair;
      }
      else
      {
        curr_acc_pair.second = false;
      }
      return curr_acc_pair;
    }
  private:
    const tree_of_dict& avltree_;
    const std::string& eng_word_;
  };

  class FindCommonPart
  {
  public:
    tree_of_words operator()(const tree_of_words& dict1, const tree_of_words* dict2) const
    {
      tree_of_words common_dict;
      std::for_each(dict1.cbegin(), dict1.cend(), FillCommonDict{dict2, common_dict});
      return common_dict;
    }
  private:
    class FillCommonDict
    {
    public:
      FillCommonDict(const tree_of_words* other_dict, tree_of_words& common):
        other_dict_(other_dict),
        common_dict_(&common)
      {}
      void operator()(const std::pair< const std::string, const std::list< std::string > >& entry) const
      {
        auto it_find = other_dict_->find(entry.first);
        if (it_find != other_dict_->cend())
        {
          (*common_dict_)[entry.first] = mergeTranslations(entry.second, it_find->second);
        }
      }
    private:
        const tree_of_words* other_dict_;
        tree_of_words* common_dict_;
    };
  };

  class ProcessWordForCommonTranslations
  {
  public:
    ProcessWordForCommonTranslations(const tree_of_words& dict, std::list< std::string >& common_list):
      dict_(dict),
      common_list_(common_list)
    {}
    void operator()(const std::string& word) const
    {
      auto word_it = dict_.find(word);
      if (word_it == dict_.cend())
      {
        throw std::logic_error("<INVALID WORD>");
      }
      if (common_list_.empty())
      {
        common_list_ = word_it->second;
      }
      else
      {
        common_list_ = getCommonElements(common_list_, word_it->second);
      }
    }
  private:
    const tree_of_words& dict_;
    std::list< std::string >& common_list_;
  };

  class CopyTranslationToTargetDict
  {
  public:
    CopyTranslationToTargetDict(tree_of_dict& avltree, const std::string& eng_word, const std::list< std::string >& translations):
      avltree_(avltree),
      eng_word_(eng_word),
      translations_(translations)
    {}
    void operator()(const std::string& curr_dict_name) const
    {
      auto it = avltree_.find(curr_dict_name);
      if (it == avltree_.end())
      {
        throw std::logic_error("<INVALID DICTIONARY>");
      }
      tree_of_words& target = it->second;
      target[eng_word_] = mergeTranslations(target[eng_word_], translations_);
    }
  private:
    tree_of_dict& avltree_;
    const std::string& eng_word_;
    const std::list< std::string >& translations_;
  };

  class CheckWordForTranslation
  {
  public:
    CheckWordForTranslation(const std::string& translation, std::set< std::string >& words_set):
      translation_(translation),
      words_set_(words_set)
    {}
    void operator()(const std::pair< const std::string, const std::list< std::string > >& word_entry) const
    {
      if (findTranslation(word_entry.second, translation_))
      {
        words_set_.insert(word_entry.first);
      }
    }
  private:
    const std::string& translation_;
    std::set< std::string >& words_set_;
  };

  class FindEnglishWordsWithTranslation
  {
  public:
    FindEnglishWordsWithTranslation(const tree_of_dict& avltree, const std::string& translation, std::set< std::string >& tree_word):
      avltree_(avltree),
      translation_(translation),
      tree_word_(tree_word)
    {}
    void operator()(const std::string& curr_dict_name) const
    {
      auto it = avltree_.find(curr_dict_name);
      if (it == avltree_.cend())
      {
        throw std::logic_error("<INVALID DICTIONARY>");
      }
      std::for_each(it->second.cbegin(), it->second.cend(), CheckWordForTranslation{translation_, tree_word_});
    }
  private:
    const tree_of_dict& avltree_;
    const std::string& translation_;
    std::set< std::string >& tree_word_;
  };

  struct WordEntry
  {
    std::string eng_word;
    std::list< std::string > translations;
  };

  std::istream& operator>>(std::istream& in, WordEntry& entry)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    int count = 0;
    if (!(in >> entry.eng_word))
    {
      return in;
    }
    if (!(in >> count) || count < 0)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    entry.translations.clear();
    std::copy_n(std::istream_iterator< std::string >{in}, count, std::back_inserter(entry.translations));
    return in;
  }

  struct FileDictionaryBlock
  {
    std::string name;
    tree_of_words dictionary;
  };

  class InsertAndMergeWordEntry
  {
  public:
    InsertAndMergeWordEntry(tree_of_words& dict):
      target_map_(&dict)
    {}
    void operator()(const WordEntry& entry) const
    {
      (*target_map_)[entry.eng_word] = mergeTranslations((*target_map_)[entry.eng_word], entry.translations);
    }
  private:
    tree_of_words* target_map_;
  };

  std::istream& operator>>(std::istream& in, FileDictionaryBlock& block)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    int count_words = 0;
    if (!(in >> block.name))
    {
      return in;
    }
    if (!(in >> count_words) || count_words < 0)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    block.dictionary.clear();
    std::vector< WordEntry > temp_word_entries;
    std::copy_n(std::istream_iterator< WordEntry >{in}, count_words, std::back_inserter(temp_word_entries));
    if (temp_word_entries.size() != static_cast< size_t >(count_words))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::for_each(temp_word_entries.begin(), temp_word_entries.end(), InsertAndMergeWordEntry{block.dictionary});
    return in;
  }

  class CollectAndMergeFileDictionaries
  {
  public:
    explicit CollectAndMergeFileDictionaries(tree_of_dict& dicts):
      target_dicts_(&dicts)
    {}
    void operator()(const FileDictionaryBlock& block) const
    {
      (*target_dicts_)[block.name] = mergeDicts({&block.dictionary, &(*target_dicts_)[block.name]});
    }
  private:
    tree_of_dict* target_dicts_;
  };

  class ApplyAllDicts
  {
  public:
    explicit ApplyAllDicts(tree_of_dict& dicts):
      target_dicts_(&dicts)
    {}
    void operator()(const std::pair< const std::string, const tree_of_words >& pair) const
    {
      (*target_dicts_)[pair.first] = mergeDicts({&pair.second, &(*target_dicts_)[pair.first]});
    }
  private:
    tree_of_dict* target_dicts_;
  };
}

void tkach::import(std::istream& in, tree_of_dict& avltree)
{
  std::string file_name = "";
  int count_of_dict = 0;
  if (!(in >> file_name))
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  std::fstream in2(file_name);
  if (!in2.is_open())
  {
    throw std::logic_error("<INVALID IMPORT>");
  }
  if (!(in >> count_of_dict) || count_of_dict < 0)
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  using istreamIt = std::istream_iterator< FileDictionaryBlock >;
  tree_of_dict imported_dictionaries;
  std::for_each(istreamIt{in2}, istreamIt{}, CollectAndMergeFileDictionaries(imported_dictionaries));
  if (!in2.eof() && in2.fail())
  {
    throw std::logic_error("<INVALID IMPORT>");
  }
  if (count_of_dict != 0)
  {
    std::vector< std::string > main_name_dict(count_of_dict);
    std::copy_n(std::istream_iterator< std::string >{in}, count_of_dict, main_name_dict.begin());
    if (std::any_of(main_name_dict.begin(), main_name_dict.end(), IsStringEmpty{}) || in.eof())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    if (!std::all_of(main_name_dict.cbegin(), main_name_dict.cend(), CheckDictExists{avltree}))
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    std::for_each(main_name_dict.cbegin(), main_name_dict.cend(), ImportDictProcessor{avltree, imported_dictionaries});
  }
  else
  {
    std::for_each(imported_dictionaries.cbegin(), imported_dictionaries.cend(), ApplyAllDicts{avltree});
  }
}

void tkach::addWord(std::istream& in, tree_of_dict& avltree)
{
  std::string dict_name;
  std::string eng_word;
  int num_translations = 0;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> num_translations) || (num_translations <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  std::list< std::string > translations;
  std::copy_n(std::istream_iterator< std::string >{in}, num_translations, std::back_inserter(translations));
  if (std::any_of(translations.begin(), translations.end(), IsStringEmpty{}) || in.eof())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  avltree[dict_name][eng_word] = mergeTranslations(translations, avltree[dict_name][eng_word]);
}

void tkach::mergeWords(std::istream& in, tree_of_dict& avltree)
{
  std::string dict_name;
  std::string eng_word1;
  std::string eng_word2;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it = avltree.find(dict_name);
  if (it == avltree.end())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  if (!(in >> eng_word1) || eng_word1.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it2 = it->second.find(eng_word1);
  if (it2 == it->second.end())
  {
    throw std::logic_error("<INVALID WORD>");
  }
  if (!(in >> eng_word2) || eng_word2.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it3 = it->second.find(eng_word2);
  if (it3 == it->second.end())
  {
    throw std::logic_error("<INVALID WORD>");
  }
  it2->second = mergeTranslations(it2->second, it3->second);
  it3->second = it2->second;
}

void tkach::printCommonTranslations(std::istream& in, std::ostream& out, const tree_of_dict& avltree)
{
  std::string dict_name;
  int number_of_words = 0;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it = avltree.find(dict_name);
  if (it == avltree.cend())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  if (!(in >> number_of_words) || number_of_words <= 0)
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  std::vector< std::string > words_to_process(number_of_words);
  std::copy_n(std::istream_iterator< std::string >{in}, number_of_words, words_to_process.begin());
  if (std::any_of(words_to_process.begin(), words_to_process.end(), IsStringEmpty{}) || in.eof())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  std::list< std::string > common_translations;
  std::for_each(words_to_process.cbegin(), words_to_process.cend(), ProcessWordForCommonTranslations{it->second, common_translations});
  if (common_translations.empty())
  {
    out << "\n";
  }
  else
  {
    out << *common_translations.cbegin();
    std::for_each(std::next(common_translations.cbegin()), common_translations.cend(), PrintStringWithLeadingSpace{out});
    out << "\n";
  }
}

void tkach::doExportOverwrite(std::istream& in, const tree_of_dict& avltree)
{
  exportDictionaries(in, avltree, std::ios_base::out);
}

void tkach::doExportInEnd(std::istream& in, const tree_of_dict& avltree)
{
  exportDictionaries(in, avltree, std::ios_base::app);
}

void tkach::printAll(std::ostream& out, const tree_of_dict& data)
{
  if (data.empty())
  {
    throw std::logic_error("<EMPTY>");
  }
  std::for_each(data.cbegin(), data.cend(), PrintDictEntry{out});
}

void tkach::removeWord(std::istream& in, tree_of_dict& avltree)
{
  std::string dict_name;
  std::string eng_word;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it = avltree.find(dict_name);
  if (it == avltree.end())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  bool removed = (it->second).erase(eng_word);
  if (!removed)
  {
    throw std::logic_error("<INVALID WORD>");
  }
}

void tkach::addTranslation(std::istream& in, tree_of_dict& avltree)
{
  std::string dict_name = "";
  std::string eng_word;
  std::string translation;
  size_t dict_name_specified = 2;
  if (!(in >> dict_name_specified) || (dict_name_specified != 0 && dict_name_specified != 1))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  auto it = avltree.end();
  if (dict_name_specified)
  {
    if (!(in >> dict_name) || dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    it = avltree.find(dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
  }
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> translation) || translation.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (dict_name_specified)
  {
    auto it2 = it->second.find(eng_word);
    if (it2 == it->second.end())
    {
      throw std::logic_error("<INVALID WORD>");
    }
    it2->second = mergeTranslations(it2->second, {translation});
  }
  else
  {
    std::for_each(avltree.begin(), avltree.end(), AddTranslationToWord{eng_word, translation});
  }
}

void tkach::removeTranslation(std::istream& in, tree_of_dict& avltree)
{
  std::string dict_name = "";
  std::string eng_word;
  std::string translation;
  size_t dict_name_specified = 2;
  if (!(in >> dict_name_specified) || (dict_name_specified != 0 && dict_name_specified != 1))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  auto it = avltree.end();
  if (dict_name_specified)
  {
    if (!(in >> dict_name) || dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    it = avltree.find(dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
  }
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> translation) || translation.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (dict_name_specified)
  {
    auto it2 = it->second.find(eng_word);
    if (it2 == it->second.end())
    {
      throw std::logic_error("<INVALID WORD OR TRANSLATION>");
    }
    if (findTranslation(it2->second, translation))
    {
      it2->second.erase(std::remove(it2->second.begin(), it2->second.end(), translation), it2->second.end());
    }
    else
    {
      throw std::logic_error("<INVALID WORD OR TRANSLATION>");
    }
  }
  else
  {
    std::for_each(avltree.begin(), avltree.end(), RemoveTranslationFromWord{eng_word, translation});
  }
}

void tkach::clear(std::istream& in, tree_of_dict& avltree)
{
  std::string dict_name;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it = avltree.find(dict_name);
  if (it == avltree.end())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  it->second.clear();
}

void tkach::printCount(std::istream& in, std::ostream& out, tree_of_dict& avltree)
{
  std::string dict_name;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it = avltree.find(dict_name);
  if (it == avltree.end())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  out << it->second.size() << "\n";
}

void tkach::substructDicts(std::istream& in, tree_of_dict& avltree)
{
  std::string new_dict_name;
  int number_of_dictionaries = 0;
  if (!(in >> new_dict_name) || new_dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  std::vector< std::string > dict_names;
  std::copy_n(std::istream_iterator< std::string >{in}, number_of_dictionaries, std::back_inserter(dict_names));
  if (dict_names.size() != static_cast< size_t >(number_of_dictionaries))
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (std::any_of(dict_names.cbegin(), dict_names.cend(), IsStringEmpty{}) || in.eof())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  std::vector< tree_of_words* > source_dicts;
  std::transform(dict_names.cbegin(), dict_names.cend(), std::back_inserter(source_dicts), FindDictAndGetPtr{avltree});
  tree_of_words result_dict(*source_dicts[0]);
  if (source_dicts.size() > 1)
  {
    std::for_each(source_dicts.begin() + 1, source_dicts.end(), SubstractWordsFromDict{result_dict});
  }
  avltree[new_dict_name] = result_dict;
}

void tkach::mergeNumberDicts(std::istream& in, tree_of_dict& avltree)
{
  std::string new_dict_name;
  int number_of_dictionaries = 0;
  if (!(in >> new_dict_name) || new_dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  std::vector< std::string > dict_names;
  std::copy_n(std::istream_iterator< std::string >{in}, number_of_dictionaries, std::back_inserter(dict_names));
  if (dict_names.size() != static_cast< size_t >(number_of_dictionaries))
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (std::any_of(dict_names.cbegin(), dict_names.cend(), IsStringEmpty{}) || in.eof())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  std::list< const tree_of_words* > source_dicts;
  std::transform(dict_names.cbegin(), dict_names.cend(), std::back_inserter(source_dicts), FindDictAndGetPtr{avltree});
  tree_of_words result_dict = mergeDicts(source_dicts);
  avltree[new_dict_name] = result_dict;
}

void tkach::doCommonPartDicts(std::istream& in, tree_of_dict& avltree)
{
  std::string new_dict_name;
  int number_of_dictionaries = 0;
  if (!(in >> new_dict_name) || new_dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  std::vector< std::string > dict_names;
  std::copy_n(std::istream_iterator< std::string >{in}, number_of_dictionaries, std::back_inserter(dict_names));
  if (dict_names.size() != static_cast< size_t >(number_of_dictionaries))
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (std::any_of(dict_names.cbegin(), dict_names.cend(), IsStringEmpty{}) || in.eof())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  std::vector< tree_of_words* > source_dicts;
  std::transform(dict_names.cbegin(), dict_names.cend(), std::back_inserter(source_dicts), FindDictAndGetPtr{avltree});
  tree_of_words result_dict(*source_dicts[0]);
  if (source_dicts.size() > 1)
  {
    result_dict = std::accumulate(std::next(source_dicts.cbegin()), source_dicts.cend(), result_dict, FindCommonPart{});
  }
  avltree[new_dict_name] = result_dict;
}

void tkach::copyTranslations(std::istream& in, tree_of_dict& avltree)
{
  std::string source_dict_name;
  std::string eng_word;
  int number_of_dictionaries = 0;
  if (!(in >> source_dict_name) || source_dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto source_it = avltree.find(source_dict_name);
  if (source_it == avltree.end())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  const tree_of_words& source_dict = source_it->second;
  auto word_it = source_dict.find(eng_word);
  if (word_it == source_dict.cend())
  {
    throw std::logic_error("<INVALID WORD>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  std::vector< std::string > target_dict_names;
  std::copy_n(std::istream_iterator< std::string >{in}, number_of_dictionaries, std::back_inserter(target_dict_names));
  if (target_dict_names.size() != static_cast< size_t >(number_of_dictionaries))
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (std::any_of(target_dict_names.cbegin(), target_dict_names.cend(), IsStringEmpty{}) || in.eof())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  std::for_each(target_dict_names.cbegin(), target_dict_names.cend(), CopyTranslationToTargetDict{avltree, eng_word, word_it->second});
}

void tkach::printTranslations(std::istream& in, std::ostream& out, const tree_of_dict& avltree)
{
  std::string eng_word;
  int number_of_dictionaries = 0;
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  std::vector< std::string > dict_names;
  std::copy_n(std::istream_iterator< std::string >{in}, number_of_dictionaries, std::back_inserter(dict_names));
  if (dict_names.size() != static_cast< size_t >(number_of_dictionaries))
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (std::any_of(dict_names.cbegin(), dict_names.cend(), IsStringEmpty{}) || in.eof())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  CollectTranslations collector{avltree, eng_word};
  auto it1 = dict_names.cbegin();
  auto it2 = dict_names.cend();
  auto pair_tree_translations = std::accumulate(it1, it2, std::make_pair(std::set< std::string>{}, true), collector);
  std::set< std::string > tree_translations = pair_tree_translations.first;
  if (!pair_tree_translations.second)
  {
    throw std::logic_error("<INVALID WORD>");
  }
  else if (tree_translations.empty())
  {
    out << "\n";
  }
  else
  {
    out << *tree_translations.cbegin();
    std::for_each(std::next(tree_translations.cbegin()), tree_translations.cend(), PrintStringWithLeadingSpace{out});
    out << "\n";
  }
}

void tkach::help(std::ostream& out)
{
  out << "import <file_name> <number_of_dictionaries> <dictionary_name1> ... - Импорт словарей из файла.\n";
  out << "addword <dictionary_name> <english_word> <number_of_translations> <translation1> ...";
  out << "- Добавление слова и его переводов в словарь.\n";
  out << "mergewords <dictionary_name> <english_word1> <english_word2>";
  out << "- Объединение переводов двух английских слов в одном словаре.\n";
  out << "printcommontranslations <dictionary_name> <number_of_words> <word1> ... ";
  out << "- Вывод общих переводов для нескольких слов в одном словаре.\n";
  out << "exportoverwrite <file_name> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Экспорт словарей в файл, перезаписывая его.\n";
  out << "exportinend <file_name> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Экспорт словарей в файл, добавляя в конец.\n";
  out << "printall - Вывод всех словарей и их содержимого.\n";
  out << "removeword <dictionary_name> <english_word> - Удаление слова из словаря.\n";
  out << "addtranslation <0_or_1> [dictionary_name] <english_word> <translation> ";
  out << "- Добавление перевода к слову.\n";
  out << "removetranslation <0_or_1> [dictionary_name] <english_word> <translation> - Удаление перевода слова.\n";
  out << "clear <dictionary_name> - Очистка словаря от всех слов.\n";
  out << "mergedictionaries <new_dictionary_name> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Объединение нескольких словарей в новый.\n";
  out << "subtractedictionaries <new_dictionary_name> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Вычитание словарей из первого в новый.\n";
  out << "commonpartdictionaries <new_dictionary_name> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Создание нового словаря из общих слов нескольких словарей.\n";
  out << "copytranslations <source_dictionary_name> <english_word> <number_of_dictionaries> <target_dictionary_name1> ... ";
  out << "- Копирование переводов слова из одного словаря в другие.\n";
  out << "printtranslations <english_word> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Вывод переводов слова в указанных словарях.\n";
  out << "printengwordswithtraslation <translation> <number_of_dictionaries> <dictionary_name1> ...  ";
  out << "- Вывод английских слов, содержащих этот перевод.\n";
  out << "count <dictionary_name> - Вывод количества слов в словаре.\n";
}

void tkach::printEngWordsWithTraslation(std::istream& in, std::ostream& out, const tree_of_dict& avltree)
{
  std::string translation;
  int number_of_dictionaries = 0;
  if (!(in >> translation) || translation.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  std::set< std::string > tree_word;
  std::vector< std::string > dict_names;
  std::copy_n(std::istream_iterator< std::string >{in}, number_of_dictionaries, std::back_inserter(dict_names));
  if (dict_names.size() != static_cast< size_t >(number_of_dictionaries))
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (std::any_of(dict_names.cbegin(), dict_names.cend(), IsStringEmpty{}) || in.eof())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  std::for_each(dict_names.cbegin(), dict_names.cend(), FindEnglishWordsWithTranslation{avltree, translation, tree_word});
  if (tree_word.empty())
  {
    throw std::logic_error("<NO WORD WITH THIS TRASLATION>");
  }
  else
  {
    out << *tree_word.cbegin();
    std::for_each(std::next(tree_word.cbegin()), tree_word.cend(), PrintStringWithLeadingSpace{out});
    out << "\n";
  }
}
