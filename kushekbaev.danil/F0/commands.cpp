#include "commands.hpp"
#include <stdexcept>
#include <functional>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include "dictionary_utils.hpp"

using dictionary_system = std::unordered_map< std::string, std::unordered_map< std::string, std::set < std::string > > >;
using dict_type = std::unordered_map< std::string, std::set< std::string > >;
using out_it = std::ostream_iterator< std::string >;

void kushekbaev::insert(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, inputed_word, inputed_translation;
  in >> dictionary_name >> inputed_word >> inputed_translation;
  auto& word_map = current_dictionary_system[dictionary_name];
  auto& translation_set = word_map[inputed_word];
  if (translation_set.count(inputed_translation))
  {
    throw std::logic_error("<THIS TRANSLATION ALREADY EXISTS>");
  }
  translation_set.insert(inputed_translation);
  out << "Translation inserted successfully.\n";
}

void kushekbaev::insert_without_translation(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, inputed_word;
  in >> dictionary_name >> inputed_word;
  auto& word_map = current_dictionary_system[dictionary_name];
  word_map[inputed_word];
  out << "Successfully inserted without translation!\n";
}

void kushekbaev::print(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name;
  in >> dictionary_name;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& word_map = dict_it->second;
  out << "In " << dictionary_name << ":\n";
  if (!word_map.empty())
  {
    Printer printer{ out };
    std::transform(word_map.begin(), word_map.end(), out_it(out, "\n"), printer);
  }
}

void kushekbaev::remove(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, word_to_remove;
  in >> dictionary_name >> word_to_remove;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto word_it = dict_it->second.find(word_to_remove);
  if (word_it == dict_it->second.end())
  {
    throw std::out_of_range("<WORD NOT FOUND>");
  }
  dict_it->second.erase(word_it);
  out << "Word with its translation successfully removed.\n";
}

void kushekbaev::save(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string filename;
  in >> filename;
  std::ofstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open/create your file!");
  }
  if (!current_dictionary_system.empty())
  {
    DictionarySaver saver;
    std::transform(current_dictionary_system.begin(), current_dictionary_system.end(), out_it(file), saver);
  }
  out << "Dictionary system successfully saved.\n";
}

void kushekbaev::import_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string filename;
  in >> filename;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open your file!");
  }
  FileImporter importer{ file, current_dictionary_system };
  importer();
  out << "Successfully imported file.\n";
}

void kushekbaev::search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, word_to_find;
  in >> dictionary_name >> word_to_find;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& word_map = dict_it->second;
  auto word_it = word_map.find(word_to_find);
  if (word_it == word_map.end())
  {
    throw std::out_of_range("<WORD NOT FOUND>");
  }
  auto& translations = word_it->second;
  out << word_it->first;
  if (!translations.empty())
  {
    out << " ";
    std::copy(translations.begin(), translations.end(), out_it(out, " "));
  }
  out << "\n";
}

void kushekbaev::clear_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name;
  in >> dictionary_name;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  current_dictionary_system.erase(dict_it);
  out << "Dictionary successfully deleted\n";
}

void kushekbaev::reverse_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, translation_to_find;
  in >> dictionary_name >> translation_to_find;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  std::vector< std::string > matching_words;
  std::vector< std::pair< std::string, std::set< std::string > > > matching_entries;
  std::copy_if(dict_it->second.begin(), dict_it->second.end(),
    std::back_inserter(matching_entries), HasTranslation{ translation_to_find });
  std::transform(matching_entries.begin(), matching_entries.end(),
    std::back_inserter(matching_words), KeyExtractor{});
  if (matching_words.empty())
  {
    throw std::out_of_range("<TRANSLATION NOT FOUND>");
  }
  out << "Words with translation *" << translation_to_find << "*: " << matching_words[0];
  std::copy(std::next(matching_words.begin()), matching_words.end(), out_it(out, ", "));
  out << "\n";
}

void kushekbaev::remove_translation(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, word_to_find, translation_to_delete;
  in >> dictionary_name >> word_to_find >> translation_to_delete;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& word_map = dict_it->second;
  auto word_it = word_map.find(word_to_find);
  if (word_it == word_map.end())
  {
    throw std::out_of_range("<WORD NOT FOUND>");
  }
  auto& translations = word_it->second;
  auto translation_it = translations.find(translation_to_delete);
  if (translation_it == translations.end())
  {
    throw std::out_of_range("<TRANSLATION NOT FOUND>");
  }
  translations.erase(translation_it);
  out << "Translation successfully deleted.\n";
}

