#include "commands.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <iostream>
#include <vector>
#include <set>
#include "Alphabet.hpp"
#include "ScopeGuard.hpp"

namespace
{
  struct SetTranslationsCreator
  {
    const std::string& englishWord;
    std::set< std::string >& setTranslations;

    holodilov::Dictionary& operator()(holodilov::MapDictsPair& pair)
    {
      if (pair.second.dict.find(englishWord) != pair.second.dict.end())
      {
        std::list < std::string > listTranslations = pair.second.dict.at(englishWord);
        setTranslations.insert(listTranslations.begin(), listTranslations.end());
      }
      return pair.second;
    }
  };

  std::string dictToDictName(const holodilov::MapDictsPair& pair)
  {
    return pair.first;
  }

  bool checkIntersect(const holodilov::MapWordsPair& pair, const holodilov::Dictionary& dict)
  {
    return dict.dict.find(pair.first) != dict.dict.end();
  }

  holodilov::MapWordsPair mergeWords(const holodilov::MapWordsPair& pair, const holodilov::MapWords& mapDict2)
  {
    holodilov::MapWordsPair result = pair;
    if (mapDict2.find(result.first) != mapDict2.end())
    {
      std::list< std::string > translationsDict2 = mapDict2.at(result.first);
      result.second.insert(result.second.end(), translationsDict2.begin(), translationsDict2.end());
      result.second.sort();
      result.second.unique();
    }
    return result;
  }
}

void holodilov::createDict(std::istream& in, std::ostream& out, MapDicts& dictionaries)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }
  std::string lang;
  in >> lang;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }
  if (dictionaries.find(name) != dictionaries.end())
  {
    throw std::logic_error("Error: dictionary with this name already exists.");
  }

  dictionaries.emplace(name, Dictionary{ name, lang, MapWords() });
  out << "Created dictionary:\n" << "name: " << name << "\nlang: " << lang;
}

void holodilov::exportDict(std::istream& in, std::ostream& out, MapDicts& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }
  std::string fileName;
  in >> fileName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if (dictionaries.find(dictName) == dictionaries.end())
  {
    throw std::logic_error("Error: dictionary not found.");
  }

  std::ofstream fout;
  fout.open(fileName);
  if (!fout.is_open())
  {
    throw std::logic_error("Error: unable to write dictionary into file.");
  }
  fout << dictionaries[dictName];
  if (!fout)
  {
    fout.close();
    throw std::logic_error("Error: unable to write dictionary into file.");
  }
  fout.close();

  out << "Dictionary " << dictName << " successfully exported into file " << fileName;
}

void holodilov::importDict(std::istream& in, std::ostream& out, MapDicts& dictionaries)
{
  std::string filename;
  in >> filename;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::ifstream fis;
  fis.open(filename);
  if (!fis.is_open())
  {
    throw std::logic_error("Error: dictionary file not found.");
  }
  Dictionary dict;
  fis >> dict;
  if (!fis)
  {
    fis.close();
    throw std::logic_error("Error: dictionary file has invalid format.");
  }
  fis.close();

  if (dictionaries.find(dict.name) != dictionaries.end())
  {
    throw std::logic_error("Error: dictionary with this name already exists.");
  }

  dictionaries.emplace(dict.name, dict);
  out << "Dictionary " << dict.name << " was successfully imported.";
}

void holodilov::clearDict(std::istream& in, std::ostream& out, MapDicts& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if (dictionaries.find(dictName) == dictionaries.end())
  {
    throw std::logic_error("Error: dictionary not found.");
  }
  dictionaries.at(dictName).dict.clear();

  out << "Dictionary " << dictName << " was cleared.";
}

void holodilov::deleteDict(std::istream& in, std::ostream& out, MapDicts& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if (dictionaries.find(dictName) == dictionaries.end())
  {
    throw std::logic_error("Error: dictionary not found.");
  }
  dictionaries.erase(dictName);

  out << "Dictionary " << dictName << " was deleted.";
}

