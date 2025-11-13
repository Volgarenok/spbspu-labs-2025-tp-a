#include "commands.hpp"
#include "functors.hpp"
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <numeric>
#include <vector>

void shiryaeva::process_text_and_add(FrequencyDictionary &dict, const std::string &text)
{
  std::vector< std::string > words;

  std::istringstream iss(text);
  std::copy(std::istream_iterator< std::string >(iss), std::istream_iterator< std::string >(), std::back_inserter(words));

  WordInserter inserter{dict};
  std::copy(words.begin(), words.end(), std::back_inserter(inserter));
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
  size_t f = it->second.get_freq(normalized_word);

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
  RelativeFreqFilter filter{min_rel, max_rel, it->second.total_words};
  std::copy_if(it->second.dict.begin(), it->second.dict.end(), std::back_inserter(result), filter);

  if (result.empty())
  {
    out << "<NO WORDS IN RANGE>\n";
  }
  else
  {
    PrinterInserter printer{out};
    std::copy(result.begin(), result.end(), std::back_inserter(printer));
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
  ExcludeFreqFilter filter{min_rel, max_rel, it->second.total_words};
  std::copy_if(it->second.dict.begin(), it->second.dict.end(), std::back_inserter(result), filter);

  if (result.empty())
  {
    out << "<NO WORDS OUTSIDE RANGE>\n";
  }
  else
  {
    PrinterInserter printer{out};
    std::copy(result.begin(), result.end(), std::back_inserter(printer));
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

  PrinterInserter printer{out};
  std::copy(it->second.dict.begin(), it->second.dict.end(), std::back_inserter(printer));
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

  MergeInserter merger{merged};
  std::copy(it2->second.dict.begin(), it2->second.dict.end(), std::back_inserter(merger));

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

  PrinterInserter printer{out};
  std::copy(vec.begin(), vec.end(), std::back_inserter(printer));
}
