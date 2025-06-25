#include "functors.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>

void trukhanov::LineSplitter::operator()(const std::string& word) const
{
  wordMap[word].insert(lineNumber);
}

void trukhanov::FrequencyCollector::operator()(const std::pair< const std::string, std::set< std::size_t > >& entry) const
{
  result.push_back(std::make_pair(entry.first, entry.second.size()));
}

void trukhanov::ShowIndexEntry::operator()(const std::pair< const std::string, std::set< size_t > >& entry) const
{
  out << entry.first << ' ';
  std::copy(entry.second.begin(), entry.second.end(), std::ostream_iterator< std::size_t >(out, " "));
  out << '\n';
}

void trukhanov::ShowEntry::operator()(const std::pair< std::string, std::size_t >& pair) const
{
  out << pair.first << ' ' << pair.second << '\n';
}

trukhanov::FindWord::FindWord(
  const ConcordanceIndex& index_,
  const std::string& word_, 
  const std::string& indexName_,
  std::ostream& out_) :
  word(word_),
  index(index_),
  indexName(indexName_),
  out(out_)
{
}

void trukhanov::FindWord::operator()() const
{
  auto it = index.index.find(word);

  if (it == index.index.end())
  {
    out << "<NOT FOUND>\n";
    return;
  }

  std::copy(it->second.begin(), it->second.end(), std::ostream_iterator< size_t >(out, " "));
  out << '\n';
}

void trukhanov::ReplaceWordFunctor::operator()()
{
  if (oldWord == newWord)
  {
    return;
  }

  auto it = index.index.find(oldWord);

  if (it == index.index.end())
  {
    return;
  }

  index.index[newWord].insert(it->second.begin(), it->second.end());
  index.index.erase(oldWord);
}

struct ProcessPair
{
  std::size_t from;
  std::size_t to;
  const trukhanov::ConcordanceIndex& src;
  trukhanov::ConcordanceIndex& dest;

  void operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const
  {
    std::vector<std::size_t> filtered;
    std::copy_if(pair.second.begin(), pair.second.end(), std::back_inserter(filtered), trukhanov::InRange{ from, to });

    if (!filtered.empty())
    {
      std::set< std::size_t > adjusted;
      std::transform(filtered.begin(),
        filtered.end(),
        std::inserter(adjusted,adjusted.end()),
        trukhanov::AdjustOffset{ from - 1 });
      dest.index[pair.first] = std::move(adjusted);
    }
  }
};

void trukhanov::LineFilter::operator()() const
{
  std::for_each(src.index.begin(), src.index.end(), ProcessPair{ from, to, src, dest });
}

void trukhanov::FileWriter::operator()(const std::pair< const std::string, std::set< std::size_t > >& entry) const
{
  out << entry.first << ' ';
  std::copy(entry.second.begin(), entry.second.end(), std::ostream_iterator< std::size_t >(out, " "));
  out << '\n';
}

trukhanov::ExportWordFunctor::ExportWordFunctor(
  const std::vector< std::string >& lines_,
  std::ostream& out_, const std::string& word_) :
  lines(lines_),
  out(out_),
  word(word_)
{
}

void trukhanov::ExportWordFunctor::operator()(const std::map< std::string, std::set< std::size_t > >& index) const
{
  auto it = index.find(word);

  if (it == index.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  out << word;
  std::for_each(it->second.begin(), it->second.end(), trukhanov::PrintExportLine{ out, lines });
  out << '\n';
}

bool trukhanov::LengthGreater::operator()(const std::string& a, const std::string& b) const
{
  return a.length() > b.length() || (a.length() == b.length() && a < b);
}

bool trukhanov::LengthLess::operator()(const std::string& a, const std::string& b) const
{
  return a.length() < b.length() || (a.length() == b.length() && a < b);
}

std::string trukhanov::ExtractWord::operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const
{
  return pair.first;
}

std::string trukhanov::OutputWord::operator()(const std::string& word) const
{
  return word;
}

bool trukhanov::FindDifferentFrequencies::operator()(const std::string& word) const
{
  const auto& set1 = index1.index.at(word);
  const auto& set2 = index2.index.at(word);
  return set1 != set2;
}

void trukhanov::InsertToIndex::operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const
{
  index.index[pair.first] = pair.second;
}

std::string trukhanov::MergeLinesFunctor::operator()()
{
  std::string l1 = (i < lines1.size()) ? lines1[i] : "";
  std::string l2 = (i < lines2.size()) ? lines2[i] : "";
  ++i;
  return l1 + " " + l2;
}

bool trukhanov::CompareByFrequencyDesc::operator()(
  const std::pair< std::string,
  std::size_t >& a,
  const std::pair< std::string,
  std::size_t >& b) const
{
  return a.second > b.second;
}

std::string trukhanov::FormatIndexEntry::operator()(const std::pair< const std::string, ConcordanceIndex >& pair) const
{
  const std::string& file = pair.second.sourceFile;
  return pair.first + ": " + (file.empty() ? "<NO FILE>" : file);
}

void trukhanov::MergeWithOffset::operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const
{
  std::set< std::size_t > adjusted;
  std::transform(
    pair.second.begin(),
    pair.second.end(),
    std::inserter(adjusted,adjusted.end()),
    trukhanov::AddOffset{ offset });
  dest.index[pair.first].insert(adjusted.begin(), adjusted.end());
}

void trukhanov::PrintExportLine::operator()(std::size_t lineNo) const
{
  if (lineNo == 0 || lineNo > lines.size())
  {
    out << " <INVALID LINE NUMBER>";
  }
  else
  {
    out << ' ' << lineNo;
  }
}

std::size_t trukhanov::SplitWords::operator()(std::size_t lineNumber) const
{
  const std::string& line = lines.at(lineNumber);
  std::istringstream iss(line);
  std::vector< std::string > words;
  std::copy(std::istream_iterator< std::string >(iss), std::istream_iterator< std::string >(), std::back_inserter(words));
  LineSplitter splitter{ wordMap, lineNumber + 1 };
  std::for_each(words.begin(), words.end(), splitter);
  return lineNumber;
}
