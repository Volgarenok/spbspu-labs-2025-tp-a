#include "commands.hpp"
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>
#include "utils.hpp"
#include "details.hpp"

void tarasova::createDict(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
    std::string name;
    in >> name;
    if (!in || dicts.find(name) != dicts.end())
    {
        out << "<ERROR: Dictionary already exists>\n";
        return;
    }
    dicts[name] = Dictionary();
}

void tarasova::deleteDict(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
    std::string dictName;
    in >> dictName;

    if (!details::dictionaryExists(dicts, dictName))
    {
        out << "<ERROR: Dictionary not found>\n";
        return;
    }

    dicts.erase(dictName);
}

void tarasova::addWord(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
    std::string dictName, englishWord, russianWord;
    in >> dictName >> englishWord >> russianWord;
    auto it = dicts.find(dictName);
    if (!details::dictionaryExists(dicts, dictName))
    {
        out << "<ERROR: Dictionary not found>\n";
        return;
    }
    if (it->second.find(englishWord) != it->second.end())
    {
        std::cout << "<ERROR: Word already exist>\n";
        return;
    }

    std::list< std::string > russianWords{ russianWord };
    it->second[englishWord] = std::move(russianWords);
}

void tarasova::deleteWord(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
    std::string dictName;
    std::string englishWord;
    in >> dictName >> englishWord;
    if (!details::dictionaryExists(dicts, dictName))
    {
        out << "<ERROR: Dictionary not found>\n";
        return;
    }
    auto& dict = dicts.at(dictName);
    auto it = dict.find(englishWord);
    if (it == dict.end())
    {
        out << "<ERROR: Word not found>\n";
        return;
    }
    dict.erase(it);
}

void tarasova::findWord(std::istream& in, std::ostream& out, const Dictionaries& dicts)
{
    std::string dictName;
    std::string englishWord;
    in >> dictName >> englishWord;
    if (!details::dictionaryExists(dicts, dictName))
    {
        out << "<ERROR: Dictionary not found>\n";
        return;
    }
    const auto& dict = dicts.at(dictName);
    auto it = dict.find(englishWord);
    if (it == dict.end())
    {
        out << "<ERROR: Word not found>\n";
        return;
    }
    details::printTranslations(out, it->second);
}

void tarasova::addTranslate(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
    std::string dictName, englishWord, russianWord;
    in >> dictName >> englishWord >> russianWord;

    if (!details::dictionaryExists(dicts, dictName))
    {
        out << "<ERROR: Dictionary not found>\n";
        return;
    }

    auto& dict = dicts.at(dictName);
    auto wordIt = dict.find(englishWord);

    if (wordIt == dict.end())
    {
        out << "ERROR: Word not found\n";
        return;
    }

    auto& translations = wordIt->second;
    auto translationIt = std::find(translations.begin(), translations.end(), russianWord);
    if (translationIt != translations.end())
    {
        out << "ERROR: Translation already exist\n";
        return;
    }
    translations.push_back(russianWord);
}

void tarasova::deleteTranslation(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
    std::string dictName, englishWord, russianWord;
    in >> dictName >> englishWord >> russianWord;

    if (!details::dictionaryExists(dicts, dictName))
    {
        out << "<ERROR: Dictionary not found>\n";
        return;
    }

    auto& dict = dicts.at(dictName);
    auto wordIt = dict.find(englishWord);

    if (wordIt == dict.end())
    {
        out << "ERROR: Word not found\n";
        return;
    }

    auto& translations = wordIt->second;

    auto translationIt = std::find(translations.begin(), translations.end(), russianWord);
    if (translationIt == translations.end())
    {
        out << "ERROR: Translation not found\n";
        return;
    }

    translations.erase(translationIt);
    if (translations.empty())
    {
        dict.erase(wordIt);
    }
}

struct ContainsRussian
{
    const std::string target_;

    explicit ContainsRussian(const std::string& target) :
        target_(target)
    {
    }

    template < class Pair >
    bool operator()(const Pair& p) const
    {
        return std::find(p.second.begin(), p.second.end(), target_) != p.second.end();
    }
};

