#include "commands.hpp"
#include <fstream>
#include <limits>
#include <algorithm>

void smirnov::printHelp() {
    std::cout << "Справка по командам:\n";
    std::cout << "1. createemptydict <dictname>\n";
    std::cout << "2. insertword <dictname> <word>\n";
    std::cout << "3. insertfile <dictname> <filename>\n";
    std::cout << "4. getfrequency <dictname> <word>\n";
    std::cout << "5. mostfrequent <dictname>\n";
    std::cout << "6. deleteword <dictname> <word>\n";
    std::cout << "7. clear <dictname>\n";
    std::cout << "8. merge <dict1> <dict2> <newdict>\n";
    std::cout << "9. intersect <dict1> <dict2> <newdict>\n";
    std::cout << "10. printsorted <dictname>\n";
    std::cout << "11. printbyfrequency <dictname>\n";
    std::cout << "12. size <dictname>\n";
    std::cout << "13. topprelativefrequent <dictname> <N>\n";
    std::cout << "14. getrelativefrequency <dictname> <word>\n";
    std::cout << "15. frequencyrangerelative <dictname> <min> <max>\n";
    std::cout << "16. bottomprelativefrequent <dictname> <N>\n";
    std::cout << "17. medianfrequency <dictname>\n";
    std::cout << "18. delete <dictname>\n";
}

