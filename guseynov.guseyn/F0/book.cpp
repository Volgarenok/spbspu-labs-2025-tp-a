#include "book.hpp"
#include <algorithm>
#include <cctype>

namespace guseynov {
    bool Book::addRecord(const std::string& key, const std::string& record, const std::vector<std::string>& colors) {
        if (key.empty() || record.empty() || colors.empty()) {
            return false;
        }
        Record new_record;
        new_record.text = record;
        new_record.colors.insert(colors.begin(), colors.end());
        records_[key] = new_record;
        return true;
    }

    bool Book::rewriteRecord(const std::string& key, const std::string& new_record) {
        auto it = records_.find(key);
        if (it == records_.end()) {
            return false;
        }
        it->second.text = new_record;
        return true;
    }

    bool Book::editTags(const std::string& key, const std::string& mode, const std::vector<std::string>& colors) {
        auto it = records_.find(key);
        if (it == records_.end()) {
            return false;
        }
        if (mode == "SET") {
            it->second.colors.clear();
            it->second.colors.insert(colors.begin(), colors.end());
        } else if (mode == "ADD") {
            it->second.colors.insert(colors.begin(), colors.end());
        } else if (mode == "DEL") {
            for (const auto& color : colors) {
                it->second.colors.erase(color);
            }
        } else {
            return false;
        }
        return true;
    }

    Record* Book::findRecord(const std::string& key) {
        auto it = records_.find(key);
        return it != records_.end() ? &it->second : nullptr;
    }

    std::vector<std::pair<std::string, Record>> Book::findRecordsByWord(const std::string& word) const {
        std::vector<std::pair<std::string, Record>> result;
        for (const auto& entry : records_) {
            if (entry.second.text.find(word) != std::string::npos) {
                result.push_back(std::make_pair(entry.first, entry.second));
            }
        }
        return result;
    }

    std::vector<std::pair<std::string, Record>> Book::findRecordsByTags(const std::string& logic, const std::vector<std::string>& colors) const {
        std::vector<std::pair<std::string, Record>> result;
        for (const auto& entry : records_) {
            const std::string& key = entry.first;
            const Record& record = entry.second;
            if (logic == "AND") {
                bool has_all = true;
                for (const auto& color : colors) {
                    if (record.colors.find(color) == record.colors.end()) {
                        has_all = false;
                        break;
                    }
                }
                if (has_all) {
                    result.push_back(std::make_pair(key, record));
                }
            } else if (logic == "OR") {
                for (const auto& color : colors) {
                    if (record.colors.find(color) != record.colors.end()) {
                        result.push_back(std::make_pair(key, record));
                        break;
                    }
                }
            }
        }
        return result;
    }

    bool Book::deleteRecord(const std::string& key) {
        return records_.erase(key) > 0;
    }

    std::vector<std::pair<std::string, std::set<std::string>>> Book::getAllRecords() const {
        std::vector<std::pair<std::string, std::set<std::string>>> result;
        for (const auto& entry : records_) {
            result.push_back(std::make_pair(entry.first, entry.second.colors));
        }
        return result;
    }

    void Book::replaceTag(const std::string& old_color, const std::string& new_color) {
        for (auto& entry : records_) {
            auto it = entry.second.colors.find(old_color);
            if (it != entry.second.colors.end()) {
                entry.second.colors.erase(it);
                entry.second.colors.insert(new_color);
            }
        }
    }

    std::vector<std::pair<std::string, Record>> Book::findRecordsWithAnyTags(const std::vector<std::string>& colors) const {
        std::vector<std::pair<std::string, Record>> result;
        for (const auto& entry : records_) {
            const std::string& key = entry.first;
            const Record& record = entry.second;
            for (const auto& color : colors) {
                if (record.colors.find(color) != record.colors.end()) {
                    result.push_back(std::make_pair(key, record));
                    break;
                }
            }
        }
        return result;
    }

