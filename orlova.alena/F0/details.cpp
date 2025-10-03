#include "details.h"
#include <iterator>
#include "io-utils.h"

bool details::dictionaryExists(const Dictionaries& dicts, const std::string& name)
{
  return dicts.find(name) != dicts.end();
}

void details::printTranslations(std::ostream& out, const Translations& translations)
{
  if (translations.empty())
  {
    return;
  }
  auto it = translations.begin();
  out << *it << ", ";
  std::advance(it, 1);
  std::copy(it, --translations.end(), std::ostream_iterator< std::string >(out, ", "));
  out << *(--translations.end());
}

orlova::Translations details::intersectLists(Translations& list1, Translations& list2)
{
  list1.sort();
  list2.sort();
  Translations res;
  std::set_intersection(list1.begin(), list1.end(), list2.begin(), list2.end(), std::back_inserter(res));
  return res;
}

std::pair< std::string, orlova::Translations > details::intersectListDict(const std::pair< std::string, Translations >& pair,
  const orlova::Dictionary& intersected)
{
  Translations list1 = pair.second;
  Translations list2 = intersected.at(pair.first);
  Translations res = intersectLists(list1, list2);
  return { pair.first, res };
}
