#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <functional>
#include <vector>
#include <string>
#include <map>

#include "TextProcessor.hpp"

namespace crossref
{

  class CommandHandler
  {
  public:
    using ArgsType = std::vector< std::string >;
    using Handler = std::function< void(TextProcessor &, const ArgsType &) >;
    using CommandMap = std::map< std::string, Handler >;

    static void handleLoadFile(TextProcessor &tp, const ArgsType &args);
    static void handleShowText(TextProcessor &tp, const ArgsType &args);
    static void handleDeleteText(TextProcessor &tp, const ArgsType &args);

    static void handleBuildDict(TextProcessor &tp, const ArgsType &args);
    static void handleShowDict(TextProcessor &tp, const ArgsType &args);
    static void handleImportDict(TextProcessor &tp, const ArgsType &args);
    static void handleExportDict(TextProcessor &tp, const ArgsType &args);
    static void handleXrefToText(TextProcessor &tp, const ArgsType &args);
    static void handleDeleteDict(TextProcessor &tp, const ArgsType &args);

    static void handleFindCommonLines(TextProcessor &tp, const ArgsType &args);
    static void handleConcatTexts(TextProcessor &tp, const ArgsType &args);
    static void handleExtractLines(TextProcessor &tp, const ArgsType &args);
    static void handleReplaceWords(TextProcessor &tp, const ArgsType &args);
    static void handleExtractHeaders(TextProcessor &tp, const ArgsType &args);
    static void handleDuplicateTextSection(TextProcessor &tp, const ArgsType &args);

    static void handleListTexts(TextProcessor &tp, const ArgsType &);
    static void handleListDicts(TextProcessor &tp, const ArgsType &);
    static void handleClearAllData(TextProcessor &tp, const ArgsType &args);
    static void handleExit(TextProcessor &, const ArgsType &);
    static void handleHelp(TextProcessor &, const ArgsType &);

    static CommandMap createCommandMap();
  };

}
#endif
