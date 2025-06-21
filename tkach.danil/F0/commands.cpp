#include "commands.hpp"
#include <string>
#include <fstream>
#include <algorithm>
#include <set>
#include <vector>
#include <iterator>

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

  std::list< std::string > mergeTranslations(const std::list< std::string >& list1, const std::list< std::string >& list2)
  {
    std::set< std::string > tree_translations;
    std::for_each(list1.cbegin(), list1.cend(), InsertIntoSet{tree_translations});
    std::for_each(list2.cbegin(), list2.cend(), InsertIntoSet{tree_translations});
    std::list< std::string > translations;
    std::copy(tree_translations.cbegin(), tree_translations.cend(), std::back_inserter(translations));
    return translations;
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
      out(os)
    {}
    void operator()(const std::string& s) const
    {
      out << " " << s;
    }
  private:
    std::ostream& out;
  };

  class WriteWordEntry
  {
  public:
    WriteWordEntry(std::ostream& os):
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
    out << dict_name << "\n";
    std::for_each(dict.cbegin(), dict.cend(), WriteWordEntry{out});
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

  std::map< std::string, std::list< std::string > > mergeDicts(const std::list< const tree_of_words* >& source_dicts)
  {
    if (source_dicts.empty())
    {
      throw std::logic_error("EMPTY");
    }
    tree_of_words temp_dict;
    for (auto it = source_dicts.cbegin(); it != source_dicts.cend(); ++it)
    {
      const tree_of_words& now_dict = *(*it);
      std::for_each(now_dict.cbegin(), now_dict.cend(), MergeWordsInDict{temp_dict});
    }
    return temp_dict;
  }

  void exportDictionaries(std::istream& in, const tree_of_dict& avltree, std::ios_base::openmode mode)
  {
    std::string filename;
    int count_of_dicts = 0;
    if (!(in >> filename) || filename.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    if (!(in >> count_of_dicts) || (count_of_dicts < 0))
    {
      throw std::logic_error("<INVALID NUMBER>");
    }
    std::ofstream outFile(filename, mode);
    if (!outFile.is_open())
    {
      throw std::logic_error("<EXPORT FAILED>");
    }
    if (mode == std::ios_base::app)
    {
      outFile << "\n";
    }
    if (count_of_dicts == 0)
    {
      if (avltree.empty())
      {
        return;
      }
      for (auto it = avltree.cbegin(); it != avltree.cend(); ++it)
      {
        writeDictToFile(outFile, it->first, it->second);
        outFile << "\n";
      }
    }
    else
    {
      std::string current_dict_name;
      for (size_t i = 0; i < static_cast< size_t >(count_of_dicts); ++i)
      {
        if (!(in >> current_dict_name) || current_dict_name.empty())
        {
          throw std::logic_error("<INVALID ARGUMENTS>");
        }
        auto it = avltree.find(current_dict_name);
        if (it == avltree.cend())
        {
          throw std::logic_error("<INVALID DICTIONARY>");
        }
        writeDictToFile(outFile, it->first, it->second);
        outFile << "\n";
      }
    }
  }
  struct IsStringEmpty
  {
    bool operator()(const std::string& s) const
    {
      return s.empty();
    }
  };

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
      target_avltree_[dict_name] = source_dict_.at(dict_name);
    }
  private:
    tree_of_dict& target_avltree_;
    const tree_of_dict& source_dict_;
  };

  class PrintDictEntry
  {
  public:
    PrintDictEntry(std::ostream& os):
      out(os)
    {}
    void operator()(const std::pair< const std::string, tree_of_words >& dict_pair) const
    {
      out << dict_pair.first << "\n";
      if (!dict_pair.second.empty())
      {
        std::for_each(dict_pair.second.cbegin(), dict_pair.second.cend(), WriteWordEntry{out});
      }
    }
  private:
    std::ostream& out;
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
        if (!findTranslation(it2->second, translation_))
        {
          it2->second.push_back(translation_);
        }
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
    void operator()(const std::pair< const std::string, std::list< std::string > >& pair) const
    {
      target_dict_->erase(pair.first);
    }
  private:
    tree_of_words* target_dict_;
  };

  class SubtractWordsFromDict
  {
  public:
    SubtractWordsFromDict(tree_of_words& target_dict):
      target_dict_(&target_dict)
    {}
    void operator()(const tree_of_words* dict_to_subtract) const
    {
      std::for_each(dict_to_subtract->cbegin(), dict_to_subtract->cend(),EraseWordFromTargetDict{*target_dict_});
    }
  private:
    tree_of_words* target_dict_;
  };
}

