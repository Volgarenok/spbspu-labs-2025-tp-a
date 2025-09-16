#include "xref.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace amine
{
  bool positionLess::operator()(const Position& a, const Position& b) const
  {
    return (a.line < b.line) || (a.line == b.line && a.column < b.column);
  }

  std::ostream& operator<<(std::ostream& out, const Position& pos)
  {
    out << pos.line << ":" << pos.column;
    return out;
  }

  CrossRefSystem::CrossRefSystem()
  {
    indexes_ = std::map< std::string, Index >();
  }

  void readLines(std::ifstream& file, std::vector< std::string >& lines)
  {
    std::string line;
    if (!std::getline(file, line))
      return;
    lines.push_back(line);
    readLines(file, lines);
  }

  void splitWords(std::istringstream& stream, std::vector< std::string >& words)
  {
    std::string word;
    if (!(stream >> word))
      return;
    words.push_back(word);
    splitWords(stream, words);
  }

  void insertWordsRecursive(amine::Index& index, const std::vector< std::string >& words, size_t lineNum, size_t col)
  {
    if (col >= words.size())
      return;
    index[words[col]].insert(amine::Position{ lineNum, col });
    insertWordsRecursive(index, words, lineNum, col + 1);
  }

  void processLinesRecursive(amine::Index& index, const std::vector< std::string >& lines, size_t lineNum)
  {
    if (lineNum >= lines.size())
      return;

    std::istringstream stream(lines[lineNum]);
    std::vector< std::string > words;
    splitWords(stream, words);

    insertWordsRecursive(index, words, lineNum, 0);
    processLinesRecursive(index, lines, lineNum + 1);
  }

  void CrossRefSystem::buildIndex(const std::string& indexName, const std::string& fileName)
  {
    std::ifstream file(fileName);
    if (!file.is_open())
    {
      std::cout << "<FILE ERROR>\n";
      return;
    }

    if (indexes_.find(indexName) != indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    Index index;
    std::vector< std::string > lines;
    readLines(file, lines);
    processLinesRecursive(index, lines, 0);
    indexes_.insert({ indexName, index });
  }

  void CrossRefSystem::deleteIndex(const std::string& indexName)
  {
    auto it = indexes_.find(indexName);
    if (it == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }
    indexes_.erase(it);
  }

  void CrossRefSystem::containsWord(const std::string& indexName, const std::string& word)
  {
    auto indexIt = indexes_.find(indexName);
    if (indexIt == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    const Index& index = indexIt->second;
    auto wordIt = index.find(word);
    if (wordIt != index.end())
    {
      std::cout << "<YES>\n";
    }
    else
    {
      std::cout << "<NO>\n";
    }
  }

  void printPositionsRecursive(std::set< amine::Position >::const_iterator it,
                               std::set< amine::Position >::const_iterator end)
  {
    if (it == end)
      return;
    std::cout << " " << it->line << ":" << it->column;
    printPositionsRecursive(std::next(it), end);
  }

  void printIndexRecursive(amine::Index::const_iterator it, amine::Index::const_iterator end)
  {
    if (it == end)
      return;
    std::cout << it->first << ":";
    printPositionsRecursive(it->second.begin(), it->second.end());
    std::cout << "\n";
    printIndexRecursive(std::next(it), end);
  }

  void CrossRefSystem::printIndex(const std::string& indexName)
  {
    auto it = indexes_.find(indexName);
    if (it == indexes_.end() || it->second.empty())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    const Index& index = it->second;
    printIndexRecursive(index.begin(), index.end());
  }

  void CrossRefSystem::getPositions(const std::string& indexName, const std::string& word)
  {
    auto it = indexes_.find(indexName);
    if (it == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    const Index& index = it->second;
    auto wordIt = index.find(word);
    if (wordIt == index.end())
    {
      std::cout << "<NOT FOUND>\n";
      return;
    }

    const std::set< Position, positionLess >& positions = wordIt->second;

    std::function< void(std::set< Position >::const_iterator) > printRecursive;
    printRecursive = [&](std::set< Position >::const_iterator posIt) {
      if (posIt == positions.end())
        return;
      std::cout << posIt->line << ":" << posIt->column << "\n";
      printRecursive(std::next(posIt));
    };

    printRecursive(positions.begin());
  }

  void CrossRefSystem::mergeTexts(const std::string& newIndex, const std::string& index1, const std::string& index2)
  {
    auto it1 = indexes_.find(index1);
    auto it2 = indexes_.find(index2);
    if (it1 == indexes_.end() || it2 == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    const Index& first = it1->second;
    const Index& second = it2->second;
    Index result;

    copyIndexWithOffset(result, first, 0);

    size_t lastLine = getMaxLineRecursive(first.begin(), first.end(), 0);
    size_t offset = lastLine + 1;

    copyIndexWithOffset(result, second, offset);
    indexes_[newIndex] = result;
  }
  void CrossRefSystem::insertText(const std::string& newIndex, const std::string& baseIndex,
                                  const std::string& insertIndex, size_t afterLine, size_t afterColumn)
  {
    auto baseIt = indexes_.find(baseIndex);
    auto insertIt = indexes_.find(insertIndex);

    if (baseIt == indexes_.end() || insertIt == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }
    const Index& base = baseIt->second;
    const Index& toInsert = insertIt->second;
    Index result;

    std::function< bool(Index::const_iterator) > validatePosRecursive;
    validatePosRecursive = [&](Index::const_iterator it) -> bool {
      if (it == base.end())
        return false;
      std::function< bool(std::set< Position >::const_iterator) > checkPositions;
      checkPositions = [&](std::set< Position >::const_iterator posIt) -> bool {
        if (posIt == it->second.end())
          return false;
        if (posIt->line == afterLine && posIt->column == afterColumn)
          return true;
        return checkPositions(std::next(posIt));
      };
      return checkPositions(it->second.begin()) || validatePosRecursive(std::next(it));
    };

    if (!validatePosRecursive(base.begin()))
    {
      std::cout << "<INVALID POSITION>\n";
      return;
    }

    Index before, after;
    std::function< void(Index::const_iterator) > splitRecursive;
    splitRecursive = [&](Index::const_iterator it) {
      if (it == base.end())
        return;
      const std::string& word = it->first;
      std::function< void(std::set< Position >::const_iterator) > processPositions;
      processPositions = [&](std::set< Position >::const_iterator posIt) {
        if (posIt == it->second.end())
          return;
        if (posIt->line < afterLine || (posIt->line == afterLine && posIt->column <= afterColumn))
          before[word].insert(*posIt);
        else
          after[word].insert(*posIt);
        processPositions(std::next(posIt));
      };

      processPositions(it->second.begin());
      splitRecursive(std::next(it));
    };

    splitRecursive(base.begin());

    copyIndexWithOffset(result, before, 0);

    size_t insertOffset = getMaxLineRecursive(before.begin(), before.end(), 0) + 1;
    copyIndexWithOffset(result, toInsert, insertOffset);

    size_t finalOffset = insertOffset + getMaxLineRecursive(toInsert.begin(), toInsert.end(), 0) + 1;
    copyIndexWithOffset(result, after, finalOffset - insertOffset);

    indexes_[newIndex] = result;
  }

  void CrossRefSystem::extractText(const std::string& newIndex, const std::string& baseIndex, size_t startLine,
                                   size_t startCol, size_t endLine, size_t endCol)
  {
    auto baseIt = indexes_.find(baseIndex);
    if (baseIt == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }
    const Index& base = baseIt->second;
    Index result;
    if (startLine > endLine || (startLine == endLine && startCol > endCol))
    {
      std::cout << "<INVALID RANGE>\n";
      return;
    }
    std::function< void(Index::const_iterator) > extractRecursive;
    extractRecursive = [&](Index::const_iterator it) {
      if (it == base.end())
        return;

      const std::string& word = it->first;
      const auto& positions = it->second;

      std::function< void(std::set< Position >::const_iterator) > filterPositions;
      filterPositions = [&](std::set< Position >::const_iterator pit) {
        if (pit == positions.end())
          return;

        const auto& pos = *pit;
        bool insideRange = (pos.line > startLine || (pos.line == startLine && pos.column >= startCol)) &&
                           (pos.line < endLine || (pos.line == endLine && pos.column <= endCol));

        if (insideRange)
          result[word].insert(pos);

        filterPositions(std::next(pit));
      };

      filterPositions(positions.begin());
      extractRecursive(std::next(it));
    };

    extractRecursive(base.begin());
    indexes_[newIndex] = result;
  }

  void copyIndexWithOffset(amine::Index& target, const amine::Index& source, size_t lineOffset)
  {
    if (source.empty())
      return;

    auto it = source.begin();

    std::function< void(Index::const_iterator) > copyRecursive;
    copyRecursive = [&](Index::const_iterator iter) {
      if (iter == source.end())
        return;
      const std::string& word = iter->first;
      const std::set< amine::Position, amine::positionLess >& positions = iter->second;

      std::function< void(std::set< amine::Position >::const_iterator) > insertPositions;
      insertPositions = [&](std::set< amine::Position >::const_iterator posIt) {
        if (posIt == positions.end())
          return;

        amine::Position shifted{ posIt->line + lineOffset, posIt->column };
        target[word].insert(shifted);
        insertPositions(std::next(posIt));
      };

      insertPositions(positions.begin());
      copyRecursive(std::next(iter));
    };

    copyRecursive(it);
  }
  size_t getMaxLineRecursive(Index::const_iterator iter, Index::const_iterator end, size_t currentMax)
  {
    if (iter == end)
    {
      return currentMax;
    }
    size_t maxInWord = currentMax;
    std::function< size_t(std::set< Position >::const_iterator, std::set< Position >::const_iterator, size_t) >
      maxInSet;

    maxInSet = [&](std::set< Position >::const_iterator pit, std::set< Position >::const_iterator pend, size_t curMax) {
      if (pit == pend)
        return curMax;
      size_t nextMax = std::max(curMax, pit->line);
      return maxInSet(std::next(pit), pend, nextMax);
    };

    maxInWord = maxInSet(iter->second.begin(), iter->second.end(), maxInWord);
    return getMaxLineRecursive(std::next(iter), end, maxInWord);
  }
  void CrossRefSystem::replaceWord(const std::string& indexName, const std::string& oldWord, const std::string& newWord)
  {
    auto it = indexes_.find(indexName);
    if (it == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    Index& index = it->second;
    auto wordIt = index.find(oldWord);
    if (wordIt == index.end())
    {
      std::cout << "<NOT FOUND>\n";
      return;
    }

    index[newWord] = wordIt->second;
    index.erase(wordIt);
  }
  void CrossRefSystem::repeatText(const std::string& newIndex, const std::string& baseIndex, size_t N)
  {
    if (N == 0)
    {
      std::cout << "<INVALID N>\n";
      return;
    }

    auto it = indexes_.find(baseIndex);
    if (it == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    const Index& base = it->second;

    Index result;
    copyIndexWithOffset(result, base, 0);

    size_t lastLine = getMaxLineRecursive(base.begin(), base.end(), 0) + 1;

    std::function< void(size_t) > repeatRecursive;
    repeatRecursive = [&](size_t count) {
      if (count == 1)
        return;

      copyIndexWithOffset(result, base, lastLine * (count - 1));
      repeatRecursive(count - 1);
    };

    repeatRecursive(N);
    indexes_[newIndex] = result;
  }
  void CrossRefSystem::swapWords(const std::string& indexName, const std::string& word1, const std::string& word2)
  {
    auto it = indexes_.find(indexName);
    if (it == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    Index& index = it->second;
    auto word1It = index.find(word1);
    auto word2It = index.find(word2);

    if (word1It == index.end() || word2It == index.end())
    {
      std::cout << "<NOT FOUND>\n";
      return;
    }

    std::set< Position, positionLess > temp = word1It->second;
    index[word1] = word2It->second;
    index[word2] = temp;
  }
  void CrossRefSystem::interleaveLines(const std::string& newIndex, const std::string& index1,
                                       const std::string& index2)
  {
    auto it1 = indexes_.find(index1);
    auto it2 = indexes_.find(index2);
    if (it1 == indexes_.end() || it2 == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    const Index& a = it1->second;
    const Index& b = it2->second;
    Index result;

    std::map< size_t, std::vector< std::pair< std::string, size_t > > > linesA;
    std::map< size_t, std::vector< std::pair< std::string, size_t > > > linesB;
    std::function< void(Index::const_iterator) > collectLinesARecursive;
    collectLinesARecursive = [&](Index::const_iterator it) {
      if (it == a.end())
        return;
      const std::string& word = it->first;
      std::function< void(std::set< Position >::const_iterator) > processPositionsA;
      processPositionsA = [&](std::set< Position >::const_iterator posIt) {
        if (posIt == it->second.end())
          return;
        linesA[posIt->line].emplace_back(word, posIt->column);
        processPositionsA(std::next(posIt));
      };
      processPositionsA(it->second.begin());
      collectLinesARecursive(std::next(it));
    };

    std::function< void(Index::const_iterator) > collectLinesBRecursive;
    collectLinesBRecursive = [&](Index::const_iterator it) {
      if (it == b.end())
        return;
      const std::string& word = it->first;
      std::function< void(std::set< Position >::const_iterator) > processPositionsB;
      processPositionsB = [&](std::set< Position >::const_iterator posIt) {
        if (posIt == it->second.end())
          return;
        linesB[posIt->line].emplace_back(word, posIt->column);
        processPositionsB(std::next(posIt));
      };
      processPositionsB(it->second.begin());
      collectLinesBRecursive(std::next(it));
    };

    collectLinesARecursive(a.begin());
    collectLinesBRecursive(b.begin());

    size_t maxLines = std::max(linesA.size(), linesB.size());
    size_t outLine = 0;

    std::function< void(size_t) > writeInterleavedRecursive;
    writeInterleavedRecursive = [&](size_t lineIdx) {
      if (lineIdx >= maxLines)
        return;

      auto insertLineRecursive = [&](const auto& src, size_t srcLine) {
        auto found = src.find(srcLine);
        if (found != src.end())
        {
          const auto& wordCols = found->second;
          std::function< void(size_t) > processWordCols;
          processWordCols = [&](size_t idx) {
            if (idx >= wordCols.size())
              return;
            const auto& wordCol = wordCols[idx];
            result[wordCol.first].insert({ outLine, wordCol.second });
            processWordCols(idx + 1);
          };
          processWordCols(0);
        }
      };

      insertLineRecursive(linesA, lineIdx);
      ++outLine;
      insertLineRecursive(linesB, lineIdx);
      ++outLine;

      writeInterleavedRecursive(lineIdx + 1);
    };

    writeInterleavedRecursive(0);
    indexes_[newIndex] = result;
  }

  void CrossRefSystem::reverseText(const std::string& newIndex, const std::string& baseIndex)
  {
    auto it = indexes_.find(baseIndex);
    if (it == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    const Index& base = it->second;
    if (base.empty())
    {
      std::cout << "<EMPTY>\n";
      return;
    }

    size_t maxLine = getMaxLineRecursive(base.begin(), base.end(), 0);
    Index result;

    std::function< void(Index::const_iterator) > reverseRecursive;
    reverseRecursive = [&](Index::const_iterator wordIt) {
      if (wordIt == base.end())
        return;

      const std::string& word = wordIt->first;
      const auto& positions = wordIt->second;

      std::function< void(std::set< Position >::const_iterator) > processSet;
      processSet = [&](std::set< Position >::const_iterator posIt) {
        if (posIt == positions.end())
          return;

        size_t newLine = maxLine - posIt->line;
        result[word].insert(Position{ newLine, posIt->column });

        processSet(std::next(posIt));
      };

      processSet(positions.begin());
      reverseRecursive(std::next(wordIt));
    };

    reverseRecursive(base.begin());
    indexes_[newIndex] = result;
  }

  void CrossRefSystem::saveIndex(const std::string& indexName, const std::string& filename)
  {
    auto it = indexes_.find(indexName);
    if (it == indexes_.end())
      return;

    std::ofstream out(filename);
    if (!out.is_open())
    {
      std::cout << "<FILE ERROR>\n";
      return;
    }

    const Index& index = it->second;

    std::function< void(Index::const_iterator) > writeWords;
    writeWords = [&](Index::const_iterator w) {
      if (w == index.end())
        return;
      out << w->first;

      std::function< void(std::set< Position >::const_iterator) > writePos;
      writePos = [&](std::set< Position >::const_iterator p) {
        if (p == w->second.end())
          return;
        out << " " << p->line << ":" << p->column;
        writePos(std::next(p));
      };

      writePos(w->second.begin());
      out << "\n";

      writeWords(std::next(w));
    };

    writeWords(index.begin());
  }

  void loadRecursive(std::ifstream& in, amine::Index& index, std::string& word)
  {
    size_t line = 0;
    size_t col = 0;

    if (!(in >> word))
      return;
    std::function< void() > skipSpaces;
    skipSpaces = [&]() {
      if (in.peek() != ' ')
        return;
      in.get();
      skipSpaces();
    };
    skipSpaces();

    std::string position;
    if (!(in >> position))
      return;

    size_t colonPos = position.find(':');
    if (colonPos == std::string::npos)
      return;

    line = std::stoul(position.substr(0, colonPos));
    col = std::stoul(position.substr(colonPos + 1));

    index[word].insert({ line, col });

    char next = in.peek();
    if (next == '\n' || next == EOF)
    {
      std::string nextWord;
      loadRecursive(in, index, nextWord);
    }
    else
    {
      loadRecursive(in, index, word);
    }
  }

  void CrossRefSystem::loadIndex(const std::string& indexName, const std::string& fileName)
  {
    std::ifstream in(fileName);
    if (!in.is_open())
    {
      std::cout << "<FILE ERROR>\n";
      return;
    }

    Index index;
    std::string word;
    loadRecursive(in, index, word);
    indexes_[indexName] = index;
  }

  void CrossRefSystem::reconstructText(const std::string& indexName, const std::string& filename)
  {
    auto it = indexes_.find(indexName);
    if (it == indexes_.end())
    {
      std::cout << "<WRONG INDEX>\n";
      return;
    }

    const Index& index = it->second;
    if (index.empty())
    {
      std::cout << "<EMPTY>\n";
      return;
    }

    size_t maxLine = 0;
    size_t maxCol = 0;

    std::function< void(Index::const_iterator) > calcMax;
    calcMax = [&](Index::const_iterator wordIt) {
      if (wordIt == index.end())
        return;

      std::function< void(std::set< Position >::const_iterator) > checkPositions;
      checkPositions = [&](std::set< Position >::const_iterator pit) {
        if (pit == wordIt->second.end())
          return;
        if (pit->line > maxLine)
          maxLine = pit->line;
        if (pit->column > maxCol)
          maxCol = std::max(maxCol, wordIt->first.length() + pit->column);
        checkPositions(std::next(pit));
      };

      checkPositions(wordIt->second.begin());
      calcMax(std::next(wordIt));
    };

    calcMax(index.begin());

    std::vector< std::string > lines(maxLine + 1, std::string(maxCol + 10, ' '));

    std::function< void(Index::const_iterator) > insertWords;
    insertWords = [&](Index::const_iterator wordIt) {
      if (wordIt == index.end())
        return;

      const std::string& word = wordIt->first;

      std::function< void(std::set< Position >::const_iterator) > insertPos;
      insertPos = [&](std::set< Position >::const_iterator pit) {
        if (pit == wordIt->second.end())
          return;

        const Position& pos = *pit;
        std::string& line = lines[pos.line];

        std::function< void(size_t) > insertChars;
        insertChars = [&](size_t i) {
          if (i >= word.length())
            return;
          if (pos.column + i >= line.length())
            return;
          line[pos.column + i] = word[i];
          insertChars(i + 1);
        };

        insertChars(0);
        insertPos(std::next(pit));
      };

      insertPos(wordIt->second.begin());
      insertWords(std::next(wordIt));
    };

    insertWords(index.begin());

    std::ofstream out(filename);
    if (!out.is_open())
    {
      std::cout << "<FILE ERROR>\n";
      return;
    }

    std::function< void(size_t) > writeLines;
    writeLines = [&](size_t i) {
      if (i >= lines.size())
        return;
      out << lines[i] << "\n";
      writeLines(i + 1);
    };

    writeLines(0);
  }
}