void holodilov::addWord(std::istream& in, std::ostream& out, MapDicts& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string enWord;
  in >> enWord;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  int amountTranslations = 0;
  in >> amountTranslations;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if (dictionaries.find(dictName) == dictionaries.end())
  {
    throw std::logic_error("Error: dictionary not found.");
  }

  Dictionary& dict = dictionaries.at(dictName);
  if (dict.dict.find(enWord) != dict.dict.end())
  {
    throw std::logic_error("Error: this word is already in the dictionary.");
  }
  dict.dict[enWord] = std::list< std::string >();

  using istreamIter = std::istream_iterator< std::string >;
  std::copy_n(istreamIter(in), amountTranslations, std::back_inserter(dict.dict.at(enWord)));
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }
  out << "Word " << enWord << " was added to " << dictName;
}

void holodilov::deleteWord(std::istream& in, std::ostream& out, MapDicts& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string englishWord;
  in >> englishWord;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if (dictionaries.find(dictName) == dictionaries.end())
  {
    throw std::logic_error("Error: dictionary not found.");
  }

  if (dictionaries.at(dictName).dict.find(englishWord) == dictionaries.at(dictName).dict.end())
  {
    throw std::logic_error("Error: word not found in this dictionary.");
  }

  dictionaries.at(dictName).dict.erase(englishWord);
  out << "Word " << englishWord << " was deleted from dictionary " << dictName;
}

void holodilov::updateWord(std::istream& in, std::ostream& out, MapDicts& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string englishWord;
  in >> englishWord;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string englishWordNew;
  in >> englishWordNew;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if (dictionaries.find(dictName) == dictionaries.end())
  {
    throw std::logic_error("Error: dictionary not found.");
  }
  if (dictionaries.at(dictName).dict.find(englishWord) == dictionaries.at(dictName).dict.end())
  {
    throw std::logic_error("Error: word not found in this dictionary.");
  }

  std::list< std::string > translations = dictionaries.at(dictName).dict.at(englishWord);
  dictionaries.at(dictName).dict.erase(englishWord);
  dictionaries.at(dictName).dict[englishWordNew] = translations;
  out << "Word " << englishWord << " was updated to " << englishWordNew;
}

void holodilov::printDict(std::istream& in, std::ostream& out, const MapDicts& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if (dictionaries.find(dictName) == dictionaries.end())
  {
    throw std::logic_error("Error: dictionary not found.");
  }
  out << dictionaries.at(dictName);
}

void holodilov::findWord(std::istream& in, std::ostream& out, MapDicts& mapDicts)
{
  std::string englishWord;
  in >> englishWord;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::vector< Dictionary > vecDictionaries;
  std::set< std::string > setTranslations;

  SetTranslationsCreator creator{ englishWord, setTranslations };
  std::transform(mapDicts.begin(), mapDicts.end(), std::back_inserter(vecDictionaries), creator);
  if (setTranslations.empty())
  {
    out << "Unable to find translations of this word.";
  }
  std::copy(setTranslations.begin(), setTranslations.end(), std::ostream_iterator< std::string >(out, "\n"));
}

void holodilov::merge(std::istream& in, std::ostream& out, MapDicts& dictionaries)
{
  std::string dictName1;
  in >> dictName1;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string dictName2;
  in >> dictName2;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string dictNameNew;
  in >> dictNameNew;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string dictLangNew;
  in >> dictLangNew;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if ((dictionaries.find(dictName1) == dictionaries.end()) || (dictionaries.find(dictName2) == dictionaries.end()))
  {
    throw std::logic_error("Error: dictionary not found.");
  }

  MapWords mapNewDict;
  Dictionary& dict1 = dictionaries.at(dictName1);
  Dictionary& dict2 = dictionaries.at(dictName2);

  auto mergeWordsBound = std::bind(mergeWords, std::placeholders::_1, std::cref(dict2.dict));
  std::transform(dict1.dict.begin(), dict1.dict.end(), std::inserter(mapNewDict, mapNewDict.end()), mergeWordsBound);
  mapNewDict.insert(dict2.dict.begin(), dict2.dict.end());

  Dictionary dictNew{ dictNameNew, dictLangNew, mapNewDict };
  dictionaries[dictNameNew] = dictNew;
  out << "Dictionaries " << dictName1 << " and " << dictName2 << " were merged to " << dictNameNew;
}

