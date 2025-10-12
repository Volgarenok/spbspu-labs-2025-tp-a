#include "FrequencyDictionary.hpp"
#include <fstream>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>
#include <scopeGuard.hpp>
#include "analyzeAndParseFunctors.hpp"

void shapkov::analyzeText(std::istream& in, std::ostream& out, FrequencyDictionary& dict)
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

void shapkov::wordInfo(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string word, id;
  in >> word >> id;
  auto text = dict.dicts.find(id);
  if (text == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  cleanWord(word);
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
  cleanWord(word);
  isAnagram anagramChecker{ word };
  size_t anagramsCnt = 0;
  DictProcessor< isAnagram > dictProcessor{ out, anagramChecker, anagramsCnt };
  std::for_each(dict.dicts.begin(), dict.dicts.end(), dictProcessor);
  if (anagramsCnt == 0)
  {
    out << "<NO ANAGRAMS>\n";
  }
}

void shapkov::similarFrequency(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string word;
  int delta = 0;
  in >> word >> delta;
  if (!in && delta < 0)
  {
    throw std::invalid_argument("Wrong input");
  }
  cleanWord(word);
  size_t freqCnt = 0;
  FrequencyChecker freqChecker{ word, delta, out, 0 };
  DictProcessor< FrequencyChecker > dictProcessor{ out, freqChecker, freqCnt };
  std::for_each(dict.dicts.begin(), dict.dicts.end(), dictProcessor);
  if (freqCnt == 0)
  {
    out << "<NO WORD>\n";
  }
}

double shapkov::entropyCount(const OneFreqDict& text)
{
  EntropyCalc calc{ text.size };
  std::vector< double > wordProbs;
  std::transform(text.dictionary.begin(), text.dictionary.end(), std::back_inserter(wordProbs), calc);
  double entropy = std::accumulate(wordProbs.begin(), wordProbs.end(), 0.0);
  return (-1) * entropy;
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

void shapkov::totalWords(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
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
  isPalindrome PalindromeChecker;
  std::vector< std::string > txtWords;
  auto txt = text->second.dictionary;
  std::transform(txt.begin(), txt.end(), std::back_inserter(txtWords), ExtractFirst());
  std::copy_if(txtWords.begin(), txtWords.end(), std::ostream_iterator< std::string >{ out, "\n" }, std::ref(PalindromeChecker));
  if (PalindromeChecker.palindromesCnt == 0)
  {
    out << "<NO PALINDROMES>\n";
  }
}

void shapkov::showWithPattern(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
  std::string pattern, id;
  in >> pattern >> id;
  auto text = dict.dicts.find(id);
  if (text == dict.dicts.end())
  {
    out << "<TEXT NOT FOUND>\n";
    return;
  }
  isPattern PatternChecker{ pattern };
  std::vector< std::string > txtWords;
  auto txt = text->second.dictionary;
  std::transform(txt.begin(), txt.end(), std::back_inserter(txtWords), ExtractFirst());
  std::copy_if(txtWords.begin(), txtWords.end(), std::ostream_iterator< std::string >{ out, "\n" }, std::ref(PatternChecker));
  if (PatternChecker.patternMatches == 0)
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
  std::transform(text->second.dictionary.begin(), text->second.dictionary.end(), std::ostream_iterator< PairIO >{ out, "\n" }, makePairIO);
}

std::string shapkov::topWord(const OneFreqDict& text)
{
  auto topWord = std::max_element(text.dictionary.begin(), text.dictionary.end(), CompareByFreq);
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
  MergeFunctor merger{ temp.dictionary, temp.size };
  std::for_each(text2->second.dictionary.begin(), text2->second.dictionary.end(), merger);
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
  DiffFunctor d1{ text2->second.dictionary, temp.dictionary, temp.size };
  std::for_each(text1->second.dictionary.begin(), text1->second.dictionary.end(), d1);
  DiffFunctor d2{ text1->second.dictionary, temp.dictionary, temp.size };
  std::for_each(text2->second.dictionary.begin(), text2->second.dictionary.end(), d2);
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
  std::unordered_map< std::string, size_t >* smallerDict = &text1->second.dictionary;
  std::unordered_map< std::string, size_t >* largerDict = &text2->second.dictionary;
  if (text2->second.dictionary.size() < text1->second.dictionary.size())
  {
    smallerDict = &text2->second.dictionary;
    largerDict = &text1->second.dictionary;
  }
  IntersectFunctor isect{ *largerDict, temp.dictionary, temp.size };
  std::for_each(smallerDict->begin(), smallerDict->end(), isect);
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
    for (const auto& wordPair: pair.second.dictionary)
    {
      save << wordPair.first << '\t' << wordPair.second << '\n';
    }
  }
  out << "<SAVED SUCCESSFULLY>\n";
}

void shapkov::load(std::ostream& out, const std::string& fileName, FrequencyDictionary& dict)
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
