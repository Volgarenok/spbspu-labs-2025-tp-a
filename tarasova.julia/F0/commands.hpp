#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <map>
#include <list>
#include <fstream>
#include <istream>
#include <ostream>
#include <functional>

namespace tarasova
{
	using namespace std::placeholders;
	using Translations = std::list< std::string >;
	using Dictionary = std::map< std::string, Translations >;
	using Dictionaries = std::map< std::string, Dictionary >;

	void createDict(std::istream&, std::ostream&, Dictionaries&);
	void deleteDict(std::istream&, std::ostream&, Dictionaries&);
	void addWord(std::istream&, std::ostream&, Dictionaries&);
	void deleteWord(std::istream&, std::ostream&, Dictionaries&);
	void findWord(std::istream&, std::ostream&, const Dictionaries&);
	void addTranslate(std::istream&, std::ostream&, Dictionaries&);
	void deleteTranslation(std::istream&, std::ostream&, Dictionaries&);
	void findTranslate(std::istream&, std::ostream&, const Dictionaries&);
	void printDict(std::istream&, std::ostream&, const Dictionaries&);
	void clear(std::istream&, std::ostream&, Dictionaries&);
	void replaceWord(std::istream&, std::ostream&, Dictionaries&);
	void mergeDicts(std::istream&, std::ostream&, Dictionaries&);
	void delRepeatWords(std::istream&, std::ostream&, Dictionaries&);
}

#endif