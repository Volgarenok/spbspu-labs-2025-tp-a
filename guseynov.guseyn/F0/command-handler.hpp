#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include "book.hpp"
#include <vector>
#include <string>

namespace guseynov {
    class CommandHandler {
    private:
        BookSystem book_system_;
        std::vector<std::string> parseCommand(const std::string& input);
        std::vector<std::string> extractQuotedText(const std::string& input);
        void printHelp();
    public:
        void processCommand(const std::string& command);
        void runInteractive();
    };
}

#endif
