#include "book.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>

namespace guseynov {
  bool Book::addRecord(const std::string& key, const std::string& text, 
                      const std::vector<std::string>& tags) {
    if (tags.empty()) return false;
    Record newRecord;
    newRecord.text = text;
    for (size_t i = 0; i < tags.size(); i++) {
      newRecord.tags.insert(tags[i]);
    }
    records[key] = newRecord;
    return true;
  }
  bool Book::rewriteRecord(const std::string& key, const std::string& newText) {
    auto it = records.find(key);
    if (it == records.end()) return false;
    it->second.text = newText;
    return true;
  }
  bool Book::editTags(const std::string& key, const std::string& mode,
                     const std::vector<std::string>& tags) {
    auto it = records.find(key);
    if (it == records.end()) return false;
    if (mode == "SET") {
      it->second.tags.clear();
      for (size_t i = 0; i < tags.size(); i++) {
        it->second.tags.insert(tags[i]);
      }
    }
    else if (mode == "ADD") {
      for (size_t i = 0; i < tags.size(); i++) {
        it->second.tags.insert(tags[i]);
      }
    }
    else if (mode == "DEL") {
      for (size_t i = 0; i < tags.size(); i++) {
        it->second.tags.erase(tags[i]);
      }
    }
    return true;
  }
  bool Book::deleteRecord(const std::string& key) {
    return records.erase(key) > 0;
  }
  void Book::replaceTag(const std::string& oldTag, const std::string& newTag) {
    for (auto it = records.begin(); it != records.end(); it++) {
      if (it->second.tags.find(oldTag) != it->second.tags.end()) {
        it->second.tags.erase(oldTag);
        it->second.tags.insert(newTag);
      }
    }
  }
  Record* Book::getRecord(const std::string& key) {
    auto it = records.find(key);
    if (it == records.end()) return nullptr;
    return &it->second;
  }
  std::vector<std::string> Book::findRecordsByWord(const std::string& word) const {
    std::vector<std::string> result;
    for (auto it = records.begin(); it != records.end(); it++) {
      if (it->second.text.find(word) != std::string::npos) {
        result.push_back(it->first);
      }
    }
    return result;
  }
  std::vector<std::string> Book::findRecordsByTags(const std::string& logic,
                                                  const std::vector<std::string>& tags) const {
    std::vector<std::string> result;
    if (logic == "AND") {
      for (auto it = records.begin(); it != records.end(); it++) {
        bool hasAll = true;
        for (size_t i = 0; i < tags.size(); i++) {
          if (it->second.tags.find(tags[i]) == it->second.tags.end()) {
            hasAll = false;
            break;
          }
        }
        if (hasAll) {
          result.push_back(it->first);
        }
      }
    }
    else if (logic == "OR") {
      for (auto it = records.begin(); it != records.end(); it++) {
        for (size_t i = 0; i < tags.size(); i++) {
          if (it->second.tags.find(tags[i]) != it->second.tags.end()) {
            result.push_back(it->first);
            break;
          }
        }
      }
    }
    return result;
  }
  std::vector<std::string> Book::getAllRecords() const {
    std::vector<std::string> result;
    for (auto it = records.begin(); it != records.end(); it++) {
      result.push_back(it->first);
    }
    return result;
  }
  std::vector<std::string> Book::getRecordsWithTag(const std::string& tag) const {
    std::vector<std::string> result;
    for (auto it = records.begin(); it != records.end(); it++) {
      if (it->second.tags.find(tag) != it->second.tags.end()) {
        result.push_back(it->first);
      }
    }
    return result;
  }
  bool Book::hasRecord(const std::string& key) const {
    return records.find(key) != records.end();
  }
  size_t Book::size() const {
    return records.size();
  }
  void Book::combineWith(const Book& otherBook, const std::string& suffix) {
    for (auto it = otherBook.records.begin(); it != otherBook.records.end(); it++) {
      std::string newKey = it->first;
      if (records.find(newKey) != records.end()) {
        newKey += suffix;
      }
      records[newKey] = it->second;
    }
  }
}
