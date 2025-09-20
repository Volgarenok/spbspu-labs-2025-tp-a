#include <iostream>
#include <string>
#include <vector>
#include "command_handler.hpp"

int main() {
  guseynov::CommandHandler handler;
  std::string input;
  std::cout << "Book Management System\n";
  std::cout << "Type 'help' for available commands\n";
  std::cout << "Type 'exit' to quit\n\n";
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, input);
    if (input == "exit") {
      break;
    }
    if (input.empty()) {
      continue;
    }
    std::vector<std::string> args;
    size_t pos = 0;
    while (pos < input.length()) {
      if (input[pos] == '"') {
        std::string quoted = handler.extractQuotedText(input, pos);
        args.push_back(quoted);
      } else if (input[pos] != ' ') {
        size_t start = pos;
        while (pos < input.length() && input[pos] != ' ') {
          pos++;
        }
        args.push_back(input.substr(start, pos - start));
      } else {
        pos++;
      }
    }
    handler.executeCommand(args);
    std::cout << "\n";
  }
  return 0;
}