void kushekbaev::remove_translation_at_all(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, translation_to_delete;
  in >> dictionary_name >> translation_to_delete;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  std::vector< std::string > words_to_erase;
  size_t removed_count = 0;
  if (!dict_it->second.empty())
  {
    TranslationRemover remover{ translation_to_delete, words_to_erase, removed_count, dict_it->second };
    remover();
  }
  if (removed_count == 0)
  {
    throw std::out_of_range("<TRANSLATION NOT FOUND>");
  }
  out << "Removed " << removed_count << " instances of translation '" << translation_to_delete << ".'\n";
}

void kushekbaev::delete_all_translations(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, translation_to_delete;
  in >> dictionary_name >> translation_to_delete;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }

  std::vector< std::string > matching_words;
  std::vector< std::pair< std::string, std::set< std::string > > > matching_entries;

  std::copy_if(dict_it->second.begin(), dict_it->second.end(),
    std::back_inserter(matching_entries), HasTranslation{ translation_to_delete });
  std::transform(matching_entries.begin(), matching_entries.end(),
    std::back_inserter(matching_words), KeyExtractor{});

  if (matching_words.empty())
  {
    throw std::out_of_range("<TRANSLATION NOT FOUND>");
  }
  dict_type new_dict;
  std::remove_copy_if(dict_it->second.begin(), dict_it->second.end(),
    std::inserter(new_dict, new_dict.end()), IsInMatchingWords{ matching_words });
  out << "Words with this translation deleted successfully.\n";
}

void kushekbaev::prefix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, prefix;
  in >> dictionary_name >> prefix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  out << "Words with prefix '" << prefix << "':\n";
  std::vector< std::pair< std::string, std::set< std::string > > > matching_entries;
  std::copy_if(dict_it->second.begin(), dict_it->second.end(), std::back_inserter(matching_entries), HasPrefix{ prefix });
  if (matching_entries.empty())
  {
    out << "<NO WORDS FOUND>\n";
  }
  else
  {
    std::transform(matching_entries.begin(), matching_entries.end(), out_it(out, ", "), OutputTransformer{ out });
  }
}

void kushekbaev::no_prefix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, prefix;
  in >> dictionary_name >> prefix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  out << "Words without prefix '" << prefix << "':\n";
  std::vector< std::pair< std::string, std::set< std::string > > > matching_entries;
  std::copy_if(dict_it->second.begin(), dict_it->second.end(), std::back_inserter(matching_entries), NoPrefix{ prefix });
  if (matching_entries.empty())
  {
    out << "<NO WORDS FOUND>\n";
  }
  else
  {
    std::transform(matching_entries.begin(), matching_entries.end(), out_it(out, ", "), OutputTransformer{ out });
  }
}

void kushekbaev::suffix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, suffix;
  in >> dictionary_name >> suffix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  out << "Words with suffix '" << suffix << "':\n";
  std::vector< std::pair< std::string, std::set< std::string > > > matching_entries;
  std::copy_if(dict_it->second.begin(), dict_it->second.end(), std::back_inserter(matching_entries), HasSuffix{ suffix });
  if (matching_entries.empty())
  {
    out << "<NO WORDS FOUND>\n";
  }
  else
  {
    std::transform(matching_entries.begin(), matching_entries.end(), out_it(out, ", "), OutputTransformer{ out });
  }
}

void kushekbaev::no_suffix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, suffix;
  in >> dictionary_name >> suffix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  out << "Words with bo suffix '" << suffix << "':\n";
  std::vector< std::pair< std::string, std::set< std::string > > > matching_entries;
  std::copy_if(dict_it->second.begin(), dict_it->second.end(), std::back_inserter(matching_entries), NoSuffix{ suffix });
  if (matching_entries.empty())
  {
    out << "<NO WORDS FOUND>\n";
  }
  else
  {
    std::transform(matching_entries.begin(), matching_entries.end(), out_it(out, ", "), OutputTransformer{ out });
  }
}

void kushekbaev::merge(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_dictionary, first_dictionary, second_dictionary;
  in >> new_dictionary >> first_dictionary >> second_dictionary;
  auto first_dict_it = current_dictionary_system.find(first_dictionary);
  if (first_dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<FIRST DICTIONARY NOT FOUND>");
  }
  auto second_dict_it = current_dictionary_system.find(second_dictionary);
  if (second_dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<SECOND DICTIONARY NOT FOUND>");
  }
  if (current_dictionary_system.count(new_dictionary))
  {
    throw std::runtime_error("<NEW DICTIONARY NAME ALREADY EXISTS>");
  }
  auto& new_dict = current_dictionary_system[new_dictionary];
  std::vector< std::pair < std::string, std::set< std::string > > > all_entries;
  std::copy(first_dict_it->second.begin(), first_dict_it->second.end(), std::back_inserter(all_entries));
  std::copy(second_dict_it->second.begin(), second_dict_it->second.end(), std::back_inserter(all_entries));
  std::transform(all_entries.begin(), all_entries.end(), out_it(out), DictionaryBuilder{ new_dict });
  out << "Successfully merged!\n";
}

