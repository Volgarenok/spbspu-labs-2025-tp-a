#include "command-handler.hpp"
#include <iostream>
#include <algorithm>

namespace guseynov {
    std::vector<std::string> CommandHandler::parseCommand(const std::string& input) {
        std::vector<std::string> tokens;
        std::string current_token;
        bool in_quotes = false;
        for (size_t i = 0; i < input.length(); ++i) {
            char c = input[i];
            if (c == '"') {
                in_quotes = !in_quotes;
                if (!in_quotes && !current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token.clear();
                }
            } else if (std::isspace(c) && !in_quotes) {
                if (!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token.clear();
                }
            } else {
                current_token += c;
            }
        }
        if (!current_token.empty()) {
            tokens.push_back(current_token);
        }
        return tokens;
    }

    std::vector<std::string> CommandHandler::extractQuotedText(const std::string& input) {
        std::vector<std::string> quoted_texts;
        size_t start = 0;
        while ((start = input.find('"', start)) != std::string::npos) {
            size_t end = input.find('"', start + 1);
            if (end == std::string::npos) {
                break;
            }
            quoted_texts.push_back(input.substr(start + 1, end - start - 1));
            start = end + 1;
        }
        return quoted_texts;
    }

    void CommandHandler::printHelp() {
        std::cout << "Available commands:\n"
                  << "1. addbook <book_name>\n"
                  << "2. add <book_name> <key> \"<record>\" <color_1> [<color_2> ...]\n"
                  << "3. rewrite <book_name> <key> \"<new_record>\"\n"
                  << "4. find <book_name> <word>\n"
                  << "5. findbytags <book_name> <logic> <color_1> [<color_2> ...]\n"
                  << "6. read <book_name> <key>\n"
                  << "7. readbook <book_name>\n"
                  << "8. combinebooks <book_name_1> <book_name_2> <result_book_name>\n"
                  << "9. movetags <source_book> <target_book> <color_1> [<color_2> ...]\n"
                  << "10. edittags <book_name> <key> <mode> <color_1> [<color_2> ...]\n"
                  << "11. deletebytags <book_name> <logic> <color_1> [<color_2> ...]\n"
                  << "12. replacetags <book_name> <old_color> <new_color>\n"
                  << "13. listallwith <key>\n"
                  << "14. deletebook <book_name>\n"
                  << "15. delete <book_name> <key>\n"
                  << "16. listallwithcolor <color>\n"
                  << "17. help\n"
                  << "18. exit\n";
    }

    void CommandHandler::processCommand(const std::string& command) {
        if (command.empty()) {
            return;
        }
        auto tokens = parseCommand(command);
        if (tokens.empty()) {
            return;
        }
        std::string cmd = tokens[0];
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
        if (cmd == "addbook") {
            if (tokens.size() == 2) {
                if (book_system_.addBook(tokens[1])) {
                    std::cout << "Book '" << tokens[1] << "' created successfully.\n";
                } else {
                    std::cout << "Error: Failed to create book '" << tokens[1] << "'.\n";
                }
            } else {
                std::cout << "Error: Invalid number of arguments for addbook.\n";
            }
        } else if (cmd == "add") {
            if (tokens.size() >= 5) {
                auto quoted_texts = extractQuotedText(command);
                if (quoted_texts.empty()) {
                    std::cout << "Error: No quoted text found.\n";
                    return;
                }
                std::string record_text = quoted_texts[0];
                std::vector<std::string> colors(tokens.begin() + 4, tokens.end());
                if (book_system_.addRecord(tokens[1], tokens[2], record_text, colors)) {
                    std::cout << "Record added successfully to book '" << tokens[1] << "'.\n";
                } else {
                    std::cout << "Error: Failed to add record.\n";
                }
            } else {
                std::cout << "Error: Invalid number of arguments for add.\n";
            }
        } else if (cmd == "rewrite") {
            if (tokens.size() >= 4) {
                auto quoted_texts = extractQuotedText(command);
                if (quoted_texts.empty()) {
                    std::cout << "Error: No quoted text found.\n";
                    return;
                }
                if (book_system_.rewriteRecord(tokens[1], tokens[2], quoted_texts[0])) {
                    std::cout << "Record rewritten successfully.\n";
                } else {
                    std::cout << "Error: Failed to rewrite record.\n";
                }
            } else {
                std::cout << "Error: Invalid number of arguments for rewrite.\n";
            }
        } else if (cmd == "find") {
            if (tokens.size() == 3) {
                auto results = book_system_.findRecords(tokens[1], tokens[2]);
                if (results.empty()) {
                    std::cout << "No records found.\n";
                } else {
                    std::cout << "Found " << results.size() << " record(s):\n";
                    for (const auto& entry : results) {
                        std::cout << "Key: " << entry.first << ", Text: " << entry.second << "\n";
                    }
                }
            } else {
                std::cout << "Error: Invalid number of arguments for find.\n";
            }
        } else if (cmd == "findbytags") {
            if (tokens.size() >= 4) {
                std::vector<std::string> colors(tokens.begin() + 3, tokens.end());
                auto results = book_system_.findRecordsByTags(tokens[1], tokens[2], colors);
                if (results.empty()) {
                    std::cout << "No records found.\n";
                } else {
                    std::cout << "Found " << results.size() << " record(s):\n";
                    for (const auto& entry : results) {
                        std::cout << "Key: " << entry.first << ", Text: " << entry.second << "\n";
                    }
                }
            } else {
                std::cout << "Error: Invalid number of arguments for findbytags.\n";
            }
        } else if (cmd == "read") {
            if (tokens.size() == 3) {
                auto record = book_system_.readRecord(tokens[1], tokens[2]);
                if (record.first.empty()) {
                    std::cout << "Record not found.\n";
                } else {
                    std::cout << "Key: " << tokens[2] << "\n";
                    std::cout << "Text: " << record.first << "\n";
                    std::cout << "Tags: ";
                    for (const auto& tag : record.second) {
                        std::cout << tag << " ";
                    }
                    std::cout << "\n";
                }
            } else {
                std::cout << "Error: Invalid number of arguments for read.\n";
            }
        } else if (cmd == "readbook") {
            if (tokens.size() == 2) {
                auto records = book_system_.readBook(tokens[1]);
                if (records.empty()) {
                    std::cout << "Book is empty or not found.\n";
                } else {
                    std::cout << "Book '" << tokens[1] << "' contains " << records.size() << " record(s):\n";
                    for (const auto& entry : records) {
                        std::cout << "Key: " << entry.first << ", Tags: ";
                        for (const auto& tag : entry.second) {
                            std::cout << tag << " ";
                        }
                        std::cout << "\n";
                    }
                }
            } else {
                std::cout << "Error: Invalid number of arguments for readbook.\n";
            }
        } else if (cmd == "help") {
            printHelp();
        } else if (cmd == "exit") {
            std::cout << "Goodbye!\n";
            exit(0);
        } else {
            std::cout << "Error: Unknown command '" << cmd << "'. Type 'help' for available commands.\n";
        }
    }

    void CommandHandler::runInteractive() {
        std::cout << "Book System v1.0\n";
        std::cout << "Type 'help' for available commands or 'exit' to quit.\n";
        std::string input;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);
            if (input.empty()) {
                continue;
            }
            processCommand(input);
        }
    }
}
