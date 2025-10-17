#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>
#include <vector>
#include <map>
#include <set>

namespace guseynov {
    struct Record {
        std::string text;
        std::set<std::string> colors;
    };

    class Book {
    private:
        std::map<std::string, Record> records_;
    public:
        bool addRecord(const std::string& key, const std::string& record, const std::vector<std::string>& colors);
        bool rewriteRecord(const std::string& key, const std::string& new_record);
        bool editTags(const std::string& key, const std::string& mode, const std::vector<std::string>& colors);
        Record* findRecord(const std::string& key);
        std::vector<std::pair<std::string, Record>> findRecordsByWord(const std::string& word) const;
        std::vector<std::pair<std::string, Record>> findRecordsByTags(const std::string& logic, const std::vector<std::string>& colors) const;
        bool deleteRecord(const std::string& key);
        std::vector<std::pair<std::string, std::set<std::string>>> getAllRecords() const;
        void replaceTag(const std::string& old_color, const std::string& new_color);
        std::vector<std::pair<std::string, Record>> findRecordsWithAnyTags(const std::vector<std::string>& colors) const;
        bool deleteRecordsByTags(const std::string& logic, const std::vector<std::string>& colors);
        size_t getRecordCount() const;
    };

    class BookSystem {
    private:
        std::map<std::string, Book> books_;
        bool bookExists(const std::string& book_name) const;
        bool isValidColor(const std::string& color) const;
        bool containsWord(const std::string& text, const std::string& word) const;
    public:
        bool addBook(const std::string& book_name);
        bool addRecord(const std::string& book_name, const std::string& key, const std::string& record, const std::vector<std::string>& colors);
        bool rewriteRecord(const std::string& book_name, const std::string& key, const std::string& new_record);
        std::vector<std::pair<std::string, std::string>> findRecords(const std::string& book_name, const std::string& word);
        std::vector<std::pair<std::string, std::string>> findRecordsByTags(const std::string& book_name, const std::string& logic, const std::vector<std::string>& colors);
        std::pair<std::string, std::set<std::string>> readRecord(const std::string& book_name, const std::string& key);
        std::vector<std::pair<std::string, std::set<std::string>>> readBook(const std::string& book_name);
        bool combineBooks(const std::string& book_name_1, const std::string& book_name_2, const std::string& result_book_name);
        bool moveTags(const std::string& source_book, const std::string& target_book, const std::vector<std::string>& colors);
        bool editTags(const std::string& book_name, const std::string& key, const std::string& mode, const std::vector<std::string>& colors);
        bool deleteByTags(const std::string& book_name, const std::string& logic, const std::vector<std::string>& colors);
        bool replaceTags(const std::string& book_name, const std::string& old_color, const std::string& new_color);
        std::vector<std::pair<std::string, std::pair<std::string, std::set<std::string>>>> listAllWith(const std::string& key);
        bool deleteBook(const std::string& book_name);
        bool deleteRecord(const std::string& book_name, const std::string& key);
        std::vector<std::pair<std::string, std::pair<std::string, std::set<std::string>>>> listAllWithColor(const std::string& color);
        std::vector<std::string> getAllBookNames() const;
    };
}

#endif