void holodilov::intersect(std::istream& in, std::ostream& out, MapDicts& dictionaries)
{
  std::string dictName1;
  in >> dictName1;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string dictName2;
  in >> dictName2;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string dictNameNew;
  in >> dictNameNew;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string dictLangNew;
  in >> dictLangNew;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if ((dictionaries.find(dictName1) == dictionaries.end()) || (dictionaries.find(dictName2) == dictionaries.end()))
  {
    throw std::logic_error("Error: dictionary not found.");
  }

  Dictionary& dict1 = dictionaries.at(dictName1);
  Dictionary& dict2 = dictionaries.at(dictName2);
  MapWords mapNewDict;

  auto checkIntersectBound = std::bind(checkIntersect, std::placeholders::_1, std::cref(dict2));
  auto inserterNewDict = std::inserter(mapNewDict, mapNewDict.end());
  std::copy_if(dict1.dict.begin(), dict1.dict.end(), inserterNewDict, checkIntersectBound);

  auto mergeWordsBound = std::bind(mergeWords, std::placeholders::_1, std::cref(dict2.dict));
  MapWords mapNewDictTemp;
  auto inserterNewDictTemp = std::inserter(mapNewDictTemp, mapNewDictTemp.end());
  std::transform(mapNewDict.begin(), mapNewDict.end(), inserterNewDictTemp, mergeWordsBound);

  Dictionary dictNew{ dictNameNew, dictLangNew, mapNewDictTemp };
  dictionaries[dictNameNew] = dictNew;
  out << "Dictionaries " << dictName1 << " and " << dictName2 << " were intersected to " << dictNameNew;
}

void holodilov::exportAlphabet(std::istream& in, std::ostream& out, const MapDicts& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string filename;
  in >> filename;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if (dictionaries.find(dictName) == dictionaries.end())
  {
    throw std::logic_error("Error: dictionary not found.");
  }

  Alphabet alphabet(dictionaries.at(dictName));
  std::ofstream fos;
  fos.open(filename);
  if (!fos.is_open())
  {
    throw std::logic_error("Error: unable to create alphabet file.");
  }
  fos << alphabet;
  if (!fos)
  {
    throw std::logic_error("Error: unable to create alphabet file.");
  }

  out << "Alphabet of " << dictName << " dictionary was exported into file " << filename;
}

void holodilov::checkAlphabet(std::istream& in, std::ostream& out, const MapDicts& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  std::string filename;
  in >> filename;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.");
  }

  if (dictionaries.find(dictName) == dictionaries.end())
  {
    throw std::logic_error("Error: dictionary not found.");
  }

  std::ifstream fis;
  fis.open(filename);
  if (!fis.is_open())
  {
    throw std::logic_error("Error: alphabet file not found.");
  }
  Alphabet alphabetFromFile(fis);
  if (fis.fail() || fis.bad())
  {
    throw std::logic_error("Error: unable to read alphabet file.");
  }

  Alphabet alphabetFromDict(dictionaries.at(dictName));
  ScopeGuard scopeGuard(out);
  out << std::boolalpha;
  out << (alphabetFromFile == alphabetFromDict);
}

void holodilov::printDictNames(std::ostream& out, const MapDicts& dictionaries)
{
  if (dictionaries.empty())
  {
    out << "There are no dictionaries yet.";
    return;
  }

  using ostreamIter = std::ostream_iterator< std::string >;
  std::vector< std::string > dictNames(dictionaries.size());
  std::transform(dictionaries.begin(), dictionaries.end(), dictNames.begin(), dictToDictName);
  std::copy(dictNames.begin(), dictNames.end(), ostreamIter(out, "\n"));
}
