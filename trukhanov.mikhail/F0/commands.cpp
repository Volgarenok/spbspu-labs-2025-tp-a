#include "commands.hpp"
#include "output_functors.hpp"
#include "split_functors.hpp"
#include "filter_functors.hpp"
#include "word_functors.hpp"
#include "index.hpp"
#include <fstream>
#include <iostream>
#include <numeric>
#include <iterator>
#include <functional>
#include <algorithm>

trukhanov::CommandProcessor::CommandProcessor(std::ostream& output):
  out_(output)
{ initializeCommands(); }

void trukhanov::CommandProcessor::initializeCommands()
{
  using namespace std::placeholders;

  commands["createindex"] = std::bind(&CommandProcessor::createIndexCmd, this, _1);
  commands["showindex"] = std::bind(&CommandProcessor::showIndexCmd, this, _1);
  commands["searchword"] = std::bind(&CommandProcessor::searchWordCmd, this, _1);
  commands["saveindex"] = std::bind(&CommandProcessor::saveIndexCmd, this, _1);
  commands["replaceword"] = std::bind(&CommandProcessor::replaceWordCmd, this, _1);
  commands["exportword"] = std::bind(&CommandProcessor::exportWordCmd, this, _1);
  commands["mergeindexes"] = std::bind(&CommandProcessor::mergeIndexesCmd, this, _1);
  commands["mergebylines"] = std::bind(&CommandProcessor::mergeByLinesCmd, this, _1);
  commands["showfrequency"] = std::bind(&CommandProcessor::showFrequencyCmd, this, _1);
  commands["listindexes"] = std::bind(&CommandProcessor::listIndexesCmd, this, _1);
  commands["reconstructtext"] = std::bind(&CommandProcessor::reconstructTextCmd, this, _1);
  commands["compareindexes"] = std::bind(&CommandProcessor::compareIndexesCmd, this, _1);
  commands["longestwords"] = std::bind(&CommandProcessor::longestWordsCmd, this, _1);
  commands["shortestwords"] = std::bind(&CommandProcessor::shortestWordsCmd, this, _1);
  commands["clearindex"] = std::bind(&CommandProcessor::clearIndexCmd, this, _1);
  commands["filterlines"] = std::bind(&CommandProcessor::filterLinesCmd, this, _1);
}

void trukhanov::CommandProcessor::execute(const std::string& line)
{
  std::vector< std::string > args;
  std::size_t wordCount = countWords(line);
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

void trukhanov::CommandProcessor::createIndex(const std::string& indexName, const std::string& filename)
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

  ConcordanceIndex newIndex;
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

  std::vector< std::size_t > lineNumbers(newIndex.lines.size());
  std::iota(lineNumbers.begin(), lineNumbers.end(), 0);

  SplitWords processor{ newIndex.lines, newIndex.index };
  std::transform(lineNumbers.begin(), lineNumbers.end(), lineNumbers.begin(), processor);

  indexes_[indexName] = std::move(newIndex);
  out_ << "Index " << indexName << " created successfully" << '\n';
}

void trukhanov::CommandProcessor::searchWord(const std::string& indexName, const std::string& word)
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  FindWord finder(indexes_.at(indexName), word, indexName, out_);
  finder();
}

void trukhanov::CommandProcessor::showIndex(const std::string& indexName)
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  const ConcordanceIndex& index = it->second;

  std::transform(index.index.begin(), index.index.end(), std::ostream_iterator< WordEntry >(out_, "\n"), pairToWordEntry);
}

void trukhanov::CommandProcessor::clearIndex(const std::string& indexName)
{
  auto it = indexes_.find(indexName);

  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  indexes_.erase(it);
  out_ << "Index " << indexName << " cleared" << '\n';
}

void trukhanov::CommandProcessor::showFrequency(const std::string& indexName)
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  std::vector< std::pair< std::string, std::size_t > > result;

  std::transform(it->second.index.begin(), it->second.index.end(), std::back_inserter(result), frequencyCollector);
  std::sort(result.begin(), result.end(), compareByFrequencyDesc);
  std::transform(result.begin(), result.end(), std::ostream_iterator< std::string >(out_, "\n"), showEntryToString);
}

