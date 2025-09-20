#ifndef BOOK_HPP
#define BOOK_HPP
#include <string>
#include <vector>
#include <map>
#include <set>

namespace guseynov {
  struct Record {
    std::string text;
    std::set<std::string> tags;
  };

  class Book {
  private:
    std::map<std::string, Record> records;
  public:
    Book() = default;
    bool addRecord(const std::string& key, const std::string& text, 
                  const std::vector<std::string>& tags);
    bool rewriteRecord(const std::string& key, const std::string& newText);
    bool editTags(const std::string& key, const std::string& mode,
                 const std::vector<std::string>& tags);
    bool deleteRecord(const std::string& key);
    void replaceTag(const std::string& oldTag, const std::string& newTag);
    Record* getRecord(const std::string& key);
    std::vector<std::string> findRecordsByWord(const std::string& word) const;
    std::vector<std::string> findRecordsByTags(const std::string& logic,
                                              const std::vector<std::string>& tags) const;
    std::vector<std::string> getAllRecords() const;
    std::vector<std::string> getRecordsWithTag(const std::string& tag) const;
    bool hasRecord(const std::string& key) const;
    size_t size() const;
    void combineWith(const Book& otherBook, 
                    const std::string& suffix = "_2");
  };
}

#endif
