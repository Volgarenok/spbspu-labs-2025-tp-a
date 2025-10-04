#include "Alphabet.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <iostream>

namespace
{
  const std::string& joinWords(const std::string& str, std::string& strJoined)
  {
    strJoined += str;
    return str;
  }

  std::string joinListsToWords(const holodilov::MapWordsPair& pair)
  {
    std::string strJoined;
    std::vector< std::string > vecWordsTemp;
    auto joinWordsBound = std::bind(joinWords, std::placeholders::_1, std::ref(strJoined));
    std::transform(pair.second.begin(), pair.second.end(), std::back_inserter(vecWordsTemp), joinWordsBound);
    return strJoined;
  }
}

holodilov::Alphabet::Alphabet(const Dictionary &dictionary)
{
  std::vector< std::string > vecWords;
  std::transform(dictionary.dict.begin(), dictionary.dict.end(), std::back_inserter(vecWords), joinListsToWords);

  std::string strJoined;
  auto joinWordsBound = std::bind(joinWords, std::placeholders::_1, std::ref(strJoined));
  std::transform(vecWords.begin(), vecWords.end(), vecWords.begin(), joinWordsBound);

  std::sort(strJoined.begin(), strJoined.end());
  auto newStrEnd = std::unique(strJoined.begin(), strJoined.end());
  strJoined.erase(newStrEnd, strJoined.end());
  alphabet_ = strJoined;
}

holodilov::Alphabet::Alphabet(std::istream& in)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    in.setstate(std::ios::failbit);
    return;
  }
  in >> alphabet_;
}

std::ostream& holodilov::operator<<(std::ostream& out, const Alphabet& alphabet)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << alphabet.alphabet_;
  return out;
}

bool holodilov::Alphabet::operator==(const Alphabet& rhs) const
{
  return alphabet_ == rhs.alphabet_;
}
