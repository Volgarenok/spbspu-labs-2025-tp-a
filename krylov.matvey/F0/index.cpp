#include "index.hpp"
#include "splitFunctors.hpp"
#include <iostream>
#include <set>
#include <iterator>
#include <algorithm>

krylov::WordEntry::WordEntry(const std::pair< const std::string, std::set< size_t > >& p):
  word(p.first), lines(p.second)
{}

std::ostream& krylov::operator<<(std::ostream& out, const std::set< size_t >& s)
{
  if (s.empty())
  {
    return out;
  }

  auto it = s.begin();
  out << *it;
  ++it;

  if (it != s.end())
  {
    out << " ";
    std::copy(it, std::prev(s.end()), std::ostream_iterator< size_t >(out, " "));
    out << *std::prev(s.end());
  }

  return out;
}

std::ostream& krylov::operator<<(std::ostream& out, const krylov::WordEntry& entry)
{
  out << entry.word << " : " << entry.lines;
  return out;
}

std::istream& krylov::operator>>(std::istream& in, std::vector< std::string >& args)
{
  std::string line;
  if (!std::getline(in, line))
  {
    return in;
  }
  std::size_t wordCount = countWords(line);
  WordGenerator gen{ line.begin(), line.end() };
  std::generate_n(std::back_inserter(args), wordCount, gen);
  return in;
}
