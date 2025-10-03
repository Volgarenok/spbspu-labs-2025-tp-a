#include "commands.hpp"
#include "wordFunctors.hpp"
#include "outputFunctors.hpp"
#include "splitFunctors.hpp"
#include "index.hpp"
#include <fstream>
#include <iostream>
#include <numeric>
#include <iterator>
#include <functional>
#include <algorithm>


krylov::CommandProcessor::CommandProcessor(std::ostream& output):
  out_(output)
{
  initializeCommands();
}

void krylov::CommandProcessor::initializeCommands()
{
  using namespace std::placeholders;
  commands["create"] = std::bind(&CommandProcessor::createIndexCmd, this, _1);
  commands["printindex"] = std::bind(&CommandProcessor::printIndexCmd, this, _1);
  commands["find"] = std::bind(&CommandProcessor::findWordCmd, this, _1);
  commands["merge"] = std::bind(&CommandProcessor::mergeIndexesCmd, this, _1);
  commands["zip"] = std::bind(&CommandProcessor::zipTextsCmd, this, _1);
  commands["list"] = std::bind(&CommandProcessor::listIndexesCmd, this, _1);
  commands["deleteindex"] = std::bind(&CommandProcessor::deleteIndexCmd, this, _1);
  commands["weave"] = std::bind(&CommandProcessor::deleteIndexCmd, this, _1);
  commands["intersect"] = std::bind(&CommandProcessor::deleteIndexCmd, this, _1);
  commands["diff"] = std::bind(&CommandProcessor::deleteIndexCmd, this, _1);
}

void krylov::CommandProcessor::execute(const std::string& line)
{
  std::vector< std::string > args;
  size_t wordCount = countWords(line);
  WordGenerator gen{ line.begin(), line.end() };
  std::generate_n(std::back_inserter(args), wordCount, gen);

  if (args.empty())
  {
    throw std::invalid_argument("Invalid command");
  }

  const std::string& cmd = args[0];
  auto commandIt = commands.find(cmd);
  if (commandIt == commands.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  std::vector< std::string > commandArgs(args.begin() + 1, args.end());
  commandIt->second(commandArgs);
}

void krylov::CommandProcessor::createIndex(const std::string& indexName, const std::string& filename)
{
  if (indexes_.count(indexName))
  {
    throw std::invalid_argument("Invalid command");
  }

  std::ifstream file(filename);
  if (!file)
  {
    throw std::invalid_argument("Invalid command");
  }

  IndexDocument newIndex;
  newIndex.sourceFile = filename;

  std::string line;
  while (std::getline(file, line))
  {
    newIndex.lines.push_back(line);
  }

  if (newIndex.lines.empty())
  {
    throw std::invalid_argument("Invalid command");
  }

  std::vector< size_t > lineNumbers(newIndex.lines.size());
  std::iota(lineNumbers.begin(), lineNumbers.end(), 0);

  SplitWords processor{ newIndex.lines, newIndex.index };
  std::transform(lineNumbers.begin(), lineNumbers.end(), lineNumbers.begin(), processor);

  indexes_[indexName] = std::move(newIndex);
  out_ << "Index " << indexName << " created successfully" << '\n';
}

void krylov::CommandProcessor::findWord(const std::string& indexName, const std::string& word)
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  FindWord finder(indexes_.at(indexName), word, indexName, out_);
  finder();
}

void krylov::CommandProcessor::printIndex(const std::string& indexName)
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  const IndexDocument& index = it->second;

  std::transform(index.index.begin(), index.index.end(), std::ostream_iterator< WordEntry >(out_, "\n"), pairToWordEntry);
}

void krylov::CommandProcessor::deleteIndex(const std::string& indexName)
{
  auto it = indexes_.find(indexName);

  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  indexes_.erase(it);
  out_ << "Index " << indexName << " cleared" << '\n';
}

void krylov::CommandProcessor::listIndexes()
{
  std::vector< std::string > lines;
  lines.reserve(indexes_.size());

  std::transform(indexes_.begin(), indexes_.end(), std::back_inserter(lines), formatIndexEntry);
  std::copy(lines.begin(), lines.end(), std::ostream_iterator< std::string >(out_, "\n"));
}

void krylov::CommandProcessor::mergeIndexes(const std::string& index1, const std::string& index2, const std::string& newIndex)
{
  auto it1 = indexes_.find(index1);
  auto it2 = indexes_.find(index2);

  if (it1 == indexes_.end() || it2 == indexes_.end() || indexes_.count(newIndex))
  {
    throw std::invalid_argument("Invalid command");
  }

  IndexDocument result;

  std::copy(it1->second.index.begin(), it1->second.index.end(), std::inserter(result.index, result.index.end()));

  size_t offset = it1->second.lines.size();
  std::for_each(it2->second.index.begin(), it2->second.index.end(), MergeWithOffset{ result, offset });

  std::vector< std::string >& lines1 = it1->second.lines;
  std::vector< std::string >& lines2 = it2->second.lines;

  result.lines.reserve(lines1.size() + lines2.size());
  std::copy(lines1.begin(), lines1.end(), std::back_inserter(result.lines));
  std::copy(lines2.begin(), lines2.end(), std::back_inserter(result.lines));

  indexes_[newIndex] = std::move(result);
  out_ << "Index " << newIndex << " created by merging" << '\n';
}

