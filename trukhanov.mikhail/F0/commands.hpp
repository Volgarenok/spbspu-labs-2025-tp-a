#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iosfwd>
#include <functional>
#include "index.hpp"

namespace trukhanov
{
  class CommandProcessor
  {
  public:
    explicit CommandProcessor(std::ostream& output);

    void execute(const std::string& line);

  private:
    IndexStorage storage_;
    std::ostream& out_;
    std::map< std::string, ConcordanceIndex > indexes_;
    std::map< std::string, std::function< void(const std::vector< std::string >&) > > commands;

    void initializeCommands();
    void createIndex(const std::string& indexName, const std::string& filename);
    void searchWord(const std::string& indexName, const std::string& word);
    void showIndex(const std::string& indexName);
    void clearIndex(const std::string& indexName);
    void showFrequency(const std::string& indexName);
    void saveIndex(const std::string& indexName, const std::string& filename);
    void exportWord(const std::string& indexName, const std::string& word, const std::string& filename);
    void filterLines(const std::string& indexName, std::size_t fromLine, std::size_t toLine);
    void replaceWord(const std::string& indexName, const std::string& oldWord, const std::string& newWord);
    void listIndexes();
    void compareIndexes(const std::string& index1, const std::string& index2);
    void longestWords(const std::string& indexName, std::size_t count);
    void shortestWords(const std::string& indexName, std::size_t count);
    void mergeIndexes(const std::string& index1, const std::string& index2, const std::string& newIndex);
    void reconstructText(const std::string& indexName, const std::string& filename);
    void mergeByLines(const std::string& index1, const std::string& index2, const std::string& newIndex);

    void createIndexCmd(const std::vector< std::string >& args);
    void showIndexCmd(const std::vector< std::string >& args);
    void searchWordCmd(const std::vector< std::string >& args);
    void saveIndexCmd(const std::vector< std::string >& args);
    void replaceWordCmd(const std::vector< std::string >& args);
    void exportWordCmd(const std::vector< std::string >& args);
    void mergeIndexesCmd(const std::vector< std::string >& args);
    void mergeByLinesCmd(const std::vector< std::string >& args);
    void showFrequencyCmd(const std::vector< std::string >& args);
    void listIndexesCmd(const std::vector< std::string >& args);
    void reconstructTextCmd(const std::vector< std::string >& args);
    void compareIndexesCmd(const std::vector< std::string >& args);
    void longestWordsCmd(const std::vector< std::string >& args);
    void shortestWordsCmd(const std::vector< std::string >& args);
    void clearIndexCmd(const std::vector< std::string >& args);
    void filterLinesCmd(const std::vector< std::string >& args);
  };
}

#endif
