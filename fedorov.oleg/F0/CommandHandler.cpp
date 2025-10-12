#include "CommandHandler.hpp"

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <map>
#include <cstring>

#include "TextProcessor.hpp"

namespace crossref
{

  namespace
  {
    struct UsageChecker
    {
      const char *usage;
      const CommandHandler::ArgsType &args;

      UsageChecker(const char *u, const CommandHandler::ArgsType &a):
        usage(u),
        args(a)
      {}

      void operator()() const
      {
        size_t required = std::count(usage, usage + std::strlen(usage), '<');
        if (args.size() < required)
        {
          throw std::runtime_error("Usage: " + std::string(usage));
        }
      }
    };
  }

  void CommandHandler::handleLoadFile(TextProcessor &tp, const ArgsType &args)
  {
    const char *loadTFUsage = "loadTextFile <text_id> <filename>";
    UsageChecker(loadTFUsage, args)();
    tp.loadFile(args[0], args[1]);
  }

  void CommandHandler::handleShowText(TextProcessor &tp, const ArgsType &args)
  {
    const char *showTUsage = "showText <text_id>";
    UsageChecker(showTUsage, args)();
    tp.showText(args[0]);
  }

  void CommandHandler::handleDeleteText(TextProcessor &tp, const ArgsType &args)
  {
    const char *deleteTUsage = "deleteText <text_id>";
    UsageChecker(deleteTUsage, args)();
    tp.deleteText(args[0]);
  }

  void CommandHandler::handleBuildDict(TextProcessor &tp, const ArgsType &args)
  {
    const char *buildDUsage = "buildDict <dict_id> <text_id>";
    UsageChecker(buildDUsage, args)();
    tp.buildDict(args[0], args[1]);
  }

  void CommandHandler::handleShowDict(TextProcessor &tp, const ArgsType &args)
  {
    const char *showDUsage = "showDict <dict_id>";
    UsageChecker(showDUsage, args)();
    tp.showDict(args[0]);
  }

  void CommandHandler::handleImportDict(TextProcessor &tp, const ArgsType &args)
  {
    const char *importDUsage = "importDict <dict_id> <filename>";
    UsageChecker(importDUsage, args)();
    tp.importDict(args[0], args[1]);
  }

  void CommandHandler::handleExportDict(TextProcessor &tp, const ArgsType &args)
  {
    const char *exportDUsage = "exportDict <dict_id> <filename>";
    UsageChecker(exportDUsage, args)();
    tp.exportDict(args[0], args[1]);
  }

  void CommandHandler::handleXrefToText(TextProcessor &tp, const ArgsType &args)
  {
    const char *xrefToTextUsage = "xrefToText <new_text_id> <dict_id>";
    UsageChecker(xrefToTextUsage, args)();
    tp.xrefToText(args[0], args[1]);
  }

  void CommandHandler::handleDeleteDict(TextProcessor &tp, const ArgsType &args)
  {
    const char *deleteDUsage = "deleteDict <dict_id>";
    UsageChecker(deleteDUsage, args)();
    tp.deleteDict(args[0]);
  }

  void CommandHandler::handleFindCommonLines(TextProcessor &tp, const ArgsType &args)
  {
    const char *findCLUsage = "findCommonLines <new_text_id> <text_id1> <text_id2>";
    UsageChecker(findCLUsage, args)();
    tp.findCommonLines(args[0], args[1], args[2]);
  }

  void CommandHandler::handleConcatTexts(TextProcessor &tp, const ArgsType &args)
  {
    const char *concatTUsage = "concatTexts <new_text_id> <text_id1> <text_id2>";
    UsageChecker(concatTUsage, args)();
    tp.concatTexts(args[0], args[1], args[2]);
  }

  void CommandHandler::handleExtractLines(TextProcessor &tp, const ArgsType &args)
  {
    const char *extractLUsage = "extractLines <new_text_id> <source_text_id> <start> <end>";
    UsageChecker(extractLUsage, args)();
    tp.extractLines(args[0], args[1], std::stoi(args[2]), std::stoi(args[3]));
  }

