#include "Dictionary.hpp"
#include <algorithm>
#include <iterator>
#include <map>
#include <limits>
#include <iostream>

std::ostream& holodilov::operator<<(std::ostream& out, const Dictionary& dict)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << dict.name << " " << dict.lang << "\n";

  using OstreamIterator = std::ostream_iterator< std::string >;

  auto beginIter = dict.dict.begin();
  while (beginIter != dict.dict.end())
  {
    out << beginIter->first << " ";
    out << beginIter->second.size() << " ";
    std::copy(std::begin(beginIter->second), std::end(beginIter->second), OstreamIterator(out, " "));
    ++beginIter;

    if (beginIter != dict.dict.end()) {
      out << "\n";
    }
  }
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

  Dictionary dictTemp(std::move(name), std::move(lang));

  std::string englishWord;
  while (!in.eof())
  {
    in >> englishWord;
    dictTemp.dict[englishWord] = std::list< std::string >();

    int amountTranslations = 0;
    in >> amountTranslations;

    using istreamIter = std::istream_iterator< std::string >;
    std::copy_n(istreamIter(in), amountTranslations, std::back_inserter(dictTemp.dict[englishWord]));

  }
  if (in)
  {
    dict = dictTemp;
  }
  return in;
}
