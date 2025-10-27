#include "details.hpp"
#include <iterator>
#include "utils.hpp"

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
    std::copy(it, --translations.end(), std::ostream_iterator< std::string >(out, ", "));
    out << *(--translations.end());
}