void tkach::import(std::istream& in, tree_of_dict& avltree)
{
  tree_of_dict temp(avltree);
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
  std::vector< std::string > main_name_dict(count_of_dict);
  if (count_of_dict > 0)
  {
    std::copy_n(std::istream_iterator< std::string >{in}, count_of_dict, main_name_dict.begin());
    if (std::any_of(main_name_dict.begin(), main_name_dict.end(), IsStringEmpty{}))
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
  }
  std::string name_of_dict = "";
  while (in2 >> name_of_dict)
  {
    if (name_of_dict.empty())
    {
      continue;
    }
    tree_of_words temp_dict;
    std::string eng_word;
    while (in2 >> eng_word)
    {
      std::list< std::string > translations;
      std::string translation;
      while (in2 >> translation)
      {
        translations.push_back(translation);
        if (in2.peek() == '\n')
        {
          in2.get();
          break;
        }
      }
      temp_dict[eng_word] = mergeTranslations(translations, temp_dict[eng_word]);
      if (in2.peek() == '\n')
      {
        in2.get();
        break;
      }
    }
    auto it = temp.find(name_of_dict);
    if (it == temp.end())
    {
      temp[name_of_dict] = temp_dict;
    }
    else
    {
      std::map< std::string, std::list< std::string > > merged = mergeDicts({&temp_dict, &temp[name_of_dict]});
      temp[name_of_dict] = merged;
    }
  }
  if (!in2.eof())
  {
    throw std::logic_error("<INVALID IMPORT>");
  }
  if (count_of_dict != 0)
  {
    std::for_each(main_name_dict.cbegin(), main_name_dict.cend(), ImportDictProcessor{avltree, temp});
  }
  else
  {
    avltree = std::move(temp);
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
  std::string translation;
  std::copy_n(std::istream_iterator< std::string >{in}, num_translations, std::back_inserter(translations));
  if (std::any_of(translations.begin(), translations.end(), IsStringEmpty()))
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
  std::list< std::string > common_translations;
  for (size_t i = 0; i < static_cast< size_t >(number_of_words); ++i)
  {
    std::string word;
    if (!(in >> word) || word.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto word_it = it->second.find(word);
    if (word_it == it->second.cend())
    {
      throw std::logic_error("<INVALID WORD>");
    }
    if (common_translations.empty())
    {
      common_translations = word_it->second;
    }
    else
    {
      common_translations = getCommonElements(common_translations, word_it->second);
    }
  }
  if (common_translations.empty())
  {
    out << "\n";
  }
  else
  {
    auto common_it = common_translations.cbegin();
    out << *common_it;
    std::copy(++common_it, common_translations.cend(), std::ostream_iterator< std::string >{out, " "});
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
  if (dict_name_specified)
  {
    if (!(in >> dict_name) || dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
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
    auto it = avltree.find(dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    auto it2 = it->second.find(eng_word);
    if (it2 == it->second.end())
    {
      throw std::logic_error("<INVALID WORD>");
    }
    if (!findTranslation(it2->second, translation))
    {
      it2->second.push_back(translation);
    }
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
  if (dict_name_specified)
  {
    if (!(in >> dict_name) || dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
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
    auto it = avltree.find(dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    auto it2 = it->second.find(eng_word);
    if (it2 == it->second.end())
    {
      throw std::logic_error("<INVALID WORD OR TRANSLATION>");
    }
    if (findTranslation(it2->second, translation))
    {
      it2->second.remove(translation);
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
  std::vector< tree_of_words* > source_dicts(number_of_dictionaries);
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    source_dicts[i] = &it->second;
  }
  tree_of_words result_dict(*source_dicts[0]);
  if (source_dicts.size() > 1)
  {
    std::for_each(source_dicts.begin() + 1, source_dicts.end(), SubtractWordsFromDict{result_dict});
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
  std::list< const tree_of_words* > source_dicts;
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    source_dicts.push_back(&(it->second));
  }
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
  std::vector< tree_of_words* > source_dicts(number_of_dictionaries);
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    source_dicts[i] = &it->second;
  }
  tree_of_words result_dict(*source_dicts[0]);
  for (size_t i = 1; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    const tree_of_words* dict = source_dicts[i];
    tree_of_words common;
    for (auto it = result_dict.begin(); it != result_dict.end(); ++it)
    {
      auto it_find = dict->find(it->first);
      if (it_find != dict->cend())
      {
        common[it->first] = mergeTranslations(it->second, it_find->second);
      }
    }
    result_dict = common;
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
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    tree_of_words& target = it->second;
    target[eng_word] = mergeTranslations(target[eng_word], word_it->second);
  }
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
  std::set< std::string > tree_translations;
  bool word_found = false;
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.cend())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    auto it2 = it->second.find(eng_word);
    if (it2 != it->second.cend())
    {
      word_found = true;
      std::copy(it2->second.cbegin(), it2->second.cend(), std::inserter(tree_translations, tree_translations.end()));
    }
  }
  if (!word_found)
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
  bool translation_found = false;
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.cend())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
    {
      if (findTranslation(it2->second, translation))
      {
        translation_found = true;
        tree_word.insert(it2->first);
      }
    }
  }
  if (!translation_found)
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
