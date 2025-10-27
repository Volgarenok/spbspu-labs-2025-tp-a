#ifndef XREF_H
#define XREF_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>

namespace sherkunov
{

  struct TextData
  {
    std::string content;
    std::map< std::string, std::vector< size_t > > references;
  };

  class CrossReferenceSystem
  {
  public:
    bool isValidName(const std::string& name);
    bool isValidWord(const std::string& word);
    void buildReferences(const std::string& text_name, const std::string& content);

    void build(const std::string& text_name, const std::string& text_content);
    void reconstruct(const std::string& text_name, const std::string& output_file = "");
    void concat(const std::string& new_name, const std::string& name1, const std::string& name2);
    std::vector< size_t > search(const std::string& text_name, const std::string& word);
    void replace(const std::string& text_name, const std::string& old_word, const std::string& new_word);
    void insert(const std::string& text_name, size_t position, const std::string& word);
    void remove(const std::string& text_name, size_t start, size_t end);
    void import_text(const std::string& filename);
    void export_text(const std::string& text_name, const std::string& filename);
    void stats(const std::string& text_name);
    void merge(const std::string& new_name, const std::string& name1, const std::string& name2);

  private:
    std::map< std::string, TextData > texts;
    std::vector< std::string > splitIntoWords(const std::string& content);
    std::string joinWords(const std::vector< std::string >& words);
  };

  int processCommandLineArguments(int argc, char** argv, CrossReferenceSystem& system);
  void runInteractiveMode(CrossReferenceSystem& system);

}

#endif
