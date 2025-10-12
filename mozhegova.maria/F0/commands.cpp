#include "commands.hpp"
#include <fstream>
#include <algorithm>
#include <string>
#include <functional>
#include <numeric>
#include <iomanip>

namespace
{
  using constWord  = const std::pair< std::string, mozhegova::Xrefs >;
  using Word  = std::pair< const std::string, mozhegova::Xrefs >;
  bool cmpMaxWordLen(constWord & a, constWord & b)
  {
    return a.first.size() < b.first.size();
  }

  struct PrintWordPos
  {
    std::ostream & out;
    void operator()(const std::pair< size_t, size_t > & pos) const
    {
      out << " (" << pos.first << ", " << pos.second << ')';
    }
  };

  struct PrintWords
  {
    std::ostream & out;
    size_t width;
    void operator()(constWord & word) const
    {
      out << std::left << std::setw(width) << word.first;
      std::for_each(word.second.cbegin(), word.second.cend(), PrintWordPos{out});
      out << '\n';
    }
  };

  std::pair< mozhegova::WordPos, std::string > createWordPair(const mozhegova::WordPos & pos, const std::string & word)
  {
    return std::make_pair(pos, word);
  }

  struct Accumulator
  {
    std::string result;
    std::pair< mozhegova::WordPos, std::string > prev;
    bool first = true;
    Accumulator(const std::string & initial):
      result(initial)
    {}
  };

  Accumulator accumulateWords(Accumulator acc, const std::pair< mozhegova::WordPos, std::string > & current)
  {
    if (acc.first)
    {
      acc.result += current.second;
      acc.first = false;
    }
    else
    {
      if (acc.prev.first.first != current.first.first)
      {
        acc.result += '\n';
      }
      else
      {
        acc.result += ' ';
      }
      acc.result += current.second;
    }
    acc.prev = current;
    return acc;
  }

  void supRecText(constWord & word, std::vector< std::pair< mozhegova::WordPos, std::string > > & sortedWords)
  {
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::transform(b, e, std::back_inserter(sortedWords), std::bind(createWordPair, _1, word.first));
  }

  std::string reconstructText(const mozhegova::Text & text)
  {
    std::vector< std::pair< mozhegova::WordPos, std::string > > sortedWords;
    using namespace std::placeholders;
    std::for_each(text.cbegin(), text.cend(), std::bind(supRecText, _1, std::ref(sortedWords)));
    std::sort(sortedWords.begin(), sortedWords.end());
    if (sortedWords.empty())
    {
      return "";
    }
    Accumulator initialValue("");
    Accumulator result = std::accumulate(sortedWords.begin(), sortedWords.end(), initialValue, accumulateWords);
    return result.result;
  }

  size_t getSizeWord(constWord & word)
  {
    return word.second.size();
  }

  struct AccumulateTextSize
  {
    size_t operator()(size_t currentSize, constWord & word) const
    {
      return currentSize + getSizeWord(word);
    }
  };

  struct PrintText
  {
    std::ostream & out;
    void operator()(const std::pair< std::string, mozhegova::Text > & text) const
    {
      out << text.first << ' ';
      out << std::accumulate(text.second.cbegin(), text.second.cend(), 0, AccumulateTextSize()) << '\n';
      out << reconstructText(text.second) << '\n';
    }
  };

  bool cmpMaxLineNumWord(const std::pair< size_t, size_t > & a, const std::pair< size_t, size_t > & b)
  {
    return a.first < b.first;
  }

  size_t getMaxLineNumWord(constWord & word)
  {
    mozhegova::Xrefs xrefs = word.second;
    if (xrefs.empty())
    {
      return 0;
    }
    auto maxNumIt = std::max_element(xrefs.cbegin(), xrefs.cend(), cmpMaxLineNumWord);
    return maxNumIt->first;
  }

  size_t getMaxLineNum(const mozhegova::Text & text)
  {
    std::vector< size_t > lineNum;
    std::transform(text.cbegin(), text.cend(), std::back_inserter(lineNum), getMaxLineNumWord);
    return *std::max_element(lineNum.cbegin(), lineNum.cend());
  }

  bool cmpMaxNumWord(const std::pair< size_t, size_t > & a, const std::pair< size_t, size_t > & b)
  {
    return a.second < b.second;
  }