void trukhanov::CommandProcessor::saveIndex(const std::string& indexName, const std::string& filename)
{
  auto it = indexes_.find(indexName);

  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  std::ofstream file(filename);

  if (!file)
  {
    throw std::invalid_argument("Invalid command");
  }

  const ConcordanceIndex& index = it->second;

  std::transform(index.index.begin(), index.index.end(), std::ostream_iterator< WordEntry >(file, "\n"), pairToWordEntry);
  out_ << "Index " << indexName << " saved to " << filename << '\n';
}

void trukhanov::CommandProcessor::exportWord(const std::string& indexName, const std::string& word, const std::string& filename)
{
  auto it = indexes_.find(indexName);

  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  std::ofstream file(filename);
  if (!file)
  {
    throw std::invalid_argument("Invalid command");
  }

  const ConcordanceIndex& index = it->second;
  ExportWordFunctor exporter{ index.lines, file, word };
  exporter(index.index);

  out_ << "Word " << word << " exported to " << filename << '\n';
}

void trukhanov::CommandProcessor::filterLines(const std::string& indexName, std::size_t fromLine, std::size_t toLine)
{
  auto it = indexes_.find(indexName);

  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  ConcordanceIndex newIndex;
  newIndex.lines.assign(it->second.lines.begin() + fromLine - 1, it->second.lines.begin() + toLine);

  LineFilter filter{ it->second, newIndex, fromLine, toLine };
  filter();

  std::string newName = indexName + "_filtered";
  indexes_[newName] = std::move(newIndex);
  out_ << "Filtered index " << newName << " created from lines " << fromLine << " to " << toLine << '\n';
}

void trukhanov::CommandProcessor::replaceWord(const std::string& indexName, const std::string& oldWord, const std::string& newWord)
{
  auto it = indexes_.find(indexName);

  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  ConcordanceIndex& index = it->second;
  ReplaceWordFunctor replacer{ index, oldWord, newWord };
  replacer();

  out_ << "Word " << oldWord << " replaced with " << newWord << " in index " << indexName << '\n';
}

void trukhanov::CommandProcessor::listIndexes()
{
  std::vector< std::string > lines;
  lines.reserve(indexes_.size());

  std::transform(indexes_.begin(), indexes_.end(), std::back_inserter(lines), formatIndexEntry);
  std::copy(lines.begin(), lines.end(), std::ostream_iterator< std::string >(out_, "\n"));
}

