#include "dictionary_utils.hpp"
#include <algorithm>
#include <sstream>
#include <iterator>

using pair = std::pair< std::string, std::set< std::string > >;
using out_it = std::ostream_iterator< std::string >;

std::vector< std::string > kushekbaev::split_line(std::string& line)
{
  std::vector< std::string > tokens;
  size_t pos = 0;
  while (true)
  {
    size_t start = line.find_first_not_of(" \t", pos);
    if (start == std::string::npos)
    {
      break;
    }
    size_t end = line.find_first_of(" \t", start);
    if (end == std::string::npos)
    {
      end = line.size();
    }
    tokens.push_back(line.substr(start, end - start));
    pos = end;
  }
  return tokens;
}

std::string kushekbaev::Printer::operator()(const pair& p) const
{
  std::ostringstream oss;
  oss << p.first << " : ";
  std::copy(p.second.begin(), p.second.end(), out_it(oss, ", "));
  auto result = oss.str();
  if (!result.empty())
  {
    result.erase(result.size() - 2);
  }
  return result;
}

std::string kushekbaev::TranslationTransformer::operator()(const std::string& translation) const
{
  return translation;
}

bool kushekbaev::IsInMatchingWords::operator()(const pair& entry) const
{
  return std::find(words.begin(), words.end(), entry.first) != words.end();
}

void kushekbaev::WordPrinter::operator()(const pair& entry) const
{
  out << "-> " << entry.first << " : ";
  if (!entry.second.empty())
  {
    out << *entry.second.begin();
    std::copy(std::next(entry.second.begin()), entry.second.end(), out_it(out, ", "));
  }
  out << "\n";
}

bool kushekbaev::NoPrefix::operator()(const pair& entry) const
{
  return entry.first.rfind(prefix, 0) != 0;
}

bool kushekbaev::HasPrefix::operator()(const pair& entry) const
{
  return entry.first.rfind(prefix, 0) == 0;
}

bool kushekbaev::NoSuffix::operator()(const pair& entry) const
{
  return entry.first.rfind(suffix, 0) != 0;
}

bool kushekbaev::HasSuffix::operator()(const pair& entry) const
{
  return entry.first.rfind(suffix, 0) == 0;
}

std::string kushekbaev::OutputTransformer::operator()(const pair& entry) const
{
  std::ostringstream oss;
  oss << "-> " << entry.first << " : ";
  if (!entry.first.empty())
  {
    std::copy(entry.second.begin(), entry.second.end(), out_it(oss, ", "));
    std::string result = oss.str();
    if (result.size() > 2)
    {
      result = result.substr(0, result.size() - 2);
    }
    return result;
  }
  return oss.str();
}

std::string kushekbaev::DictionaryBuilder::operator()(const pair& entry) const
{
  result_dict[entry.first].insert(entry.second.begin(), entry.second.end());
  return "";
}

bool kushekbaev::LessThanDelimiter::operator()(const pair& entry) const
{
  return entry.first < delimiter;
}

bool kushekbaev::GreaterOrEqualDelimiter::operator()(const pair& entry) const
{
  return entry.first >= delimiter;
}

bool kushekbaev::HasNoTranslation::operator()(const pair& entry) const
{
  return entry.second.empty();
}

std::string kushekbaev::WordPrinterForFind::operator()(const pair& entry) const
{
  return "-> " + entry.first + "\n";
}

bool kushekbaev::IsInDict2::operator()(const pair& entry) const
{
  return dict2.find(entry.first) != dict2.end();
}

pair kushekbaev::MergeTranslations::operator()(const pair& entry) const
{
  auto dict2_it = dict2.find(entry.first);
  std::set< std::string > merged_translations = entry.second;
  if (dict2_it != dict2.end())
  {
    merged_translations.insert(dict2_it->second.begin(), dict2_it->second.end());
  }
  return { entry.first, merged_translations };
}

std::string kushekbaev::DictionaryMerger::operator()(const pair& entry)
{
  target_dict[entry.first].insert(entry.second.begin(), entry.second.end());
  return "";
}

