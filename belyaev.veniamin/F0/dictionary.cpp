#include "dictionary.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>

std::ostream& belyaev::operator<<(std::ostream& out, const entryPair& pair)
{
  out << pair.first << " -> " << pair.second;
  return out;
}

std::string belyaev::formPairString(const entryPair& pair)
{
  return pair.first + " -> " + pair.second;
}

std::ostream& belyaev::operator<<(std::ostream& out, const Dictionary& dictionary)
{
  if (dictionary.dict.empty())
  {
    out << "<EMPTY>\n";
    return out;
  }
  using ostreamItStr = std::ostream_iterator< std::string >;
  std::transform(dictionary.dict.begin(), dictionary.dict.end(), ostreamItStr{out, "\n"}, formPairString);
  return out;
}