void smirnov::createemptydict(dictionaries& dicts, std::istream& in) {
    std::string name;
    in >> name;
    if (dicts.find(name) != dicts.end()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    dicts[name] = Dictionary();
}

void smirnov::insertword(dictionaries& dicts, std::istream& in) {
    std::string dictname, word;
    in >> dictname >> word;
    if (dicts.find(dictname) == dicts.end()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    dicts[dictname].insertWord(word);
}

void smirnov::insertfile(dictionaries& dicts, std::istream& in) {
    std::string dictname, filename;
    in >> dictname >> filename;
    if (dicts.find(dictname) == dicts.cend()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    dicts[dictname].insertFromFile(filename);
}

void smirnov::deleteword(dictionaries& dicts, std::istream& in) {
    std::string dictname, word;
    in >> dictname >> word;
    if (dicts.find(dictname) == dicts.end() || !dicts[dictname].contains(word)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    dicts[dictname].deleteWord(word);
}

void smirnov::clear(dictionaries& dicts, std::istream& in) {
    std::string dictname;
    in >> dictname;
    if (dicts.find(dictname) == dicts.end()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    if (dicts[dictname].empty()) {
        std::cout << "<ALREADY EMPTY>\n";
        return;
    }
    dicts[dictname].clear();
}

    void smirnov::merge(dictionaries& dicts, std::istream& in) {
    std::string dict1, dict2, newdict;
    in >> dict1 >> dict2 >> newdict;

    if (!in || dicts.find(dict1) == dicts.end() || dicts.find(dict2) == dicts.end() || dicts.find(newdict) != dicts.end()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    const Dictionary& d1 = dicts[dict1];
    const Dictionary& d2 = dicts[dict2];

    if (d1.empty() && d2.empty()) {
        std::cout << "<EMPTY>\n";
        return;
    }

    Dictionary result;

    for (auto& p : d1.getSortedByWord()) {
        for (int i = 0; i < p.second; ++i)
            result.insertWord(p.first);
    }

    for (auto& p : d2.getSortedByWord()) {
        for (int i = 0; i < p.second; ++i)
            result.insertWord(p.first);
    }

    dicts[newdict] = result;
}

    void smirnov::intersect(dictionaries& dicts, std::istream& in) {
    std::string dict1, dict2, newdict;
    in >> dict1 >> dict2 >> newdict;

    if (!in || dicts.find(dict1) == dicts.end() || dicts.find(dict2) == dicts.end() || dicts.find(newdict) != dicts.end()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    const Dictionary& d1 = dicts[dict1];
    const Dictionary& d2 = dicts[dict2];

    Dictionary result;
    for (auto& p : d1.getSortedByWord()) {
        if (d2.contains(p.first)) {
            int freq = p.second + d2.getFrequency(p.first);
            for (int i = 0; i < freq; ++i)
                result.insertWord(p.first);
        }
    }

    if (result.empty()) {
        std::cout << "<EMPTY>\n";
        return;
    }

    dicts[newdict] = result;
}

    void smirnov::deleteDict(dictionaries& dicts, std::istream& in) {
    std::string dictname;
    in >> dictname;

    if (!in || dicts.find(dictname) == dicts.end()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    dicts.erase(dictname);
}

void smirnov::getfrequency(dictionaries& dicts, std::istream& in, std::ostream& out) {
    std::string dictname, word;
    in >> dictname >> word;
    if (dicts.find(dictname) == dicts.end()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    int f = dicts[dictname].getFrequency(word);
    if (f < 0) out << "<INVALID COMMAND>\n";
    else out << f << "\n";
}

void smirnov::mostfrequent(dictionaries& dicts, std::istream& in, std::ostream& out) {
    std::string dictname;
    in >> dictname;
    if (dicts.find(dictname) == dicts.end() || dicts[dictname].empty()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    out << dicts[dictname].mostFrequent() << "\n";
}

void smirnov::printsorted(dictionaries& dicts, std::istream& in, std::ostream& out) {
    std::string dictname;
    in >> dictname;
    if (!in || dicts.find(dictname) == dicts.end()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    for (auto& p : dicts[dictname].getSortedByWord()) {
        out << p.first << ": " << p.second << "\n";
    }
}

void smirnov::printbyfrequency(dictionaries& dicts, std::istream& in, std::ostream& out) {
    std::string dictname;
    in >> dictname;
    if (!in || dicts.find(dictname) == dicts.end()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    for (auto& p : dicts[dictname].getSortedByFrequency()) {
        out << p.first << ": " << p.second << "\n";
    }
}

void smirnov::size(dictionaries& dicts, std::istream& in, std::ostream& out) {
    std::string dictname;
    in >> dictname;
    if (!in || dicts.find(dictname) == dicts.end()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    out << dicts[dictname].size() << "\n";
}

void smirnov::topprelativefrequent(dictionaries& dicts, std::istream& in, std::ostream& out) {
    std::string dictname;
    int N;
    in >> dictname >> N;
    if (!in || dicts.find(dictname) == dicts.end()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    auto result = dicts[dictname].getTopRelative(N);
    if (result.empty()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    out << std::fixed << std::setprecision(6);
    for (auto& p : result) {
        out << p.first << ": " << p.second << "\n";
    }
}

void smirnov::getrelativefrequency(dictionaries& dicts, std::istream& in, std::ostream& out) {
    std::string dictname, word;
    in >> dictname >> word;
    if (!in || dicts.find(dictname) == dicts.end()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    double freq = dicts[dictname].getRelativeFrequency(word);
    if (freq < 0) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    out << std::fixed << std::setprecision(6);
    out << word << ": " << freq << "\n";
}

void smirnov::frequencyrangerelative(dictionaries& dicts, std::istream& in, std::ostream& out) {
    std::string dictname;
    double min, max;
    in >> dictname >> min >> max;
    if (!in || dicts.find(dictname) == dicts.end() || min > max) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    auto result = dicts[dictname].getRangeRelative(min, max);
    if (result.empty()) {
        out << "<EMPTY>\n";
        return;
    }
    out << std::fixed << std::setprecision(6);
    for (auto& p : result) {
        out << p.first << ": " << p.second << "\n";
    }
}

void smirnov::bottomprelativefrequent(dictionaries& dicts, std::istream& in, std::ostream& out) {
    std::string dictname;
    int N;
    in >> dictname >> N;
    if (!in || dicts.find(dictname) == dicts.end()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    auto result = dicts[dictname].getBottomRelative(N);
    if (result.empty()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    out << std::fixed << std::setprecision(6);
    for (auto& p : result) {
        out << p.first << ": " << p.second << "\n";
    }
}

void smirnov::medianfrequency(dictionaries& dicts, std::istream& in, std::ostream& out) {
    std::string dictname;
    in >> dictname;
    if (!in || dicts.find(dictname) == dicts.end()) {
        out << "<INVALID COMMAND>\n";
        return;
    }
    auto result = dicts[dictname].medianFrequency();
    if (dicts[dictname].empty()) {
        out << "<EMPTY>\n";
        return;
    }
    for (auto& w : result) {
        out << w << "\n";
    }
}


