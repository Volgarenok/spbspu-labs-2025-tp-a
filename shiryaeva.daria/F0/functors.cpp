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

bool shiryaeva::MinFreqFilter::operator()(const std::pair< const std::string, size_t > &p) const
{
  return p.second >= minf;
}

std::string shiryaeva::MaxFreqFilter::operator()(const std::pair< const std::string, size_t > &p) const
{
  return (p.second > maxf) ? p.first : std::string{};
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

size_t shiryaeva::AddFreq::operator()(size_t acc, const std::pair<const std::string, size_t>& p) const
{
  return acc + p.second;
}

void shiryaeva::Adder::operator()(const std::string &w) const
{
  d.add_word(normalize_word(w));
}

std::string shiryaeva::GetKey::operator()(const std::pair<const std::string, size_t>& p) const
{
  return p.first;
}

std::string shiryaeva::ProcessWord::operator()(const std::string& w) const
{
  Adder{dict}(w);
  return w;
}

 std::string shiryaeva::MergeWord::operator()(const std::string& word) const
{
  size_t count = source.dict.at(word);

  target.dict[word] += count;
  target.total_words += count;
  return word;
}

std::string shiryaeva::RemoveWord::operator()(const std::string& key) const
{
  dict.dict.erase(key);
  return key;
}
