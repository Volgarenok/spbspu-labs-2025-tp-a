#ifndef DETAILS_HPP
#define DETAILS_HPP

#include <algorithm>
#include "commands.hpp"
#include "utils.hpp"

namespace details
{
    using namespace tarasova;
    bool dictionaryExists(const Dictionaries&, const std::string&);
    void printTranslations(std::ostream&, const Translations&);

    struct DictMerger
    {
        using Pair = std::pair< std::string, std::list< std::string > >;
        Dictionary newDict;
        DictMerger(const Dictionary& dict) :
            newDict(dict)
        {
        }
        Pair operator()(const Pair& pair)
        {
            auto it = newDict.find(pair.first);
            if (it != newDict.end())
            {
                it->second.insert(it->second.end(), pair.second.begin(), pair.second.end());
                return *it;
            }
            else
            {
                newDict[pair.first] = pair.second;
                return pair;
            }
        }
    };

    struct KeyExtractor
    {
        using Pair = std::pair< std::string, std::list< std::string > >;
        std::string operator()(const Pair& p) const
        {
            return p.first;
        }
    };

    struct PairTransformer
    {
        using Pair = std::pair< std::string, std::list< std::string > >;
        const Dictionary& dict1;
        const Dictionary& dict2;

        PairTransformer(const Dictionary& d1, const Dictionary& d2) :
            dict1(d1),
            dict2(d2)
        {
        }

        Pair operator()(std::string& key)
        {
            if (dict1.find(key) != dict1.end())
            {
                return { key, dict1.at(key) };
            }
            else
            {
                return { key, dict2.at(key) };
            }
        }
    };
}

#endif