void kushekbaev::split(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_dictionary_1, new_dictionary_2, original_dictionary, delimiter;
  in >> new_dictionary_1 >> new_dictionary_2 >> original_dictionary >> delimiter;
  auto original_dict_it = current_dictionary_system.find(original_dictionary);
  if (original_dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<ORIGINAL DICTIONARY NOT FOUND>");
  }
  if (current_dictionary_system.count(new_dictionary_1) || current_dictionary_system.count(new_dictionary_2))
  {
    throw std::runtime_error("<NEW DICTIONARY NAME ALREADY EXISTS>");
  }
  auto& dict1 = current_dictionary_system[new_dictionary_1];
  auto& dict2 = current_dictionary_system[new_dictionary_2];
  std::copy_if(original_dict_it->second.begin(), original_dict_it->second.end(), std::inserter(dict1, dict1.end()),
    LessThanDelimiter{ delimiter });
  std::copy_if(original_dict_it->second.begin(), original_dict_it->second.end(), std::inserter(dict2, dict2.end()),
    GreaterOrEqualDelimiter{ delimiter });
  out << "Successfully splitted.\n";
}

void kushekbaev::find_words_without_translations(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary;
  in >> dictionary;
  auto dict_it = current_dictionary_system.find(dictionary);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  out << "Words without translations in dictionary '" << dictionary << "':\n";
  std::vector< std::pair< std::string, std::set< std::string > > > empty_words;
  std::copy_if(dict_it->second.begin(), dict_it->second.end(), std::back_inserter(empty_words),
    HasNoTranslation{});
  if (empty_words.empty())
  {
    out << "<ALL WORDS HAVE TRANSLATIONS>\n";
  }
  else
  {
    std::transform(empty_words.begin(), empty_words.end(), out_it(out), WordPrinterForFind{});
  }
}

void kushekbaev::complement(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_name, dict1_name, dict2_name;
  in >> new_name >> dict1_name >> dict2_name;
  auto dict1_it = current_dictionary_system.find(dict1_name);
  auto dict2_it = current_dictionary_system.find(dict2_name);
  if (dict1_it == current_dictionary_system.end() || dict2_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  if (dict1_name == dict2_name)
  {
    current_dictionary_system[new_name] = {};
    out << "Created empty dictionary (same dictionaries complemented)\n";
    return;
  }
  auto& new_dict = current_dictionary_system[new_name] = {};
  const auto& dict1 = dict1_it->second;
  const auto& dict2 = dict2_it->second;
  if (!dict1.empty())
  {
    std::copy_if(dict1.begin(), dict1.end(), std::inserter(new_dict, new_dict.end()), IsNotInDict2{ dict2 });
  }
  out << "Complement dictionary created with " << new_dict.size() << " words\n";
}

void kushekbaev::intersect(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_name, dict1_name, dict2_name;
  in >> new_name >> dict1_name >> dict2_name;
  auto dict1_it = current_dictionary_system.find(dict1_name);
  auto dict2_it = current_dictionary_system.find(dict2_name);
  if (dict1_it == current_dictionary_system.end() || dict2_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& new_dict = current_dictionary_system[new_name] = {};
  const auto& dict1 = dict1_it->second;
  const auto& dict2 = dict2_it->second;
  if (dict1_name == dict2_name)
  {
    new_dict = dict1;
    out << "Dictionary copied (same dictionaries intersected)\n";
    return;
  }
  if (!dict1.empty())
  {
    std::vector< std::pair< std::string, std::set< std::string > > > common_words;
    std::copy_if(dict1.begin(), dict1.end(), std::back_inserter(common_words), IsInDict2{ dict2 });
    std::transform(common_words.begin(), common_words.end(), std::inserter(new_dict, new_dict.end()),
      MergeTranslations{ dict2 });
  }
  out << "Intersection created with " << new_dict.size() << " common words\n";
}

void kushekbaev::unification(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_name, dict1_name, dict2_name;
  in >> new_name >> dict1_name >> dict2_name;

  auto dict1_it = current_dictionary_system.find(dict1_name);
  auto dict2_it = current_dictionary_system.find(dict2_name);
  if (dict1_it == current_dictionary_system.end() || dict2_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  if (current_dictionary_system.count(new_name))
  {
    throw std::runtime_error("<NEW DICTIONARY NAME ALREADY EXISTS>");
  }
  current_dictionary_system[new_name] = dict1_it->second;
  auto& new_dict = current_dictionary_system[new_name];
  const auto& dict2 = dict2_it->second;
  DictionaryMerger merger{ new_dict };
  std::transform(dict2.begin(), dict2.end(), out_it(out), merger);
  out << "Unification dictionary created successfully\n";
}

void kushekbaev::print_help(std::ostream& out)
{
  out << "Available commands: insert, ...\n";
}
