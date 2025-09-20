#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP
#include <string>
#include <vector>
#include <map>
#include "book.hpp"

namespace guseynov {
  class CommandHandler {
  private:
    std::map<std::string, Book> books;
    void printHelp() const;
    std::vector<std::string> splitString(const std::string& str, char delimiter) const;
    std::string extractQuotedText(const std::string& input, size_t& pos) const;
  public:
    CommandHandler() = default;
    void executeCommand(const std::vector<std::string>& args);
  };
}
#endif