    bool Book::deleteRecordsByTags(const std::string& logic, const std::vector<std::string>& colors) {
        std::vector<std::string> keys_to_delete;
        for (const auto& entry : records_) {
            const std::string& key = entry.first;
            const Record& record = entry.second;
            if (logic == "AND") {
                bool has_all = true;
                for (const auto& color : colors) {
                    if (record.colors.find(color) == record.colors.end()) {
                        has_all = false;
                        break;
                    }
                }
                if (has_all) {
                    keys_to_delete.push_back(key);
                }
            } else if (logic == "OR") {
                for (const auto& color : colors) {
                    if (record.colors.find(color) != record.colors.end()) {
                        keys_to_delete.push_back(key);
                        break;
                    }
                }
            }
        }
        for (const auto& key : keys_to_delete) {
            records_.erase(key);
        }
        return !keys_to_delete.empty();
    }

    size_t Book::getRecordCount() const {
        return records_.size();
    }

    bool BookSystem::bookExists(const std::string& book_name) const {
        return books_.find(book_name) != books_.end();
    }

    bool BookSystem::isValidColor(const std::string& color) const {
        return color == "red" || color == "blue" || color == "green";
    }

    bool BookSystem::containsWord(const std::string& text, const std::string& word) const {
        size_t pos = 0;
        while ((pos = text.find(word, pos)) != std::string::npos) {
            if ((pos == 0 || !std::isalnum(text[pos - 1])) && (pos + word.length() == text.length() || !std::isalnum(text[pos + word.length()]))) {
                return true;
            }
            pos += word.length();
        }
        return false;
    }

    bool BookSystem::addBook(const std::string& book_name) {
        if (book_name.empty() || bookExists(book_name)) {
            return false;
        }
        books_[book_name] = Book();
        return true;
    }

    bool BookSystem::addRecord(const std::string& book_name, const std::string& key, const std::string& record, const std::vector<std::string>& colors) {
        if (!bookExists(book_name) || key.empty() || record.empty() || colors.empty()) {
            return false;
        }
        for (const auto& color : colors) {
            if (!isValidColor(color)) {
                return false;
            }
        }
        return books_[book_name].addRecord(key, record, colors);
    }

    bool BookSystem::rewriteRecord(const std::string& book_name, const std::string& key, const std::string& new_record) {
        if (!bookExists(book_name) || key.empty() || new_record.empty()) {
            return false;
        }
        return books_[book_name].rewriteRecord(key, new_record);
    }

    std::vector<std::pair<std::string, std::string>> BookSystem::findRecords(const std::string& book_name, const std::string& word) {
        std::vector<std::pair<std::string, std::string>> result;
        if (!bookExists(book_name) || word.empty()) {
            return result;
        }
        auto records = books_[book_name].findRecordsByWord(word);
        for (const auto& entry : records) {
            result.push_back(std::make_pair(entry.first, entry.second.text));
        }
        return result;
    }

    std::vector<std::pair<std::string, std::string>> BookSystem::findRecordsByTags(const std::string& book_name, const std::string& logic, const std::vector<std::string>& colors) {
        std::vector<std::pair<std::string, std::string>> result;
        if (!bookExists(book_name) || (logic != "AND" && logic != "OR") || colors.empty()) {
            return result;
        }
        for (const auto& color : colors) {
            if (!isValidColor(color)) {
                return result;
            }
        }
        auto records = books_[book_name].findRecordsByTags(logic, colors);
        for (const auto& entry : records) {
            result.push_back(std::make_pair(entry.first, entry.second.text));
        }
        return result;
    }

    std::pair<std::string, std::set<std::string>> BookSystem::readRecord(const std::string& book_name, const std::string& key) {
        if (!bookExists(book_name) || key.empty()) {
            return std::make_pair("", std::set<std::string>());
        }
        auto record = books_[book_name].findRecord(key);
        if (!record) {
            return std::make_pair("", std::set<std::string>());
        }
        return std::make_pair(record->text, record->colors);
    }

    std::vector<std::pair<std::string, std::set<std::string>>> BookSystem::readBook(const std::string& book_name) {
        if (!bookExists(book_name)) {
            return std::vector<std::pair<std::string, std::set<std::string>>>();
        }
        return books_[book_name].getAllRecords();
    }