void krylov::CommandProcessor::zipTexts(const std::string& index1, const std::string& index2, const std::string& newIndex)
{
  if (!indexes_.count(index1) || !indexes_.count(index2) || indexes_.count(newIndex))
  {
    throw std::invalid_argument("Invalid command");
  }

  const auto& lines1 = indexes_.at(index1).lines;
  const auto& lines2 = indexes_.at(index2).lines;
  size_t maxSize = std::max(lines1.size(), lines2.size());

  std::vector< std::string > mergedLines(maxSize);
  std::generate(mergedLines.begin(), mergedLines.end(), MergeLinesFunctor{ lines1, lines2, 0 });

  IndexDocument resultIndex;
  resultIndex.lines = mergedLines;

  std::vector< size_t > lineNumbers(maxSize);
  std::iota(lineNumbers.begin(), lineNumbers.end(), 1);

  std::transform(lineNumbers.begin(), lineNumbers.end(), lineNumbers.begin(), SplitAndAdd{ resultIndex, resultIndex.lines });

  indexes_[newIndex] = std::move(resultIndex);
  out_ << "Index " << newIndex << " created by merging lines" << '\n';
}

void krylov::CommandProcessor::intersectIndexes(const std::string& index1, const std::string& index2, const std::string& newIndex)
{
  auto it1 = indexes_.find(index1);
  auto it2 = indexes_.find(index2);
  if (it1 == indexes_.end() || it2 == indexes_.end() || indexes_.count(newIndex))
  {
    throw std::invalid_argument("Invalid command");
  }
  IndexDocument result;
  const auto& idx1 = it1->second.index;
  const auto& idx2 = it2->second.index;
  std::vector<IndexDictionary::value_type> tempResult;
  std::copy_if(idx1.begin(), idx1.end(), std::back_inserter(tempResult), IsInSecondIndex{ idx2 });
  std::transform(tempResult.begin(), tempResult.end(), std::inserter(result.index, result.index.begin()), MergeEntries{idx2});
  indexes_[newIndex] = std::move(result);
  out_ << "Index " << newIndex << " created by intersection" << '\n';
}

void krylov::CommandProcessor::weaveTexts(const std::string& index1, const std::string& index2, const std::string& newIndex)
{
  if (!indexes_.count(index1) || !indexes_.count(index2) || indexes_.count(newIndex))
  {
    throw std::invalid_argument("Invalid command");
  }
  const auto& lines1 = indexes_.at(index1).lines;
  const auto& lines2 = indexes_.at(index2).lines;
  std::vector< std::string > newText;
  size_t totalSize = lines1.size() + lines2.size();
  newText.reserve(totalSize);
  WeaveLinesGenerator generator(lines1, lines2);
  std::generate_n(std::back_inserter(newText), totalSize, generator);
  IndexDocument resultIndex;
  resultIndex.lines = newText;
  std::vector< size_t > lineNumbers(newText.size());
  std::iota(lineNumbers.begin(), lineNumbers.end(), 1);
  std::transform(lineNumbers.begin(), lineNumbers.end(), lineNumbers.begin(), SplitAndAdd{ resultIndex, resultIndex.lines });
  indexes_[newIndex] = std::move(resultIndex);
  out_ << "Index " << newIndex << " created by weaving" << '\n';
}

void krylov::CommandProcessor::diffIndexes(const std::string& index1, const std::string& index2, const std::string& newIndex)
{
  auto it1 = indexes_.find(index1);
  auto it2 = indexes_.find(index2);
  if (it1 == indexes_.end() || it2 == indexes_.end() || indexes_.count(newIndex))
  {
    throw std::invalid_argument("Invalid command");
  }
  IndexDocument result;
  const auto& idx1 = it1->second.index;
  const auto& idx2 = it2->second.index;
  std::copy_if(idx1.begin(), idx1.end(), std::inserter(result.index, result.index.begin()), IsNotInSecondIndex{ idx2 });
  indexes_[newIndex] = std::move(result);
  out_ << "Index " << newIndex << " created by difference" << '\n';
}

void krylov::CommandProcessor::intersectIndexesCmd(const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid command");
  }
  intersectIndexes(args[0], args[1], args[2]);
}

void krylov::CommandProcessor::weaveTextsCmd(const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid command");
  }
  weaveTexts(args[0], args[1], args[2]);
}

void krylov::CommandProcessor::diffIndexesCmd(const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid command");
  }
  diffIndexes(args[0], args[1], args[2]);
}

void krylov::CommandProcessor::createIndexCmd(const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid command");
  }
  createIndex(args[0], args[1]);
}

void krylov::CommandProcessor::printIndexCmd(const std::vector< std::string >& args)
{
  if (args.size() != 1)
  {
    throw std::invalid_argument("Invalid command");
  }
  printIndex(args[0]);
}

void krylov::CommandProcessor::findWordCmd(const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid command");
  }
  findWord(args[0], args[1]);
}


void krylov::CommandProcessor::mergeIndexesCmd(const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid command");
  }
  mergeIndexes(args[0], args[1], args[2]);
}

void krylov::CommandProcessor::zipTextsCmd(const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid command");
  }
  zipTexts(args[0], args[1], args[2]);
}

void krylov::CommandProcessor::listIndexesCmd(const std::vector< std::string >& args)
{
  if (!args.empty())
  {
    throw std::invalid_argument("Invalid command");
  }
  listIndexes();
}


void krylov::CommandProcessor::deleteIndexCmd(const std::vector< std::string >& args)
{
  if (args.size() != 1)
  {
    throw std::invalid_argument("Invalid command");
  }
  deleteIndex(args[0]);
}
