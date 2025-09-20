#include "Alphabet.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

namespace
{
  std::list< std::string > dictPairToTranslations(const std::pair< std::string, std::list< std::string > >& pair)
  {
    return pair.second;
  }

  std::string translationToAlphabet(std::string& translation, std::set< char >& alphabet)
  {
    alphabet.insert(translation.begin(), translation.end());
    return translation;
  }

  std::list< std::string > translationListsToAlphabet(std::list< std::string >& translations, std::set< char >& alphabet)
  {
    auto translationToAlphabetBound = std::bind(translationToAlphabet, std::placeholders::_1, std::ref(alphabet));
    std::transform(translations.begin(), translations.end(), translations.begin(), translationToAlphabetBound);
    return translations;
  }
}

void holodilov::Alphabet::load(const Dictionary& dictionary)
{
  std::vector< std::list< std::string > > vecTranslationLists(dictionary.dict.size());
  std::transform(dictionary.dict.begin(), dictionary.dict.end(), vecTranslationLists.begin(), dictPairToTranslations);

  auto translationsToAlphabetBound = std::bind(translationListsToAlphabet, std::placeholders::_1, std::ref(alphabet_));
  std::transform(vecTranslationLists.begin(), vecTranslationLists.end(), vecTranslationLists.begin(), translationsToAlphabetBound);
  isLoaded_ = true;
}

bool holodilov::Alphabet::isLoaded() const {
  return isLoaded_;
}

std::ostream& holodilov::operator<<(std::ostream& out, const Alphabet& alphabet)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  using ostreamIter = std::ostream_iterator< char >;
  std::copy(alphabet.alphabet_.begin(), alphabet.alphabet_.end(), ostreamIter(out));
  return out;
}

std::istream& holodilov::operator>>(std::istream& in, Alphabet& alphabet)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  using istreamIter = std::istream_iterator< char >;
  std::copy(istreamIter(in), istreamIter(), std::inserter(alphabet.alphabet_, alphabet.alphabet_.end()));
  alphabet.isLoaded_ = true;
  return in;
}

bool holodilov::Alphabet::operator==(const Alphabet& rhs) const
{
  return alphabet_ == rhs.alphabet_;
}
