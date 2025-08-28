#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <vector>
#include <string>
#include <map>

#include "HashTable.hpp"

namespace crossref
{
  class TextProcessor
  {

  public:
    TextProcessor() = default;

    void loadFile(const std::string &text_id, const std::string &filename);
    void showText(const std::string &text_id) const;
    void deleteText(const std::string &text_id);
    void concatTexts(const std::string &new_text_id, const std::string &text_id1, const std::string &text_id2);
    void extractLines(const std::string &new_text_id, const std::string &source_text_id, int start_line, int end_line);
    void replaceWords(const std::string &text_id, const std::string &old_word, const std::string &new_word);
    void extractHeaders(const std::string &new_text_id, const std::string &source_text_id, const std::string &pattern);
    void duplicateTextSection(
        const std::string &new_text_id, const std::string &source_text_id, int start_line, int end_line, int times);
    void listTexts() const;

    void buildDict(const std::string &dict_id, const std::string &text_id);
    void showDict(const std::string &dict_id) const;
    void importDict(const std::string &dict_id, const std::string &filename);
    void exportDict(const std::string &dict_id, const std::string &filename) const;
    void xrefToText(const std::string &new_text_id, const std::string &dict_id);
    void deleteDict(const std::string &dict_id);
    void listDicts() const;

    void findCommonLines(const std::string &new_text_id, const std::string &text_id1, const std::string &text_id2);
    void clearAll();

    static std::string sanitizeWord(std::string word);
    void processLine(HashTable &table, const std::string &line, int lineNumber) const;

  private:
    std::map< std::string, std::vector< std::string > > texts;
    std::map< std::string, HashTable > dicts;
    std::map< std::string, std::map< int, std::vector< std::string > > > dictWordOrder;

    static bool isNotAlpha(unsigned char c);
    static char toLower(char c);
  };

}

#endif
