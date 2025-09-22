#include "splitFunctors.hpp"
#include <algorithm>
#include <iterator>

std::string krylov::LineSplitter::operator()(const std::string& word) const
{
  wordMap[word].insert(lineNumber);
  return word;
}

std::size_t krylov::countWords(const std::string& line)
{
  return std::count_if(line.begin(), line.end(), WordCounter{});
}

std::size_t krylov::SplitWords::operator()(std::size_t lineNumber) const
{
  const std::string& line = lines.at(lineNumber);

  std::vector< std::string > words;
  std::size_t wordCount = countWords(line);
  WordGenerator gen{ line.begin(), line.end() };
  std::generate_n(std::back_inserter(words),wordCount, gen);

  std::transform(words.begin(), words.end(), words.begin(), LineSplitter{ wordMap, lineNumber + 1 });

  return lineNumber;
}

bool krylov::isSpace(unsigned char ch)
{
  return std::isspace(ch);
}

bool krylov::notSpace(unsigned char ch)
{
  return !std::isspace(ch);
}

krylov::WordCounter::WordCounter():
  prevSpase(true)
{}

bool krylov::WordCounter::operator()(unsigned char ch)
{
  bool currestIsWord = !std::isspace(ch);
  bool start = currestIsWord && prevSpase;
  prevSpase = std::isspace(ch);
  return start;
}

std::string krylov::WordGenerator::operator()()
{
  cur = std::find_if(cur, end, notSpace);
  if (cur == end)
  {
    return {};
  }
  auto wordEnd = std::find_if(cur, end, isSpace);
  std::string word(cur, wordEnd);
  cur = wordEnd;
  return word;
}

std::size_t krylov::SplitAndAdd::operator()(std::size_t lineNum) const
{
  const std::string& line = lines[lineNum - 1];

  std::vector< std::string > words;
  std::size_t wordCount = countWords(line);
  WordGenerator gen{ line.begin(), line.end() };
  std::generate_n(std::back_inserter(words), wordCount, gen);

  std::transform(words.begin(), words.end(), words.begin(), LineSplitter{ idx.index, lineNum });
  return lineNum;
}
