#include "wordFunctors.hpp"
#include "index.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>

krylov::FindWord::FindWord(
  const IndexDocument& index_,
  const std::string& word_,
  const std::string& indexName_,
  std::ostream& out_):
  word(word_), index(index_), indexName(indexName_), out(out_)
{}

void krylov::FindWord::operator()() const
{
  auto it = index.index.find(word);

  if (it == index.index.end())
  {
    out << "<NOT FOUND>\n";
    return;
  }

  WordEntry entry(*it);
  out << entry << '\n';
}


