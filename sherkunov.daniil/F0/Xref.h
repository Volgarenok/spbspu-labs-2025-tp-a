#ifndef XREF_H
#define XREF_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <functional>
#include <cctype>
#include <limits>
#include <stdexcept>

namespace sherkunov {

class CrossReferenceSystem {
private:
    struct TextData {
        std::string content;
        std::map<std::string, std::vector<size_t>> references;
    };

    std::map<std::string, TextData> texts;

    bool isValidName(const std::string& name);
    bool isValidWord(const std::string& word);
    void buildReferences(const std::string& text_name, const std::string& content);

public:
    void build(const std::string& text_name, const std::string& text_content);
    void reconstruct(const std::string& text_name, const std::string& output_file = "");
    void concat(const std::string& new_name, const std::string& name1, const std::string& name2);
    std::vector<size_t> search(const std::string& text_name, const std::string& word);
    void replace(const std::string& text_name, const std::string& old_word, const std::string& new_word);
    void insert(const std::string& text_name, size_t position, const std::string& word);
    void remove(const std::string& text_name, size_t start, size_t end);
    void import(const std::string& filename);
    void export_text(const std::string& text_name, const std::string& filename);
    void stats(const std::string& text_name);
    void merge(const std::string& new_name, const std::string& name1, const std::string& name2);
};

int processCommandLineArguments(int argc, char** argv, CrossReferenceSystem& system);
void runInteractiveMode(CrossReferenceSystem& system);

}

#endif