void tarasova::findTranslate(std::istream& in, std::ostream& out, const Dictionaries& dicts)
{
    std::string dictName, russianWord;
    in >> dictName >> russianWord;
    using pair_type = std::pair< std::string, std::list< std::string > >;

    if (!details::dictionaryExists(dicts, dictName))
    {
        out << "<Dictionary not found>\n";
        return;
    }

    const auto& dict = dicts.at(dictName);
    std::vector< pair_type > matchingPairs;
    std::copy_if(dict.begin(), dict.end(), std::back_inserter(matchingPairs), ContainsRussian{ russianWord });

    std::vector< std::string > foundWords;
    foundWords.reserve(matchingPairs.size());
    std::transform(matchingPairs.begin(), matchingPairs.end(), std::back_inserter(foundWords), details::KeyExtractor{});

    if (foundWords.empty())
    {
        out << "ERROR: Translation not found\n";
        return;
    }

    out << russianWord << " - ";
    std::copy(foundWords.begin(), foundWords.end(), std::ostream_iterator< std::string >(out, " "));
    out << "\n";
}

void tarasova::printDict(std::istream& in, std::ostream& out, const Dictionaries& dicts)
{
    std::string name;
    in >> name;
    if (!in || dicts.find(name) == dicts.end())
    {
        out << "<ERROR: Dictionary not found>\n";
        return;
    }
    auto dict = dicts.at(name);
    if (dict.empty())
    {
        out << "<ERROR: Dictionary is empty>\n";
        return;
    }
    else
    {
        out << dicts.at(name);
    }
}

void tarasova::clear(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
    std::string dictName;
    in >> dictName;
    if (!details::dictionaryExists(dicts, dictName))
    {
        out << "<ERROR: Dictionary not found>\n";
        return;
    }
    auto& dict = dicts.at(dictName);
    dict.clear();
}

void tarasova::mergeDicts(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
    std::string dictName1;
    std::string dictName2;
    std::string newDictName;
    in >> dictName1 >> dictName2 >> newDictName;
    if (!details::dictionaryExists(dicts, dictName1) || !details::dictionaryExists(dicts, dictName2))
    {
        out << "<ERROR: One or both source dictionaries not found>\n";
        return;
    }
    auto& dict1 = dicts.at(dictName1);
    auto& dict2 = dicts.at(dictName2);
    Dictionary newDict;
    std::copy(dict1.begin(), dict1.end(), std::inserter(newDict, newDict.begin()));
    details::DictMerger merger(newDict);
    std::transform(dict2.begin(), dict2.end(), std::inserter(newDict, newDict.end()), merger);
    dicts[newDictName] = newDict;
}

void tarasova::delRepeatWords(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
    std::string dictName1;
    std::string dictName2;
    std::string newDictName;
    in >> dictName1 >> dictName2 >> newDictName;
    if (!details::dictionaryExists(dicts, dictName1) || !details::dictionaryExists(dicts, dictName2))
    {
        out << "<ERROR: One or both source dictionaries not found>\n";
        return;
    }
    const auto& dict1 = dicts.at(dictName1);
    const auto& dict2 = dicts.at(dictName2);
    std::list< std::string > keys1, keys2, unique_keys;
    std::transform(dict1.begin(), dict1.end(), std::inserter(keys1, keys1.begin()), details::KeyExtractor{});
    std::transform(dict2.begin(), dict2.end(), std::inserter(keys2, keys2.begin()), details::KeyExtractor{});
    std::set_symmetric_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(),
        std::inserter(unique_keys, unique_keys.begin()));
    if (unique_keys.empty())
    {
        out << "<ERROR: no reapeating words>\n";
        return;
    }
    Dictionary newDict;
    details::PairTransformer transformer(dict1, dict2);
    std::transform(unique_keys.begin(), unique_keys.end(), std::inserter(newDict, newDict.begin()), transformer);
    dicts[newDictName] = newDict;
}

void tarasova::replaceWord(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
    std::string dictName, oldEnglishWord, newEnglishWord;
    in >> dictName >> oldEnglishWord >> newEnglishWord;

    if (!details::dictionaryExists(dicts, dictName))
    {
        out << "<ERROR: Dictionary not found>\n";
        return;
    }

    auto& dict = dicts.at(dictName);
    auto oldWordIt = dict.find(oldEnglishWord);

    if (oldWordIt == dict.end())
    {
        out << "ERROR: Word not found\n";
        return;
    }

    auto newWordIt = dict.find(newEnglishWord);
    if (newWordIt != dict.end())
    {
        out << "ERROR: New word already exists\n";
        return;
    }

    Translations savedTranslations = std::move(oldWordIt->second);
    dict.erase(oldWordIt);
    dict[newEnglishWord] = std::move(savedTranslations);
}