#include "commands.hpp"
#include "functors.hpp"
#include <sstream>
#include <fstream>
#include <regex>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <numeric>

void shiryaeva::process_text_and_add(FrequencyDictionary &dict, const std::string &text)
{
  const std::regex re("([A-Za-z]+)");
  std::sregex_token_iterator it(text.begin(), text.end(), re, 1);
  std::sregex_token_iterator end;
  std::vector< std::string > words;
  std::copy(it, end, std::back_inserter(words));

  std::transform(words.begin(), words.end(), words.begin(),ProcessWord{dict});
}

void shiryaeva::create_dict(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name;
  if (!(args >> name) || name.empty())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.count(name))
  {
    out << "<DICT ALREADY EXISTS>\n";
    return;
  }
  dicts[name] = FrequencyDictionary{};
  out << "Создан словарь \"" << name << "\"\n";
}

void shiryaeva::add_text(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name, path;
  if (!(args >> name >> path))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<DICT NOT FOUND>\n";
    return;
  }

  std::ifstream ifs(path.c_str(), std::ios::binary);
  if (!ifs)
  {
    out << "<FILE NOT FOUND>\n";
    return;
  }

  std::string content((std::istreambuf_iterator< char >(ifs)), std::istreambuf_iterator< char >());
  process_text_and_add(it->second, content);
  out << "Текст из файла добавлен в \"" << name << "\"\n";
}

void shiryaeva::add_string(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name;
  if (!(args >> name))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<DICT NOT FOUND>\n";
    return;
  }
  std::string rest;
  std::getline(args, rest);
  if (!rest.empty() && rest.front() == ' ')
  {
    rest.erase(0, 1);
  }
  process_text_and_add(it->second, rest);
  out << "Текст добавлен в словарь \"" << name << "\"\n";
}

void shiryaeva::get_freq(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name, word;
  if (!(args >> name >> word))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<DICT NOT FOUND>\n";
    return;
  }
  
  std::string normalized_word = normalize_word(word);
  std::size_t f = it->second.get_freq(normalized_word);
  
  if (f == 0)
  {
    out << "<WORD NOT FOUND>\n";
  }
  else
  {
    out << "\"" << normalized_word << "\" — " << f << "\n";
  }
}

void shiryaeva::get_rel_freq(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name, word;
  if (!(args >> name >> word))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<DICT NOT FOUND>\n";
    return;
  }

  std::string normalized_word = normalize_word(word);
  double rel = it->second.get_rel_freq(normalized_word);
  
  if (rel == 0.0)
  {
    out << "<WORD NOT FOUND>\n";
  }
  else
  {
    out << "\"" << normalized_word << "\" — " << std::fixed << std::setprecision(6) << rel << "\n";
  }
}

void shiryaeva::remove_word(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name, word;
  if (!(args >> name >> word))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<DICT NOT FOUND>\n";
    return;
  }
  it->second.remove_word(normalize_word(word));
  out << "Слово удалено из \"" << name << "\"\n";
}

void shiryaeva::filter_by_freq(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name;
  double min_rel, max_rel;
  if (!(args >> name >> min_rel >> max_rel))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  
  if (min_rel < 0.0 || max_rel > 1.0 || min_rel > max_rel)
  {
    out << "<INVALID RANGE>\n";
    return;
  }
  
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<DICT NOT FOUND>\n";
    return;
  }

  std::vector< std::pair< std::string, size_t >> result;
  std::copy_if(it->second.dict.begin(), it->second.dict.end(), std::back_inserter(result), RelativeFreqFilter{min_rel, max_rel, it->second.total_words});

  if (result.empty())
  {
    out << "<NO WORDS IN RANGE>\n";
  }
  else
  {
    std::transform(result.begin(), result.end(), std::ostream_iterator< std::string >(out), Printer{});
  }
}

void shiryaeva::exclude_by_freq(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name;
  double min_rel, max_rel;
  if (!(args >> name >> min_rel >> max_rel))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  
  if (min_rel < 0.0 || max_rel > 1.0 || min_rel > max_rel)
  {
    out << "<INVALID RANGE>\n";
    return;
  }
  
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<DICT NOT FOUND>\n";
    return;
  }

  std::vector< std::pair< std::string, size_t >> result;
  std::copy_if(it->second.dict.begin(), it->second.dict.end(), std::back_inserter(result), ExcludeFreqFilter{min_rel, max_rel, it->second.total_words});

  if (result.empty())
  {
    out << "<NO WORDS OUTSIDE RANGE>\n";
  }
  else
  {
    std::transform(result.begin(), result.end(), std::ostream_iterator< std::string >(out), Printer{});
  }
}

void shiryaeva::display_all(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name;
  if (!(args >> name))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<DICT NOT FOUND>\n";
    return;
  }
  if (it->second.dict.empty())
  {
    out << "<DICT IS EMPTY>\n";
    return;
  }

  std::transform(it->second.dict.begin(), it->second.dict.end(), std::ostream_iterator< std::string >(out), Printer{});
}

void shiryaeva::clear_dict(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name;
  if (!(args >> name))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<DICT NOT FOUND>\n";
    return;
  }
  it->second.dict.clear();
  out << "Очищен словарь \"" << name << "\"\n";
}

void shiryaeva::merge_dicts(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string dict1, dict2, result_dict;
  if (!(args >> dict1 >> dict2 >> result_dict))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  
  if (dicts.count(result_dict))
  {
    out << "<RESULT DICT EXISTS>\n";
    return;
  }
  
  auto it1 = dicts.find(dict1);
  auto it2 = dicts.find(dict2);
  
  if (it1 == dicts.end())
  {
    out << "<DICT1 NOT FOUND>\n";
    return;
  }
  if (it2 == dicts.end())
  {
    out << "<DICT2 NOT FOUND>\n";
    return;
  }

  FrequencyDictionary merged;
  merged = it1->second;
  
  std::vector<std::string> all_words;
  std::transform(it2->second.dict.begin(), it2->second.dict.end(), std::back_inserter(all_words), GetKey{});
  std::transform(all_words.begin(), all_words.end(), all_words.begin(), MergeWord{merged, it2->second});

  dicts[result_dict] = merged;
  out << "Словари \"" << dict1 << "\" и \"" << dict2 << "\" объединены в \"" << result_dict << "\"\n";
}

void shiryaeva::most_common(Dictionaries &dicts, std::istream &args, std::ostream &out)
{
  std::string name;
  if (!(args >> name))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<DICT NOT FOUND>\n";
    return;
  }
  if (it->second.dict.empty())
  {
    out << "<DICT IS EMPTY>\n";
    return;
  }
  std::vector< std::pair< std::string, size_t >> vec;
  vec.insert(vec.end(), it->second.dict.begin(), it->second.dict.end());

  std::sort(vec.begin(), vec.end(), Cmp{});
  if (vec.size() > 3)
  {
    vec.resize(3);
  }

  std::transform(vec.begin(), vec.end(), std::ostream_iterator< std::string >(out), Printer{});
}