void trukhanov::CommandProcessor::compareIndexes(const std::string& name1, const std::string& name2)
{
  auto it1 = indexes_.find(name1);
  auto it2 = indexes_.find(name2);

  if (it1 == indexes_.end() || it2 == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  const ConcordanceIndex& index1 = it1->second;
  const ConcordanceIndex& index2 = it2->second;

  std::set< std::string > words1;
  std::set< std::string > words2;

  std::transform(index1.index.begin(), index1.index.end(), std::inserter(words1, words1.begin()), extractWord);
  std::transform(index2.index.begin(), index2.index.end(), std::inserter(words2, words2.begin()), extractWord);

  std::set< std::string > common;
  std::set< std::string > unique1;
  std::set< std::string > unique2;

  std::set_intersection(words1.begin(), words1.end(), words2.begin(), words2.end(), std::inserter(common, common.begin()));
  std::set_difference(words1.begin(), words1.end(), words2.begin(), words2.end(), std::inserter(unique1, unique1.begin()));
  std::set_difference(words2.begin(), words2.end(), words1.begin(), words1.end(), std::inserter(unique2, unique2.begin()));

  std::vector< std::string>  diffs;
  std::copy_if(common.begin(), common.end(), std::back_inserter(diffs), FindDifferentFrequencies{ index1, index2 });

  out_ << "Common words:\n";
  std::copy(common.begin(), common.end(), std::ostream_iterator< std::string >(out_, "\n"));

  out_ << "Unique to " << name1 << ":\n";
  std::copy(unique1.begin(), unique1.end(), std::ostream_iterator< std::string >(out_, "\n"));

  out_ << "Unique to " << name2 << ":\n";
  std::copy(unique2.begin(), unique2.end(), std::ostream_iterator< std::string >(out_, "\n"));

  out_ << "Different frequencies:\n";
  std::copy(diffs.begin(), diffs.end(), std::ostream_iterator< std::string >(out_, "\n"));
}

void trukhanov::CommandProcessor::longestWords(const std::string& indexName, std::size_t count)
{
  auto it = indexes_.find(indexName);

  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  const ConcordanceIndex& index = it->second;
  std::vector< std::string > result;

  std::transform(index.index.begin(), index.index.end(), std::back_inserter(result), extractWord);
  std::sort(result.begin(), result.end(), lengthGreater);

  if (result.size() > count)
  {
    result.resize(count);
  }

  std::transform(result.begin(), result.end(), std::ostream_iterator< std::string >(out_, "\n"), OutputWord{ out_ });
}

void trukhanov::CommandProcessor::shortestWords(const std::string& indexName, std::size_t count)
{
  auto it = indexes_.find(indexName);

  if (it == indexes_.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  const ConcordanceIndex& index = it->second;
  std::vector< std::string > result;

  std::transform(index.index.begin(), index.index.end(), std::back_inserter(result), extractWord);
  std::sort(result.begin(), result.end(), lengthLess);

  if (result.size() > count)
  {
    result.resize(count);
  }

  std::transform(result.begin(), result.end(), std::ostream_iterator< std::string >(out_, "\n"), OutputWord{ out_ });
}

void trukhanov::CommandProcessor::mergeIndexes(const std::string& index1, const std::string& index2, const std::string& newIndex)
{
  auto it1 = indexes_.find(index1);
  auto it2 = indexes_.find(index2);

  if (it1 == indexes_.end() || it2 == indexes_.end() || indexes_.count(newIndex))
  {
    throw std::invalid_argument("Invalid command");
  }

  ConcordanceIndex result;

  std::copy(it1->second.index.begin(), it1->second.index.end(), std::inserter(result.index, result.index.end()));

  std::size_t offset = it1->second.lines.size();
  std::for_each(it2->second.index.begin(), it2->second.index.end(), MergeWithOffset{ result, offset });

  std::vector< std::string >& lines1 = it1->second.lines;
  std::vector< std::string >& lines2 = it2->second.lines;

  result.lines.reserve(lines1.size() + lines2.size());
  std::copy(lines1.begin(), lines1.end(), std::back_inserter(result.lines));
  std::copy(lines2.begin(), lines2.end(), std::back_inserter(result.lines));

  indexes_[newIndex] = std::move(result);
  out_ << "Index " << newIndex << " created by merging" << '\n';
}

void trukhanov::CommandProcessor::reconstructText(const std::string& indexName, const std::string& filename)
{
  if (!indexes_.count(indexName))
  {
    throw std::invalid_argument("Invalid command");
  }

  const auto& lines = indexes_.at(indexName).lines;

  std::ofstream outFile(filename);

  if (!outFile.is_open())
  {
    throw std::invalid_argument("Cannot open file");
  }

  std::ostream_iterator< std::string > outIt(outFile, "\n");
  std::copy(lines.begin(), lines.end(), outIt);

  out_ << "Text reconstructed to " << filename << '\n';
}

void trukhanov::CommandProcessor::mergeByLines(const std::string& index1, const std::string& index2, const std::string& newIndex)
{
  if (!indexes_.count(index1) || !indexes_.count(index2) || indexes_.count(newIndex))
  {
    throw std::invalid_argument("Invalid command");
  }

  const auto& lines1 = indexes_.at(index1).lines;
  const auto& lines2 = indexes_.at(index2).lines;
  std::size_t maxSize = std::max(lines1.size(), lines2.size());

  std::vector< std::string > mergedLines(maxSize);
  std::generate(mergedLines.begin(), mergedLines.end(), MergeLinesFunctor{ lines1, lines2, 0 });

  ConcordanceIndex resultIndex;
  resultIndex.lines = mergedLines;

  std::vector< std::size_t > lineNumbers(maxSize);
  std::iota(lineNumbers.begin(), lineNumbers.end(), 1);

  std::transform(lineNumbers.begin(), lineNumbers.end(), lineNumbers.begin(), SplitAndAdd{ resultIndex, resultIndex.lines });

  indexes_[newIndex] = std::move(resultIndex);
  out_ << "Index " << newIndex << " created by merging lines" << '\n';
}
