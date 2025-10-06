#include "Dictionary.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <limits>

namespace
{
  struct WordWithTranslations
  {
    std::string enWord;
    std::list< std::string > translations;
  };

  std::ostream& operator<<(std::ostream& out, const WordWithTranslations& rhs)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }

    out << rhs.enWord << " ";
    out << rhs.translations.size() << " ";
    using OstreamIter = std::ostream_iterator< std::string >;
    std::copy(rhs.translations.begin(), rhs.translations.end(), OstreamIter(out, " "));
    return out;
  }

  std::istream& operator>>(std::istream& in, WordWithTranslations& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    WordWithTranslations wordWithTranslationsTemp;
    in >> wordWithTranslationsTemp.enWord;
    if (!in)
    {
      return in;
    }

    int amountTranslations = 0;
    in >> amountTranslations;
    if (!in)
    {
      return in;
    }

    using IstreamIter = std::istream_iterator< std::string >;
    std::copy_n(IstreamIter(in), amountTranslations, std::back_inserter(wordWithTranslationsTemp.translations));
    if (in)
    {
      rhs = wordWithTranslationsTemp;
    }
    return in;
  }

  WordWithTranslations createWordWithTranslations(const holodilov::MapWordsPair& pair)
  {
    return WordWithTranslations{ pair.first, pair.second };
  }

  holodilov::MapWordsPair createMapWordsPair(const WordWithTranslations& wordWithTranslations)
  {
    return holodilov::MapWordsPair{ wordWithTranslations.enWord, wordWithTranslations.translations };
  }
}

std::ostream& holodilov::operator<<(std::ostream& out, const Dictionary& dict)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << dict.name << " " << dict.lang << "\n";
  out << dict.dict.size() << "\n";

  std::vector< WordWithTranslations > vecWordWithTransl;
  std::transform(dict.dict.begin(), dict.dict.end(), std::back_inserter(vecWordWithTransl), createWordWithTranslations);

  using OstreamIter = std::ostream_iterator< WordWithTranslations >;
  std::copy(vecWordWithTransl.begin(), vecWordWithTransl.end(), OstreamIter(out, "\n"));
  return out;
}

std::istream& holodilov::operator>>(std::istream& in, Dictionary& dict)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string name;
  std::string lang;
  in >> name;
  in >> lang;
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  if (!in)
  {
    return in;
  }

  int amountWords = 0;
  in >> amountWords;
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  if (!in)
  {
    return in;
  }
  if (amountWords < 0)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (amountWords == 0)
  {
    dict.name = name;
    dict.lang = lang;
    return in;
  }

  using IstreamIter = std::istream_iterator< WordWithTranslations >;
  std::vector< WordWithTranslations > vecWordWithTransl;
  std::copy_n(IstreamIter(in), amountWords, std::back_inserter(vecWordWithTransl));
  if (!in)
  {
    return in;
  }

  auto inserter = std::inserter(dict.dict, dict.dict.end());
  std::transform(vecWordWithTransl.begin(), vecWordWithTransl.end(), inserter, createMapWordsPair);
  dict.name = name;
  dict.lang = lang;
  return in;
}