std::string kushekbaev::WordEntrySaver::operator()(const std::pair< const std::string, std::set< std::string > >& word_entry)
{
  std::ostringstream word_oss;
  word_oss << word_entry.first;
  if (!word_entry.second.empty())
  {
    word_oss << " ";
    std::copy(word_entry.second.begin(), word_entry.second.end(), out_it(word_oss, " "));
  }
  word_oss << "\n";
  return word_oss.str();
}


std::string kushekbaev::DictionarySaver::operator()(const std::pair< const std::string, dict_type >& entry)
{
  std::ostringstream oss;
  oss << "[ " << entry.first << " ]\n";
  WordEntrySaver word_saver;
  std::transform(entry.second.begin(), entry.second.end(), out_it(oss), word_saver);
  oss << "\n";
  return oss.str();
}

bool kushekbaev::IsNotInDict2::operator()(const std::pair< const std::string, std::set< std::string > >& entry)
{
  return dict2.find(entry.first) == dict2.end();
}

std::string kushekbaev::KeyExtractor::operator()(const std::pair< std::string, std::set< std::string > >& entry) const
{
  return entry.first;
}

bool kushekbaev::HasTranslation::operator()(const std::pair< std::string, std::set< std::string > >& entry) const
{
  return entry.second.find(translation) != entry.second.end();
}

pair kushekbaev::TranslationProcessor::operator()(const pair& entry)
{
  auto modified_entry = entry;
  if (modified_entry.second.erase(translation))
  {
    ++count;
  }
  return modified_entry;
}

pair kushekbaev::EmptyTranslationChecker::operator()(const pair& entry)
{
  if (entry.second.empty())
  {
    to_erase.push_back(entry.first);
  }
  return entry;
}

bool kushekbaev::NonEmptyFilter::operator()(const pair& entry)
{
  return !entry.second.empty();
}

void kushekbaev::TranslationRemover::operator()()
{
  std::vector< std::pair< std::string, std::set< std::string > > > temp_entries;
  std::copy(word_map.begin(), word_map.end(), std::back_inserter(temp_entries));

  std::vector< std::pair< std::string, std::set< std::string > > > processed_entries;
  std::transform(temp_entries.begin(), temp_entries.end(), std::back_inserter(processed_entries),
    TranslationProcessor{ translation_to_delete, removed_count });

  std::vector< std::pair< std::string, std::set< std::string > > > checked_entries;
  std::transform(processed_entries.begin(), processed_entries.end(), std::back_inserter(checked_entries),
    EmptyTranslationChecker{ words_to_erase });

  word_map.clear();
  std::copy_if(checked_entries.begin(), checked_entries.end(), std::inserter(word_map, word_map.end()),
    NonEmptyFilter{});
}

void kushekbaev::FileImporter::operator()()
{
  std::string line;
  while (std::getline(file, line))
  {
    ++line_count;
    auto start = line.find_first_not_of(" \r\n");
    if (start == std::string::npos)
    {
      in_dict = false;
      continue;
    }
    auto end = line.find_last_not_of(" \r\n");
    std::string trimmed = line.substr(start, end - start + 1);
    if (trimmed.size()  > 2 && trimmed[0] == '[' && trimmed[trimmed.size()-1] == ']')
    {
      current_dict_name = trimmed.substr(1, trimmed.size()-2);
      start = current_dict_name.find_first_not_of(" ");
      end = current_dict_name.find_last_not_of(" ");
      if (start != std::string::npos)
      {
        current_dict_name = current_dict_name.substr(start, end - start + 1);
      }
      if (current_dict_name.empty())
      {
        throw std::runtime_error("Empty dictionary name at line " + std::to_string(line_count));
      }
      in_dict = true;
      continue;
    }
    if (in_dict && !current_dict_name.empty() && !trimmed.empty())
    {
      auto tokens = split_line(trimmed);
      if (!tokens.empty())
      {
        std::string word = tokens[0];
        std::set< std::string > translations;
        if (tokens.size()  > 1)
        {
          for (size_t i = 1; i <  tokens.size(); ++i)
          {
            translations.insert(tokens[i]);
          }
        }
        dicts[current_dict_name][word].insert(translations.begin(), translations.end());
      }
    }
  }
}
