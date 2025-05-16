#include "FrequencyDictionary.hpp"
#include <fstream>
#include <cctype>
#include <cmath>
#include <regex>
#include <iomanip>
#include <algorithm>
#include "functors.hpp"
#include <scopeGuard.hpp>

void shapkov::analyze_text(std::istream& in, std::ostream& out, FrequencyDictionary& dict)
{
  std::string fileName, id;
  in >> fileName >> id;
  if (dict.dicts.find(id) != dict.dicts.end())
  {
    out << "<BUSY ID>\n";
    return;
  }
  OneFreqDict temp;
  std::ifstream text(fileName);
  if (!text.is_open())
  {
    out << "<FILE NOT FOUND>\n";
    return;
  }
  std::string word;
  while (text >> word)
  {
    cleanWord(word);
    if (!word.empty())
    {
      temp.dictionary[word]++;
      temp.size++;
    }
  }
  if (temp.size == 0)
  {
    out << "<EMPTY TEXT>\n";
    return;
  }
  dict.dicts.emplace(std::move(id), std::move(temp));
}

void shapkov::word_info(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string word, id;
  in >> word >> id;
  auto text = dict.dicts.find(id);
  if (text == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  auto wrd = text->second.dictionary.find(word);
  if (wrd == text->second.dictionary.end())
  {
    out << "<WORD NOT FOUND>\n";
  }
  else
  {
    out << wrd->first << ": " << wrd->second << '\n';
  }
}

void shapkov::anagrams(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string word;
  in >> word;
  isAnagram anagramChecker{ word };
  size_t anagramsCnt = 0;
  for (const auto& dict_pair: dict.dicts)
  {
    for (const auto& word_pair: dict_pair.second.dictionary)
    {
      if (anagramChecker(word_pair.first))
      {
        out << dict_pair.first << ": word - " << word_pair.first << '\n';
        anagramsCnt++;
      }
    }
  }
  if (anagramsCnt == 0)
  {
    out << "<NO ANAGRAMS>\n";
  }
}

void shapkov::similar_frequency(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string word;
  size_t delta = 0;
  in >> word >> delta;
  size_t freqCnt = 0;
  for (const auto& dict_pair: dict.dicts)
  {
    auto wrd = dict_pair.second.dictionary.find(word);
    if (wrd != dict_pair.second.dictionary.end())
    {
      for (const auto& word_pair: dict_pair.second.dictionary)
      {
        if ((word_pair.second <= wrd->second + delta) && (word_pair.second >= wrd->second - delta))
        {
          out << dict_pair.first << ": " << word_pair.first << " - " << word_pair.second << '\n';
          freqCnt++;
        }
      }
    }
  }
  if (freqCnt == 0)
  {
    out << "<NO WORD>\n";
  }
}

double shapkov::entropyCount(const OneFreqDict& text)
{
  double entropy = 0;
  for (const auto& word_pair: text.dictionary)
  {
    double wordProbability = static_cast<double>(word_pair.second) / static_cast<double>(text.size);
    entropy -= wordProbability * log2(wordProbability);
  }
  return entropy;
}

void shapkov::entropy(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string id;
  in >> id;
  auto text = dict.dicts.find(id);
  if (text == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  double entropy = entropyCount(text->second);
  out << "Entropy of text with id " << id << " is " << entropy << " bits/words. It means that ";
  if (entropy < 3)
  {
    out << "the text is extremely repetitive (like spam or system logs). It contains almost no information diversity.\n";
  }
  else if (entropy >= 3 && entropy < 5)
  {
    out << "the text uses rigid templates (like legal documents or manuals). It's very predictable with limited vocabulary.\n";
  }
  else if (entropy >= 5 && entropy < 7)
  {
    out << "the text represents everyday communication (news, social media). It balances simplicity and information.\n";
  }
  else if (entropy >= 7 && entropy < 10)
  {
    out << "the text shows natural language complexity (quality articles, literature). It has good lexical variety.\n";
  }
  else if (entropy >= 10 && entropy < 13)
  {
    out << "the text contains specialized vocabulary (academic papers, technical docs). It requires domain knowledge.\n";
  }
  else
  {
    out << "the text approaches random complexity (encrypted data or AI artifacts). It may be non-natural language.\n";
  }
}

void shapkov::total_words(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string id;
  in >> id;
  auto text = dict.dicts.find(id);
  if (text == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  out << "Words count: " << text->second.size << '\n';
}

void shapkov::palindromes(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string id;
  in >> id;
  auto text = dict.dicts.find(id);
  if (text == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  size_t palindromesCnt = 0;
  for (const auto& word_pair: text->second.dictionary)
  {
    if (isPalindrome(word_pair.first))
    {
      out << word_pair.first << '\n';
      palindromesCnt++;
    }
  }
  if (palindromesCnt == 0)
  {
    out << "<NO PALINDROMES>\n";
  }
}

void shapkov::show_with_pattern(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string pattern, id;
  in >> pattern >> id;
  auto text = dict.dicts.find(id);
  if (text == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  std::regex patt(pattern);
  size_t patternMatches = 0;
  for (const auto& word_pair: text->second.dictionary)
  {
    if (std::regex_match(word_pair.first, patt))
    {
      out << word_pair.first << '\n';
      patternMatches++;
    }
  }
  if (patternMatches == 0)
  {
    out << "<NO MATCHES>\n";
  }
}

void shapkov::print(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string id;
  in >> id;
  auto text = dict.dicts.find(id);
  if (text == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  for (const auto& word_pair: text->second.dictionary)
  {
    out << word_pair.first << ": " << word_pair.second << '\n';
  }
}

std::string shapkov::topWord(const OneFreqDict& text)
{
  size_t topWordFreq = 0;
  auto topWord = text.dictionary.begin();
  for (auto it = text.dictionary.begin(); it != text.dictionary.end(); it++)
  {
    if (it->second > topWordFreq)
    {
      topWordFreq = it->second;
      topWord = it;
    }
  }
  return topWord->first;
}

void shapkov::top(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string id;
  in >> id;
  auto text = dict.dicts.find(id);
  if (text == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  std::string topWrd = topWord(text->second);
  out << "Top word with frequency " << text->second.dictionary.find(topWrd)->second << " is \"" << topWrd << '\"' << ".\n";
}

void shapkov::compare(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  ScopeGuard scopeGuard(out);
  std::string id1, id2;
  in >> id1 >> id2;
  auto text1 = dict.dicts.find(id1);
  auto text2 = dict.dicts.find(id2);
  if (text1 == dict.dicts.end() || text2 == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  double entropy1 = entropyCount(text1->second);
  double entropy2 = entropyCount(text2->second);
  std::string topWrd1 = topWord(text1->second);
  std::string topWrd2 = topWord(text2->second);
  double freq1 = text1->second.dictionary.find(topWrd1)->second;
  double freq2 = text2->second.dictionary.find(topWrd2)->second;
  size_t size1 = text1->second.size;
  size_t size2 = text2->second.size;
  long long diffSize = size1 - size2;
  if (diffSize < 0)
  {
    diffSize *= -1;
  }
  out << "\nTEXT COMPARISON: " << id1 << " vs " << id2 << "\n\n";
  out << "WORD COUNT\n";
  out << id1 << ": " << size1 << " words\n";
  out << id2 << ": " << size2 << " words\n";
  out << "Difference: " << diffSize << " words\n\n";
  out << "MOST FREQUENT WORDS\n";
  out << id1 << ": \"" << topWrd1 << "\" (" << freq1 << " occurrences)\n";
  out << id2 << ": \"" << topWrd2 << "\" (" << freq2 << " occurrences)\n\n";
  out << "FREQUENCY COMPARISON\n";
  size_t max_freq = std::max(freq1, freq2);
  size_t width = 40;
  out << id1 << ": " << '#' << std::string(width * freq1 / max_freq, '#');
  out << " " << freq1 << "\n";
  out << id2 << ": " << '#' << std::string(width * freq2 / max_freq, '#');
  out << " " << freq2 << "\n\n";
  out << "TEXT ENTROPY (bits/word)\n";
  out << id1 << ": " << std::fixed << std::setprecision(2) << entropy1 << "\n";
  out << id2 << ": " << entropy2 << "\n";
  double diff = entropy1 - entropy2;
  out << "Difference: " << std::abs(diff) << " (";
  if (std::abs(diff) < 0.5)
  {
    out << "nearly identical";
  }
  else if (diff > 0)
  {
    out << id1 << " is more diverse";
  }
  else
  {
    out << id2 << " is more diverse";
  }
  out << ")\n\n";
  out << "SUMMARY\n";
  if (size1 > size2)
  {
    out << "- " << id1 << " is longer\n";
  }
  else
  {
    out << "- " << id2 << " is longer\n";
  }
  if (freq1 > freq2)
  {
    out << "- " << id1 << " has more frequent top word\n";
  }
  else
  {
    out << "- " << id2 << " has more frequent top word\n";
  }
  if (entropy1 > entropy2)
  {
    out << "- " << id1 << " has higher entropy\n";
  }
  else
  {
    out << "- " << id2 << " has higher entropy\n";
  }
}

void shapkov::merge(std::istream& in, std::ostream& out, FrequencyDictionary& dict)
{
  std::string id1, id2, newDictId;
  in >> id1 >> id2 >> newDictId;
  if (dict.dicts.find(newDictId) != dict.dicts.end())
  {
    out << "<BUSY ID>\n";
    return;
  }
  auto text1 = dict.dicts.find(id1);
  auto text2 = dict.dicts.find(id2);
  if (text1 == dict.dicts.end() || text2 == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  OneFreqDict temp(text1->second);
  for (const auto& word_pair: text2->second.dictionary)
  {
    temp.dictionary[word_pair.first] += word_pair.second;
  }
  temp.size = text1->second.size + text2->second.size;
  dict.dicts.emplace(std::move(newDictId), std::move(temp));
}

void shapkov::diff(std::istream& in, std::ostream& out, FrequencyDictionary& dict)
{
  std::string id1, id2, newDictId;
  in >> id1 >> id2 >> newDictId;
  if (dict.dicts.find(newDictId) != dict.dicts.end())
  {
    out << "<BUSY ID>\n";
    return;
  }
  auto text1 = dict.dicts.find(id1);
  auto text2 = dict.dicts.find(id2);
  if (text1 == dict.dicts.end() || text2 == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  OneFreqDict temp;
  for (const auto& word_pair: text1->second.dictionary)
  {
    if (text2->second.dictionary.find(word_pair.first) == text2->second.dictionary.end())
    {
      temp.dictionary[word_pair.first] = word_pair.second;
      temp.size += word_pair.second;
    }
  }
  for (const auto& word_pair: text2->second.dictionary)
  {
    if (text1->second.dictionary.find(word_pair.first) == text1->second.dictionary.end())
    {
      temp.dictionary[word_pair.first] = word_pair.second;
      temp.size += word_pair.second;
    }
  }
  if (temp.dictionary.empty())
  {
    out << "<NO DIFFERENCES>\n";
    return;
  }
  dict.dicts.emplace(std::move(newDictId), std::move(temp));
}

void shapkov::intersect(std::istream& in, std::ostream& out, FrequencyDictionary& dict)
{
  std::string id1, id2, newDictId;
  in >> id1 >> id2 >> newDictId;
  if (dict.dicts.find(newDictId) != dict.dicts.end())
  {
    out << "<BUSY ID>\n";
    return;
  }
  auto text1 = dict.dicts.find(id1);
  auto text2 = dict.dicts.find(id2);
  if (text1 == dict.dicts.end() || text2 == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  OneFreqDict temp;
  std::unordered_map< std::string, size_t>* smallerDict = &text1->second.dictionary;
  std::unordered_map< std::string, size_t>* largerDict = &text2->second.dictionary;
  if (text2->second.dictionary.size() < text1->second.dictionary.size())
  {
    smallerDict = &text2->second.dictionary;
    largerDict = &text1->second.dictionary;
  }
  for (const auto& word_pair: *smallerDict)
  {
    auto word = largerDict->find(word_pair.first);
    if (word != largerDict->end())
    {
      temp.dictionary[word_pair.first] = word_pair.second + word->second;
      temp.size = temp.size + word_pair.second + word->second;
    }
  }
  if (temp.dictionary.empty())
  {
    out << "<NO INTERSECTIONS>\n";
    return;
  }
  dict.dicts.emplace(std::move(newDictId), std::move(temp));
}

void shapkov::save(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string fileName;
  in >> fileName;
  std::ofstream save(fileName);
  if (!save.is_open())
  {
    out << "<FAILED TO OPEN FILE>\n";
    return;
  }
  for (const auto& pair: dict.dicts)
  {
    save << "[" << pair.first << "]\n";
    save << "size=" << pair.second.size << '\n';
    for (const auto& word_pair: pair.second.dictionary)
    {
      save << word_pair.first << '\t' << word_pair.second << '\n';
    }
  }
  out << "<SAVED SUCCESSFULLY>\n";
}

void shapkov::load(std::ostream& out, const std::string fileName, FrequencyDictionary& dict)
{
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    out << "<FILE NOT FOUND>\n";
    return;
  }
  dict.dicts.clear();
  std::string line;
  std::string currentDictName;
  OneFreqDict* currentDict = nullptr;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      currentDict = nullptr;
    }
    else if (line[0] == '[')
    {
      currentDictName = line.substr(1, line.size() - 2);
      currentDict = &dict.dicts[currentDictName];
    }
    else if (line.find("size=") == 0)
    {
      currentDict->size = std::stoul(line.substr(5));
    }
    else
    {
      size_t tabPos = line.find('\t');
      std::string word = line.substr(0, tabPos);
      size_t count = std::stoul(line.substr(tabPos + 1));
      currentDict->dictionary[word] = count;
    }
  }
  out << "<LOADED SUCCESSFULLY>\n";
}