  size_t getMaxNumWord(constWord & word)
  {
    mozhegova::Xrefs xrefs = word.second;
    if (xrefs.empty())
    {
      return 0;
    }
    auto maxNumIt = std::max_element(xrefs.cbegin(), xrefs.cend(), cmpMaxNumWord);
    return maxNumIt->second;
  }

  size_t getMaxNum(const mozhegova::Text & text)
  {
    std::vector< size_t > num;
    std::transform(text.cbegin(), text.cend(), std::back_inserter(num), getMaxNumWord);
    return *std::max_element(num.cbegin(), num.cend());
  }

  bool cmpBetween(const mozhegova::WordPos & pos, size_t begin, size_t end)
  {
    return pos.first >= begin && pos.first < end;
  }

  void subExtrSubstr(constWord & word, mozhegova::Text & result, size_t begin, size_t end)
  {
    mozhegova::Xrefs newXrefs;
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::copy_if(b, e, std::back_inserter(newXrefs), std::bind(cmpBetween, _1, begin, end));
    if (!newXrefs.empty())
    {
      result[word.first] = newXrefs;
    }
  }

  mozhegova::Text extractSubstring(const mozhegova::Text & text, size_t begin, size_t end)
  {
    mozhegova::Text result;
    using namespace std::placeholders;
    std::for_each(text.cbegin(), text.cend(), std::bind(subExtrSubstr, _1, std::ref(result), begin, end));
    return result;
  }

  mozhegova::WordPos downLenNum(const mozhegova::WordPos & pos, size_t begin, size_t end)
  {
    if (pos.first >= end)
    {
      return {pos.first - (end - begin), pos.second};
    }
    return pos;
  }

  mozhegova::WordPos uppLenNum(const mozhegova::WordPos & pos, size_t n, size_t add)
  {
    if (pos.first >= n)
    {
      return {pos.first + add, pos.second};
    }
    return pos;
  }

  mozhegova::WordPos changeLenNum(const mozhegova::WordPos & pos, size_t n, size_t begin)
  {
    return {pos.first + n - begin, pos.second};
  }

  mozhegova::WordPos reverseLenNum(const mozhegova::WordPos & pos, size_t maxLine)
  {
    return {maxLine - pos.first + 1, pos.second};
  }

  mozhegova::WordPos changeNum(const mozhegova::WordPos & pos, size_t line, size_t maxNum)
  {
    if (pos.first == line)
    {
      return {pos.first, maxNum - pos.second + 1};
    }
    return pos;
  }

  mozhegova::WordPos uppNum(const mozhegova::WordPos & pos, size_t n)
  {
    return {pos.first, pos.second + n};
  }

  void subRemoveSubstr(Word & word, size_t begin, size_t end)
  {
    using namespace std::placeholders;
    auto iter = std::remove_if(word.second.begin(), word.second.end(), std::bind(cmpBetween, _1, begin, end));
    word.second.erase(iter, word.second.end());
  }

  void subDownLenNum(Word & word, size_t begin, size_t end)
  {
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::transform(b, e, b, std::bind(downLenNum, _1, begin, end));
  }

  void removeSubstring(mozhegova::Text & text, size_t begin, size_t end)
  {
    using namespace std::placeholders;
    std::for_each(text.begin(), text.end(), std::bind(subRemoveSubstr, _1, begin, end));
    std::for_each(text.begin(), text.end(), std::bind(subDownLenNum, _1, begin, end));
  }

  void subUppLenNum(Word & word, size_t n, size_t begin, size_t end)
  {
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::transform(b, e, b, std::bind(uppLenNum, _1, n, end - begin));
  }

  void subChangeLenNum(Word & word, mozhegova::Text & text1, size_t n, size_t begin)
  {
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::transform(b, e, std::back_inserter(text1[word.first]), std::bind(changeLenNum, _1, n, begin));
  }

  void insertTextTo(mozhegova::Text & text1, const mozhegova::Text & text2, size_t n, size_t begin, size_t end)
  {
    mozhegova::Text temp = extractSubstring(text2, begin, end);
    using namespace std::placeholders;
    std::for_each(text1.begin(), text1.end(), std::bind(subUppLenNum, _1, n, begin, end));
    std::for_each(text1.begin(), text1.end(), std::bind(subChangeLenNum, _1, std::ref(text1), n, begin));
  }