  void CommandHandler::handleReplaceWords(TextProcessor &tp, const ArgsType &args)
  {
    const char *replaceWUsage = "replaceWords <text_id> <old_word> <new_word>";
    UsageChecker(replaceWUsage, args)();
    tp.replaceWords(args[0], args[1], args[2]);
  }

  void CommandHandler::handleExtractHeaders(TextProcessor &tp, const ArgsType &args)
  {
    const char *extractHUsage = "extractHeaders <new_text_id> <source_text_id> <pattern>";
    UsageChecker(extractHUsage, args)();
    tp.extractHeaders(args[0], args[1], args[2]);
  }

  void CommandHandler::handleDuplicateTextSection(TextProcessor &tp, const ArgsType &args)
  {
    const char *duplicateTSUsage = "duplicateTextSection <new_text_id> <source_text_id> <start> <end> <times>";
    UsageChecker(duplicateTSUsage, args)();
    tp.duplicateTextSection(args[0], args[1], std::stoi(args[2]), std::stoi(args[3]), std::stoi(args[4]));
  }

  void CommandHandler::handleListTexts(TextProcessor &tp, const ArgsType &)
  {
    tp.listTexts();
  }

  void CommandHandler::handleListDicts(TextProcessor &tp, const ArgsType &)
  {
    tp.listDicts();
  }

  void CommandHandler::handleClearAllData(TextProcessor &tp, const ArgsType &)
  {
    tp.clearAll();
  }

  void CommandHandler::handleExit(TextProcessor &, const ArgsType &)
  {
    exit(0);
  }

  void CommandHandler::handleHelp(TextProcessor &, const ArgsType &)
  {
    std::cout << "Available commands:\n"
              << "  loadFile <text_id> <filename>    - Load text from file\n"
              << "  showText <text_id>                   - Show text with line numbers\n"
              << "  deleteText <text_id>                 - Delete text\n"
              << "  buildDict <dict_id> <text_id>        - Build dictionary from text\n"
              << "  showDict <dict_id>                   - Show dictionary contents\n"
              << "  importDict <dict_id> <filename>      - Import dictionary from file\n"
              << "  exportDict <dict_id> <filename>      - Export dictionary to file\n"
              << "  xrefToText <new_text_id> <dict_id>   - Generate text from xref\n"
              << "  deleteDict <dict_id>                 - Delete dictionary\n"
              << "  findCommonLines <new_text_id> <text_id1> <text_id2> - Find common lines\n"
              << "  concatTexts <new_text_id> <text_id1> <text_id2> - Concatenate texts\n"
              << "  extractLines <new_text_id> <source_text_id> <start> <end> - Extract lines\n"
              << "  replaceWords <text_id> <old_word> <new_word> - Replace words\n"
              << "  extractHeaders <new_text_id> <source_text_id> <pattern> - Extract headers\n"
              << "  duplicateTextSection <new_text_id> <source_text_id> <start> <end> <times> - Duplicate section\n"
              << "  listTexts                            - List all texts\n"
              << "  listDicts                            - List all dictionaries\n"
              << "  clearAll                             - Clear all data\n"
              << "  help                                 - Show help\n"
              << "  exit                                 - Exit program\n";
  }

  CommandHandler::CommandMap CommandHandler::createCommandMap()
  {
    return {{"loadFile", handleLoadFile},
            {"showText", handleShowText},
            {"deleteText", handleDeleteText},
            {"buildDict", handleBuildDict},
            {"showDict", handleShowDict},
            {"importDict", handleImportDict},
            {"exportDict", handleExportDict},
            {"xrefToText", handleXrefToText},
            {"deleteDict", handleDeleteDict},
            {"findCommonLines", handleFindCommonLines},
            {"concatTexts", handleConcatTexts},
            {"extractLines", handleExtractLines},
            {"replaceWords", handleReplaceWords},
            {"extractHeaders", handleExtractHeaders},
            {"duplicateTextSection", handleDuplicateTextSection},
            {"listTexts", handleListTexts},
            {"listDicts", handleListDicts},
            {"clearAll", handleClearAllData},
            {"exit", handleExit},
            {"help", handleHelp}};
  }

}
