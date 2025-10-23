#include "functors.hpp"

char shiryaeva::tolower_char(unsigned char c)
{
  return static_cast< char >(std::tolower(c));
}

std::string shiryaeva::normalize_word(const std::string &s)
{
  std::string r(s.size(), '\0');
  std::transform(s.begin(), s.end(), r.begin(), tolower_char);
  return r;
}

bool shiryaeva::RelativeFreqFilter::operator()(const std::pair< const std::string, size_t > &p) const
{
  if (total_words == 0) return false;
  double rel_freq = static_cast< double >(p.second) / total_words;
  return rel_freq >= min_rel && rel_freq <= max_rel;
}

bool shiryaeva::ExcludeFreqFilter::operator()(const std::pair< const std::string, size_t > &p) const
{
  if (total_words == 0) return false;
  double rel_freq = static_cast< double >(p.second) / total_words;
  return rel_freq < min_rel || rel_freq > max_rel;
}

bool shiryaeva::Cmp::operator()(const std::pair<std::string, size_t>& a, const std::pair< std::string, size_t >& b) const
{
  if (a.second != b.second) return a.second > b.second;
  return a.first < b.first;
}

std::string shiryaeva::Printer::operator()(const std::pair< std::string, size_t >& p) const
{
  return "\"" + p.first + "\" - " + std::to_string(p.second) + "\n";
}

void shiryaeva::WordInserter::push_back(const std::string& word)
{
  dict.add_word(normalize_word(word));
}

void shiryaeva::PrinterInserter::push_back(const std::pair<std::string, size_t>& pair)
{
  out << Printer{}(pair);
}

void shiryaeva::MergeInserter::push_back(const std::pair<const std::string, size_t>& pair)
{
  const std::string& word = pair.first;
  size_t count = pair.second;
  target.dict[word] += count;
  target.total_words += count;
}