  bool isWordOnLine(const mozhegova::WordPos & pos, size_t line)
  {
    return pos.first == line;
  }

  void subSideMerge(Word & word, size_t line, size_t num, mozhegova::Text & combinedText)
  {
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::transform(b, e, b, std::bind(uppNum, _1, num));
    std::copy_if(b, e, std::back_inserter(combinedText[word.first]), std::bind(isWordOnLine, _1, line));
  }

  void subInvertLines(Word & word, size_t lineNum)
  {
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::transform(b, e, b, std::bind(reverseLenNum, _1, lineNum));
  }

  void subInvertWords(Word & word, size_t line, size_t maxNum)
  {
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::transform(b, e, b, std::bind(changeNum, _1, line, maxNum));
  }
}

void mozhegova::generateLinks(std::istream & in, Texts & texts)
{
  std::string textName, fileName;
  in >> textName >> fileName;
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  if (texts.find(textName) != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text text{};
  size_t line = 0;
  while (!file.eof())
  {
    ++line;
    std::string word;
    size_t num = 0;
    while (file.peek() != '\n' && file >> word)
    {
      ++num;
      text[word].push_back({line, num});
    }
    file.ignore();
  }
  texts[textName] = std::move(text);
}

void mozhegova::removeLinks(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  if (texts.find(textName) == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  texts.erase(textName);
}

void mozhegova::printLinks(std::istream & in, std::ostream & out, const Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text = it->second;
  auto maxWordIt = std::max_element(text.cbegin(), text.cend(), cmpMaxWordLen);
  size_t maxWordLen = maxWordIt->first.length() + 2;
  std::for_each(text.cbegin(), text.cend(), PrintWords{out, maxWordLen});
}

void mozhegova::printText(std::istream & in, std::ostream & out, const Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text = it->second;
  std::string reconstructed = reconstructText(text);
  out << reconstructed << '\n';
}

void mozhegova::printTextInFile(std::istream & in, const Texts & texts)
{
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  printText(in, file, texts);
}

void mozhegova::mergeTexts(std::istream & in, Texts & texts)
{
  std::string newText, textName1, textName2;
  in >> newText >> textName1 >> textName2;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  auto it = texts.find(newText);
  if (it1 == texts.end() || it2 == texts.end() || it != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text1 = it1->second;
  const Text & text2 = it2->second;
  Text temp = text1;
  size_t num = getMaxLineNum(temp) + 1;
  insertTextTo(temp, text2, num, 1, getMaxLineNum(text2) + 1);
  texts[newText] = std::move(temp);
}

void mozhegova::insertText(std::istream & in, Texts & texts)
{
  std::string textName1, textName2;
  size_t num = 0, begin = 0, end = 0;
  in >> textName1 >> num >> textName2 >> begin >> end;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  if (it1 == texts.end() || it2 == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text1 = it1->second;
  const Text & text2 = it2->second;
  if ((num > 1 + getMaxLineNum(text1)) || (begin < 1) || (end > getMaxLineNum(text2) + 1))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  insertTextTo(text1, text2, num, begin, end);
}

void mozhegova::removeLines(std::istream & in, Texts & texts)
{
  std::string textName;
  size_t begin = 0, end = 0;
  in >> textName >> begin >> end;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  if ((begin < 1) || (end > getMaxLineNum(text) + 1))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  removeSubstring(text, begin, end);
}

void mozhegova::moveText(std::istream & in, Texts & texts)
{
  std::string textName1, textName2;
  size_t num = 0, begin = 0, end = 0;
  in >> textName1 >> num >> textName2 >> begin >> end;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  if (it1 == texts.end() || it2 == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text1 = it1->second;
  Text & text2 = it2->second;
  if ((num > 1 + getMaxLineNum(text1)) || (begin < 1) || (end > getMaxLineNum(text2) + 1))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  insertTextTo(text1, text2, num, begin, end);
  removeSubstring(text2, begin, end);
}

void mozhegova::sideMergeTexts(std::istream & in, Texts & texts)
{
  std::string newText, textName1, textName2;
  in >> newText >> textName1 >> textName2;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  auto it = texts.find(newText);
  if (it1 == texts.end() || it2 == texts.end() || it != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text1 = it1->second;
  const Text & text2 = it2->second;
  Text temp1 = text1;
  Text temp2 = text2;
  size_t maxLines = std::max(getMaxLineNum(text1), getMaxLineNum(text2));
  size_t maxNum = getMaxNum(text1);
  for (size_t line = 1; line <= maxLines; ++line)
  {
    using namespace std::placeholders;
    std::for_each(temp2.begin(), temp2.end(), std::bind(subSideMerge, _1, line, maxNum, std::ref(temp1)));
  }
  texts[newText] = std::move(temp1);
}

void mozhegova::splitTexts(std::istream & in, Texts & texts)
{
  std::string textName, newText1, newText2;
  size_t num = 0;
  in >> textName >> num >> newText1 >> newText2;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  size_t end = getMaxLineNum(text);
  if (num > end)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text newText = extractSubstring(text, num, end + 1);
  removeSubstring(text, num, end + 1);
  texts[newText1] = std::move(text);
  texts[newText2] = std::move(newText);
  texts.erase(textName);
}

void mozhegova::invertLines(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  size_t maxLine = getMaxLineNum(text);
  using namespace std::placeholders;
  std::for_each(text.begin(), text.end(), std::bind(subInvertLines, _1, maxLine));
}

void mozhegova::invertWords(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  size_t maxLines = getMaxLineNum(text);
  size_t maxNum = getMaxNum(text);
  for (size_t line = 1; line <= maxLines; ++line)
  {
    using namespace std::placeholders;
    std::for_each(text.begin(), text.end(), std::bind(subInvertWords, _1, line, maxNum));
  }
}

void mozhegova::replaceWord(std::istream & in, Texts & texts)
{
  std::string textName, oldWord, newWord;
  in >> textName >> oldWord >> newWord;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  auto wordIt = text.find(oldWord);
  if (wordIt == text.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  text[newWord] = std::move(wordIt->second);
  text.erase(wordIt);
}

void mozhegova::save(std::istream & in, const Texts & texts)
{
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  file << texts.size() << '\n';
  std::for_each(texts.cbegin(), texts.cend(), PrintText{file});
}

void mozhegova::loadFileCmd(std::istream & in, Texts & texts)
{
  std::string fileName;
  in >> fileName;
  loadFile(fileName, texts);
}

void mozhegova::loadFile(const std::string & fileName, Texts & texts)
{
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  size_t textsCount;
  file >> textsCount;
  for (size_t i = 0; i < textsCount; ++i)
  {
    std::string textName;
    size_t wordCount;
    file >> textName >> wordCount;
    Text & currText = texts[textName];
    std::string word;
    size_t line = 0;
    size_t j = 0;
    while (j < wordCount)
    {
      ++line;
      size_t num = 0;
      while (file.peek() != '\n' && file >> word)
      {
        ++num;
        currText[word].push_back({line, num});
        ++j;
      }
      file.ignore();
    }
  }
}

void mozhegova::printHelp(std::ostream & out)
{
  out << "Available commands:\n";
  out << "generatelinks <text name> <text file> - generate new links from file\n";
  out << "removelinks <text name> - delete text\n";
  out << "printlinks <text name> - print words from text with xref\n";
  out << "printtext <text name> - print text\n";
  out << "printtextinfile <text file> <text name> - print text in file\n";
  out << "merge <text result name> <text1 name> <text2 name> - merge two texts\n";
  out << "insert <text1 name> <num> <text2 name> <begin> <end> - insert part of the text into another\n";
  out << "removelines <text name> <begin> <end> - delete part of the text\n";
  out << "move <text1 name> <num> <text2 name> <begin> <end> - move part of the text into another\n";
  out << "sidemerge <text result name> <text1 name> <text2 name> - merge two texts along the lines\n";
  out << "split <text name> <num> <new text1 name> <new text2 name> - split the text into two texts\n";
  out << "invertlines <text name> - reverse the order of lines in the text\n";
  out << "invertwords <text name> - reverse the order of words in each line in the text\n";
  out << "replaceword <text name> <old word> <new word> - replace a word in the text with a new one \n";
  out << "save <file name> - save all texts\n";
  out << "loadfile <file name> - load the file\n";
  out << "--continue - load the saves\n";
  out << "--help - show this help\n";
}
