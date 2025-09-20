#include <iostream>
#include <sstream>
#include <string>
#include "xref.hpp"

namespace
{
  void readTokens(std::istringstream& iss, std::string* tokens, size_t index, size_t max, size_t& count)
  {
    std::string word;
    if (!(iss >> word) || index >= max)
    {
      count = index;
      return;
    }
    tokens[index] = word;
    readTokens(iss, tokens, index + 1, max, count);
  }

  void processInput(amine::CrossRefSystem& xref)
  {
    std::string line;
    if (!std::getline(std::cin, line))
    {
      return;
    }

    std::istringstream iss(line);
    std::string tokens[8];
    size_t count = 0;

    readTokens(iss, tokens, 0, 8, count);

    if (count == 0)
    {
      processInput(xref);
      return;
    }

    const std::string& command = tokens[0];

    if (command == "buildIndex" && count == 3)
      xref.buildIndex(tokens[1], tokens[2]);
    else if (command == "deleteIndex" && count == 2)
      xref.deleteIndex(tokens[1]);
    else if (command == "containsWord" && count == 3)
      xref.containsWord(tokens[1], tokens[2]);
    else if (command == "printIndex" && count == 2)
      xref.printIndex(tokens[1]);
    else if (command == "getPositions" && count == 3)
      xref.getPositions(tokens[1], tokens[2]);
    else if (command == "mergeTexts" && count == 4)
      xref.mergeTexts(tokens[1], tokens[2], tokens[3]);
    else if (command == "insertText" && count == 6)
      xref.insertText(tokens[1], tokens[2], tokens[3], std::stoul(tokens[4]), std::stoul(tokens[5]));
    else if (command == "extractText" && count == 7)
      xref.extractText(tokens[1], tokens[2], std::stoul(tokens[3]), std::stoul(tokens[4]), std::stoul(tokens[5]),
                       std::stoul(tokens[6]));
    else if (command == "replaceWord" && count == 4)
      xref.replaceWord(tokens[1], tokens[2], tokens[3]);
    else if (command == "repeatText" && count == 4)
      xref.repeatText(tokens[1], tokens[2], std::stoul(tokens[3]));
    else if (command == "interleaveLines" && count == 4)
      xref.interleaveLines(tokens[1], tokens[2], tokens[3]);
    else if (command == "swapWords" && count == 4)
      xref.swapWords(tokens[1], tokens[2], tokens[3]);
    else if (command == "reverseText" && count == 3)
      xref.reverseText(tokens[1], tokens[2]);
    else if (command == "saveIndex" && count == 3)
      xref.saveIndex(tokens[1], tokens[2]);
    else if (command == "loadIndex" && count == 3)
      xref.loadIndex(tokens[1], tokens[2]);
    else if (command == "reconstructText" && count == 3)
      xref.reconstructText(tokens[1], tokens[2]);
    else
      std::cout << "<INVALID COMMAND>\n";

    processInput(xref);
  }
}

int main()
{
  amine::CrossRefSystem xref;
  processInput(xref);
  return 0;
}
