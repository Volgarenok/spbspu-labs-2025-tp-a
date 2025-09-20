#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "dictionary.hpp"
#include <map>
#include <string>
#include <iostream>

namespace smirnov {

    using dictionaries = std::map<std::string, Dictionary>;

    void createemptydict(dictionaries& dicts, std::istream& in);
    void insertword(dictionaries& dicts, std::istream& in);
    void insertfile(dictionaries& dicts, std::istream& in);
    void deleteword(dictionaries& dicts, std::istream& in);
    void clear(dictionaries& dicts, std::istream& in);
    void merge(dictionaries& dicts, std::istream& in);
    void intersect(dictionaries& dicts, std::istream& in);
    void deleteDict(dictionaries& dicts, std::istream& in);

    void getfrequency(dictionaries& dicts, std::istream& in, std::ostream& out);
    void mostfrequent(dictionaries& dicts, std::istream& in, std::ostream& out);
    void printsorted(dictionaries& dicts, std::istream& in, std::ostream& out);
    void printbyfrequency(dictionaries& dicts, std::istream& in, std::ostream& out);
    void size(dictionaries& dicts, std::istream& in, std::ostream& out);
    void topprelativefrequent(dictionaries& dicts, std::istream& in, std::ostream& out);
    void getrelativefrequency(dictionaries& dicts, std::istream& in, std::ostream& out);
    void frequencyrangerelative(dictionaries& dicts, std::istream& in, std::ostream& out);
    void bottomprelativefrequent(dictionaries& dicts, std::istream& in, std::ostream& out);
    void medianfrequency(dictionaries& dicts, std::istream& in, std::ostream& out);

    void printHelp();

}

#endif


