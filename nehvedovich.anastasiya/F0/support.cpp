#include "support.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iterator>
#include <numeric>

bool nehvedovich::isLetter(char c)
{
  return std::isalpha(static_cast<unsigned char>(c));
}

char nehvedovich::toLower(char c)
{
  return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

bool nehvedovich::isValidChar(unsigned char c)
{
  return std::isalnum(c) || c == '_';
}

bool nehvedovich::isValidName(const std::string& name)
{
  return !name.empty() && std::all_of(name.begin(), name.end(), isValidChar);
}

bool nehvedovich::wordComparator(const std::pair<std::string, std::vector<std::string>>& a,
  const std::pair<std::string, std::vector<std::string>>& b)
{
  return a.first < b.first;
}

nehvedovich::Join::Join(const std::string& s):
  sep(s)
{}

std::string nehvedovich::Join::operator()(const std::vector<std::string>& v) const
{
  if (v.empty())
  {
    return "";
  }

  return std::accumulate(std::next(v.begin()), v.end(), v.front(), *this);
}

std::string nehvedovich::Join::operator()(const std::string& a, const std::string& b) const
{
  return joinPair(a, b);
}

std::string nehvedovich::Join::joinPair(const std::string& a, const std::string& b) const
{
  return a + sep + b;
}

nehvedovich::WordFormatter::WordFormatter(Mode m):
  mode(m)
{}

std::string nehvedovich::WordFormatter::operator()(
  const std::pair< std::string, std::vector< std::string > >& entry) const
{
  switch (mode)
  {
  case IdentityMode:
    return entry.first;
  case ExtractMode:
    return entry.first;
  case PrintMode:
    return entry.first + " - " + formatTranslations(entry.second);
  }
  return "";
}

std::string nehvedovich::formatTranslations(const std::vector< std::string >& translations)
{
  std::vector< std::string > sortedTranslations = translations;
  std::sort(sortedTranslations.begin(), sortedTranslations.end());
  return Join()(sortedTranslations);
}

nehvedovich::TranslationFilter::TranslationFilter(Mode m, size_t c, std::string t): 
  mode(m),
  count(c),
  target(std::move(t))
{}

bool nehvedovich::TranslationFilter::operator()(
  const std::pair< std::string, std::vector< std::string > >& entry) const
{
  switch (mode)
  {
  case MoreThanMode:
    return entry.second.size() > count;
  case ContainsMode:
    return std::find(entry.second.begin(), entry.second.end(), target) != entry.second.end();
  }

  return false;
}

const std::string& nehvedovich::AddTranslation::operator()(const std::string& tr) const
{
  translations.push_back(tr);
  std::sort(translations.begin(), translations.end());
  translations.erase(std::unique(translations.begin(), translations.end()), translations.end());
  return tr;
}

std::pair< std::string, std::vector< std::string > > nehvedovich::MergeDictionaries::operator()(
  const std::pair< std::string, std::vector< std::string > >& entry) const
{
  auto it = result.find(entry.first);

  if (it != result.end())
  {
    std::vector< std::string > tmp = entry.second;
    std::transform(tmp.begin(), tmp.end(), tmp.begin(),
      AddTranslation{ it->second });
  }
  else
  {
    result[entry.first] = entry.second;
  }

  return entry;
}