    bool BookSystem::combineBooks(const std::string& book_name_1, const std::string& book_name_2, const std::string& result_book_name) {
        if (!bookExists(book_name_1) || !bookExists(book_name_2) || bookExists(result_book_name) || result_book_name.empty()) {
            return false;
        }
        books_[result_book_name] = Book();
        auto records1 = books_[book_name_1].getAllRecords();
        for (const auto& entry : records1) {
            auto record_data = readRecord(book_name_1, entry.first);
            std::vector<std::string> colors_vec(record_data.second.begin(), record_data.second.end());
            books_[result_book_name].addRecord(entry.first, record_data.first, colors_vec);
        }
        auto records2 = books_[book_name_2].getAllRecords();
        for (const auto& entry : records2) {
            auto record_data = readRecord(book_name_2, entry.first);
            std::vector<std::string> colors_vec(record_data.second.begin(), record_data.second.end());
            if (books_[result_book_name].findRecord(entry.first)) {
                books_[result_book_name].addRecord(entry.first, record_data.first, colors_vec);
            } else {
                books_[result_book_name].addRecord(entry.first, record_data.first, colors_vec);
            }
        }
        return true;
    }

    bool BookSystem::moveTags(const std::string& source_book, const std::string& target_book, const std::vector<std::string>& colors) {
        if (!bookExists(source_book) || !bookExists(target_book) || colors.empty()) {
            return false;
        }
        for (const auto& color : colors) {
            if (!isValidColor(color)) {
                return false;
            }
        }
        auto records_to_move = books_[source_book].findRecordsWithAnyTags(colors);
        for (const auto& entry : records_to_move) {
            std::vector<std::string> colors_vec(entry.second.colors.begin(), entry.second.colors.end());
            books_[target_book].addRecord(entry.first, entry.second.text, colors_vec);
            books_[source_book].deleteRecord(entry.first);
        }
        return true;
    }

    bool BookSystem::editTags(const std::string& book_name, const std::string& key, const std::string& mode, const std::vector<std::string>& colors) {
        if (!bookExists(book_name) || key.empty() || (mode != "SET" && mode != "ADD" && mode != "DEL") || colors.empty()) {
            return false;
        }
        for (const auto& color : colors) {
            if (!isValidColor(color)) {
                return false;
            }
        }
        return books_[book_name].editTags(key, mode, colors);
    }

    bool BookSystem::deleteByTags(const std::string& book_name, const std::string& logic, const std::vector<std::string>& colors) {
        if (!bookExists(book_name) || (logic != "AND" && logic != "OR") || colors.empty()) {
            return false;
        }
        for (const auto& color : colors) {
            if (!isValidColor(color)) {
                return false;
            }
        }
        return books_[book_name].deleteRecordsByTags(logic, colors);
    }

    bool BookSystem::replaceTags(const std::string& book_name, const std::string& old_color, const std::string& new_color) {
        if (!bookExists(book_name) || !isValidColor(old_color) || !isValidColor(new_color)) {
            return false;
        }
        books_[book_name].replaceTag(old_color, new_color);
        return true;
    }

    std::vector<std::pair<std::string, std::pair<std::string, std::set<std::string>>>> BookSystem::listAllWith(const std::string& key) {
        std::vector<std::pair<std::string, std::pair<std::string, std::set<std::string>>>> result;
        if (key.empty()) {
            return result;
        }
        for (const auto& book_entry : books_) {
            const std::string& book_name = book_entry.first;
            const Book& book = book_entry.second;
            auto records = book.getAllRecords();
            for (const auto& record_entry : records) {
                if (record_entry.second.find(key) != record_entry.second.end()) {
                    auto record_data = readRecord(book_name, record_entry.first);
                    result.push_back(std::make_pair(book_name, std::make_pair(record_entry.first, record_data.second)));
                }
            }
        }
        return result;
    }

    bool BookSystem::deleteBook(const std::string& book_name) {
        if (!bookExists(book_name)) {
            return false;
        }
        books_.erase(book_name);
        return true;
    }

    bool BookSystem::deleteRecord(const std::string& book_name, const std::string& key) {
        if (!bookExists(book_name) || key.empty()) {
            return false;
        }
        return books_[book_name].deleteRecord(key);
    }

    std::vector<std::pair<std::string, std::pair<std::string, std::set<std::string>>>> BookSystem::listAllWithColor(const std::string& color) {
        return listAllWith(color);
    }

    std::vector<std::string> BookSystem::getAllBookNames() const {
        std::vector<std::string> names;
        for (const auto& entry : books_) {
            names.push_back(entry.first);
        }
        return names;
    }
}
