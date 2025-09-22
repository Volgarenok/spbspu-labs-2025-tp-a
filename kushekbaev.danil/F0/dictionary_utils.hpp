#ifndef DICTIONARY_UTILS_HPP
#define DICTIONARY_UTILS_HPP

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <fstream>
#include <iostream>

using dictionary_system = std::unordered_map< std::string, std::unordered_map< std::string, std::set< std::string > > >;
using dict_type = std::unordered_map< std::string, std::set< std::string > >;
using pair = std::pair< std::string, std::set< std::string > >;

namespace kushekbaev
{

  std::vector< std::string > split_line(std::string& line);

  struct Printer
  {
    std::ostream& out;
    std::string operator()(const pair& p) const;
  };

  struct TranslationTransformer
  {
    std::string operator()(const std::string& translation) const;
  };

  struct IsInMatchingWords
  {
    const std::vector< std::string >& words;
    bool operator()(const pair& entry) const;
  };

  struct WordPrinter
  {
    std::ostream& out;
    void operator()(const pair& entry) const;
  };

  struct HasNoTranslation
  {
    bool operator()(const pair& entry) const;
  };

  struct WordPrinterForFind
  {
    std::string operator()(const pair& entry) const;
  };

  struct HasPrefix
  {
    std::string& prefix;
    bool operator()(const pair& entry) const;
  };

  struct NoPrefix
  {
    std::string& prefix;
    bool operator()(const pair& entry) const;
  };

  struct HasSuffix
  {
    std::string& suffix;
    bool operator()(const pair& entry) const;
  };

  struct NoSuffix
  {
    std::string& suffix;
    bool operator()(const pair& entry) const;
  };

  struct OutputTransformer
  {
    std::ostream& out;
    std::string operator()(const pair& entry) const;
  };

  struct DictionaryBuilder
  {
    dict_type& result_dict;
    std::string operator()(const pair& entry) const;
  };

  struct LessThanDelimiter
  {
    std::string& delimiter;
    bool operator()(const pair& entry) const;
  };

  struct GreaterOrEqualDelimiter
  {
    std::string& delimiter;
    bool operator()(const pair& entry) const;
  };

  struct IsInDict2
  {
    const dict_type& dict2;
    bool operator()(const pair& entry) const;
  };

  struct MergeTranslations
  {
    const dict_type& dict2;
    pair operator()(const pair& entry) const;
  };

  struct DictionaryMerger
  {
    dict_type& target_dict;
    std::string operator()(const pair& entry);
  };

  struct TranslationProcessor
  {
    const std::string& translation;
    size_t& count;
    pair operator()(const pair& entry);
  };

  struct EmptyTranslationChecker
  {
    std::vector< std::string >& to_erase;
    pair operator()(const pair& entry);
  };

  struct NonEmptyFilter
  {
    bool operator()(const pair& entry);
  };

  struct DictionarySaver
  {
    std::string operator()(const std::pair< const std::string, dict_type >& dict_entry);
  };

  struct IsNotInDict2
  {
    const dict_type& dict2;
    bool operator()(const std::pair<const std::string, std::set<std::string>>& entry);
  };

  struct HasTranslation
  {
    std::string& translation;
    bool operator()(const std::pair< std::string, std::set< std::string > >& entry) const;
  };

  struct KeyExtractor
  {
    std::string operator()(const std::pair< std::string, std::set< std::string > >& entry) const;
  };

  struct FileImporter
  {
    std::ifstream& file;
    dictionary_system& dicts;
    std::string current_dict_name;
    bool in_dict = false;
    size_t line_count = 0;
    void operator()();
  };

  struct TranslationRemover
  {
    const std::string& translation_to_delete;
    std::vector< std::string >& words_to_erase;
    size_t& removed_count;
    dict_type& word_map;
    void operator()();
  };

  struct WordEntrySaver
  {
    std::string operator()(const std::pair< const std::string, std::set< std::string > >& word_entry);
  };
}

#endif
