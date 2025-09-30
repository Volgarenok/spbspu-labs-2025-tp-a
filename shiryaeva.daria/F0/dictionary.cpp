#include "dictionary.hpp"

void shiryaeva::FrequencyDictionary::clear()
{
  dict.clear();
  total_words = 0;
}

void shiryaeva::FrequencyDictionary::add_word(const std::string &word)
{
  if (word.empty())
  {
    return;
  }
  std::map< std::string, size_t >::iterator it = dict.find(word);
  if (it == dict.end())
  {
    dict.emplace(word, 1);
  }
  else
  {
    ++(it->second);
  }
  ++total_words;
}

bool shiryaeva::FrequencyDictionary::remove_word(const std::string &word)
{
  std::map<std::string, size_t>::iterator it = dict.find(word);
  if (it == dict.end())
  {
    return false;
  }

  if (it->second <= total_words)
  {
    total_words -= it->second;
  }
  else
  {
    total_words = 0;
  }
  dict.erase(it);
  return true;
}

size_t shiryaeva::FrequencyDictionary::get_freq(const std::string &word) const
{
  std::map< std::string, size_t >::const_iterator it = dict.find(word);
  if (it == dict.end())
  {
    return 0;
  }
  return it->second;
}

double shiryaeva::FrequencyDictionary::get_rel_freq(const std::string &word) const
{
  if (total_words == 0)
  {
    return 0.0;
  }
  std::map< std::string, size_t >::const_iterator it = dict.find(word);
  if (it == dict.end())
  {
    return 0.0;
  }
  return static_cast< double >(it->second) / static_cast< double >(total_words);
}
