#include "command-handler.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace guseynov {
  void CommandHandler::printHelp() const {
    std::cout << "Available commands:\n\n";
    std::cout << "1. addbook <book_name> - Create new empty book\n";
    std::cout << "2. add <book_name> <key> \"<record>\" <color_1> [<color_2> ...] - Add/update record\n";
    std::cout << "3. rewrite <book_name> <key> \"<new_record>\" - Change record text\n";
    std::cout << "4. find <book_name> <word> - Find records containing word\n";
    std::cout << "5. findbytags <book_name> <logic> <color_1> [<color_2> ...] - Find by tags (AND/OR)\n";
    std::cout << "6. read <book_name> <key> - Show record details\n";
    std::cout << "7. readbook <book_name> - Show all records in book\n";
    std::cout << "8. combinebooks <book_name_1> <book_name_2> <result_book_name> - Combine two books\n";
    std::cout << "9. movetags <source_book> <target_book> <color_1> [<color_2> ...] - Move records by tags\n";
    std::cout << "10. edittags <book_name> <key> <mode> <color_1> [<color_2> ...] - Edit tags (SET/ADD/DEL)\n";
    std::cout << "11. deletebytags <book_name> <logic> <color_1> [<color_2> ...] - Delete by tags\n";
    std::cout << "12. replacetags <book_name> <old_color> <new_color> - Replace tags in all records\n";
    std::cout << "13. listallwith <key> - Show books with records having tag\n";
    std::cout << "14. deletebook <book_name> - Delete book\n";
    std::cout << "15. delete <book_name> <key> - Delete record\n";
    std::cout << "help - Show this help message\n";
  }
  std::vector<std::string> CommandHandler::splitString(const std::string& str, char delimiter) const {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
      if (!token.empty()) {
        tokens.push_back(token);
      }
    }
    return tokens;
  }
  std::string CommandHandler::extractQuotedText(const std::string& input, size_t& pos) {
    if (pos >= input.length() || input[pos] != '"') return "";
    size_t start = pos + 1;
    size_t end = input.find('"', start);
    if (end == std::string::npos) {
      pos = input.length();
      return input.substr(start);
    }
    pos = end + 1;
    return input.substr(start, end - start);
  }
  void CommandHandler::executeCommand(const std::vector<std::string>& args) {
    if (args.empty()) return;
    std::string command = args[0];
    if (command == "help") {
      printHelp();
      return;
    }
    if (command == "addbook") {
      if (args.size() < 2) {
        std::cout << "Error: Missing book name\n";
        return;
      }
      std::string bookName = args[1];
      if (books.find(bookName) != books.end()) {
        std::cout << "Book '" << bookName << "' already exists\n";
      } else {
        books[bookName] = Book();
        std::cout << "Book '" << bookName << "' created successfully\n";
      }
      return;
    }
    if (command == "add") {
      if (args.size() < 5) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string bookName = args[1];
      std::string key = args[2];
      auto it = books.find(bookName);
      if (it == books.end()) {
        std::cout << "Error: Book '" << bookName << "' not found\n";
        return;
      }
      std::string recordText = args[3];
      std::vector<std::string> tags;
      for (size_t i = 4; i < args.size(); i++) {
        tags.push_back(args[i]);
      }
      if (it->second.addRecord(key, recordText, tags)) {
        std::cout << "Record added successfully\n";
      } else {
        std::cout << "Error: Failed to add record\n";
      }
      return;
    }
    if (command == "rewrite") {
      if (args.size() < 4) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string bookName = args[1];
      std::string key = args[2];
      std::string newText = args[3];
      auto it = books.find(bookName);
      if (it == books.end()) {
        std::cout << "Error: Book '" << bookName << "' not found\n";
        return;
      }
      if (it->second.rewriteRecord(key, newText)) {
        std::cout << "Record rewritten successfully\n";
      } else {
        std::cout << "Error: Record not found\n";
      }
      return;
    }
    if (command == "find") {
      if (args.size() < 3) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string bookName = args[1];
      std::string word = args[2];
      auto it = books.find(bookName);
      if (it == books.end()) {
        std::cout << "Error: Book '" << bookName << "' not found\n";
        return;
      }
      std::vector<std::string> foundRecords = it->second.findRecordsByWord(word);
      if (foundRecords.empty()) {
        std::cout << "No records found containing '" << word << "'\n";
      } else {
        std::cout << "Found records: ";
        for (size_t i = 0; i < foundRecords.size(); i++) {
          std::cout << foundRecords[i];
          if (i < foundRecords.size() - 1) std::cout << ", ";
        }
        std::cout << "\n";
      }
      return;
    }
    if (command == "findbytags") {
      if (args.size() < 4) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string bookName = args[1];
      std::string logic = args[2];
      if (logic != "AND" && logic != "OR") {
        std::cout << "Error: Logic must be AND or OR\n";
        return;
      }
      auto it = books.find(bookName);
      if (it == books.end()) {
        std::cout << "Error: Book '" << bookName << "' not found\n";
        return;
      }
      std::vector<std::string> tags;
      for (size_t i = 3; i < args.size(); i++) {
        tags.push_back(args[i]);
      }
      std::vector<std::string> foundRecords = it->second.findRecordsByTags(logic, tags);
      if (foundRecords.empty()) {
        std::cout << "No records found with specified tags\n";
      } else {
        std::cout << "Found records: ";
        for (size_t i = 0; i < foundRecords.size(); i++) {
          std::cout << foundRecords[i];
          if (i < foundRecords.size() - 1) std::cout << ", ";
        }
        std::cout << "\n";
      }
      return;
    }
    if (command == "read") {
      if (args.size() < 3) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string bookName = args[1];
      std::string key = args[2];
      auto it = books.find(bookName);
      if (it == books.end()) {
        std::cout << "Error: Book '" << bookName << "' not found\n";
        return;
      }
      Record* record = it->second.getRecord(key);
      if (!record) {
        std::cout << "Error: Record not found\n";
        return;
      }
      std::cout << "Key: " << key << "\n";
      std::cout << "Text: " << record->text << "\n";
      std::cout << "Tags: ";
      for (auto tagIt = record->tags.begin(); tagIt != record->tags.end(); tagIt++) {
        std::cout << *tagIt;
        if (std::next(tagIt) != record->tags.end()) std::cout << ", ";
      }
      std::cout << "\n";
      return;
    }
    if (command == "readbook") {
      if (args.size() < 2) {
        std::cout << "Error: Missing book name\n";
        return;
      }
      std::string bookName = args[1];
      auto it = books.find(bookName);
      if (it == books.end()) {
        std::cout << "Error: Book '" << bookName << "' not found\n";
        return;
      }
      std::vector<std::string> allRecords = it->second.getAllRecords();
      if (allRecords.empty()) {
        std::cout << "Book '" << bookName << "' is empty\n";
      } else {
        std::cout << "Records in book '" << bookName << "':\n";
        for (size_t i = 0; i < allRecords.size(); i++) {
          Record* record = it->second.getRecord(allRecords[i]);
          std::cout << "  " << allRecords[i] << " - Tags: ";
          for (auto tagIt = record->tags.begin(); tagIt != record->tags.end(); tagIt++) {
            std::cout << *tagIt;
            if (std::next(tagIt) != record->tags.end()) std::cout << ", ";
          }
          std::cout << "\n";
        }
      }
      return;
    }
    if (command == "combinebooks") {
      if (args.size() < 4) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string bookName1 = args[1];
      std::string bookName2 = args[2];
      std::string resultBookName = args[3];
      auto it1 = books.find(bookName1);
      auto it2 = books.find(bookName2);
      if (it1 == books.end()) {
        std::cout << "Error: Book '" << bookName1 << "' not found\n";
        return;
      }
      if (it2 == books.end()) {
        std::cout << "Error: Book '" << bookName2 << "' not found\n";
        return;
      }
      Book resultBook = it1->second;
      resultBook.combineWith(it2->second);
      books[resultBookName] = resultBook;
      std::cout << "Books combined successfully into '" << resultBookName << "'\n";
      return;
    }
    if (command == "movetags") {
      if (args.size() < 4) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string sourceBook = args[1];
      std::string targetBook = args[2];
      auto sourceIt = books.find(sourceBook);
      auto targetIt = books.find(targetBook);
      if (sourceIt == books.end()) {
        std::cout << "Error: Source book '" << sourceBook << "' not found\n";
        return;
      }
      if (targetIt == books.end()) {
        std::cout << "Error: Target book '" << targetBook << "' not found\n";
        return;
      }
      std::vector<std::string> tags;
      for (size_t i = 3; i < args.size(); i++) {
        tags.push_back(args[i]);
      }
      std::vector<std::string> recordsToMove = sourceIt->second.findRecordsByTags("OR", tags);
      for (size_t i = 0; i < recordsToMove.size(); i++) {
        Record* record = sourceIt->second.getRecord(recordsToMove[i]);
        if (record) {
          std::vector<std::string> recordTags(record->tags.begin(), record->tags.end());
          targetIt->second.addRecord(recordsToMove[i], record->text, recordTags);
          sourceIt->second.deleteRecord(recordsToMove[i]);
        }
      }
      std::cout << "Moved " << recordsToMove.size() << " records\n";
      return;
    }
    if (command == "edittags") {
      if (args.size() < 5) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string bookName = args[1];
      std::string key = args[2];
      std::string mode = args[3];
      if (mode != "SET" && mode != "ADD" && mode != "DEL") {
        std::cout << "Error: Mode must be SET, ADD, or DEL\n";
        return;
      }
      auto it = books.find(bookName);
      if (it == books.end()) {
        std::cout << "Error: Book '" << bookName << "' not found\n";
        return;
      }
      std::vector<std::string> tags;
      for (size_t i = 4; i < args.size(); i++) {
        tags.push_back(args[i]);
      }
      if (it->second.editTags(key, mode, tags)) {
        std::cout << "Tags updated successfully\n";
      } else {
        std::cout << "Error: Record not found\n";
      }
      return;
    }
    if (command == "deletebytags") {
      if (args.size() < 4) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string bookName = args[1];
      std::string logic = args[2];
      if (logic != "AND" && logic != "OR") {
        std::cout << "Error: Logic must be AND or OR\n";
        return;
      }
      auto it = books.find(bookName);
      if (it == books.end()) {
        std::cout << "Error: Book '" << bookName << "' not found\n";
        return;
      }
      std::vector<std::string> tags;
      for (size_t i = 3; i < args.size(); i++) {
        tags.push_back(args[i]);
      }
      std::vector<std::string> recordsToDelete = it->second.findRecordsByTags(logic, tags);
      for (size_t i = 0; i < recordsToDelete.size(); i++) {
        it->second.deleteRecord(recordsToDelete[i]);
      }
      std::cout << "Deleted " << recordsToDelete.size() << " records\n";
      return;
    }
    if (command == "replacetags") {
      if (args.size() < 4) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string bookName = args[1];
      std::string oldTag = args[2];
      std::string newTag = args[3];
      auto it = books.find(bookName);
      if (it == books.end()) {
        std::cout << "Error: Book '" << bookName << "' not found\n";
        return;
      }
      it->second.replaceTag(oldTag, newTag);
      std::cout << "Tags replaced successfully\n";
      return;
    }
    if (command == "listallwith") {
      if (args.size() < 2) {
        std::cout << "Error: Missing tag\n";
        return;
      }
      std::string tag = args[1];
      std::cout << "Books containing records with tag '" << tag << "':\n";
      bool found = false;
      for (auto it = books.begin(); it != books.end(); it++) {
        std::vector<std::string> recordsWithTag = it->second.getRecordsWithTag(tag);
        if (!recordsWithTag.empty()) {
          found = true;
          std::cout << "  " << it->first << " - Records: ";
          for (size_t i = 0; i < recordsWithTag.size(); i++) {
            std::cout << recordsWithTag[i];
            if (i < recordsWithTag.size() - 1) std::cout << ", ";
          }
          std::cout << "\n";
        }
      }
      if (!found) {
        std::cout << "No books found with records having tag '" << tag << "'\n";
      }
      return;
    }
    if (command == "deletebook") {
      if (args.size() < 2) {
        std::cout << "Error: Missing book name\n";
        return;
      }
      std::string bookName = args[1];
      if (books.erase(bookName) > 0) {
        std::cout << "Book '" << bookName << "' deleted successfully\n";
      } else {
        std::cout << "Error: Book '" << bookName << "' not found\n";
      }
      return;
    }
    if (command == "delete") {
      if (args.size() < 3) {
        std::cout << "Error: Not enough arguments\n";
        return;
      }
      std::string bookName = args[1];
      std::string key = args[2];
      auto it = books.find(bookName);
      if (it == books.end()) {
        std::cout << "Error: Book '" << bookName << "' not found\n";
        return;
      }
      if (it->second.deleteRecord(key)) {
        std::cout << "Record deleted successfully\n";
      } else {
        std::cout << "Error: Record not found\n";
      }
      return;
    }
    std::cout << "Error: Unknown command '" << command << "'\n";
    std::cout << "Type 'help' to see available commands\n";
  }
}
