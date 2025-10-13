#include "CommandHandler.h"
#include "Commands.h"

namespace voronina
{
  CommandHandler::CommandHandler()
  {
    using namespace std::placeholders;

    cmds_["GETCODES"] = std::bind(getCodes, std::ref(vectorOfTables_), _1, _2);
    cmds_["ENCODE"] = std::bind(encode, std::ref(vectorOfTables_), _1, _2);
    cmds_["DECODE"] = std::bind(decode, std::cref(vectorOfTables_), _1, _2);
    cmds_["PRINTINFO"] = std::bind(printInfo, std::cref(vectorOfTables_), _1, _2);
    cmds_["COMPARE"] = std::bind(compare, std::cref(vectorOfTables_), _1, _2);
    cmds_["ENTROPY"] = std::bind(entropy, _1, _2);
    cmds_["ORIGIN"] = std::bind(origin, std::cref(vectorOfTables_), _1, _2);
    cmds_["LISTCODES"] = std::bind(listCodes, std::cref(vectorOfTables_), _1, _2);
    cmds_["CODESINFO"] = std::bind(codesInfo, std::cref(vectorOfTables_), _1, _2);
    cmds_["ABLECODING"] = std::bind(ableCoding, std::cref(vectorOfTables_), _1, _2);
    cmds_["VISUALIZE"] = std::bind(visualize, std::cref(vectorOfTables_), _1, _2);
    cmds_["DEFINITE_ENCODE"] = std::bind(definiteEncode, std::cref(vectorOfTables_), _1, _2);
  }

  void CommandHandler::executeCommand(const std::string& command, std::istream& input,
                                      std::ostream& output)
  {
    auto it = cmds_.find(command);
    if (it != cmds_.end())
    {
      try
      {
        it->second(input, output);
      }
      catch (const std::exception& ex)
      {
        output << "Ошибка: " << ex.what() << std::endl;
      }
    }
    else
    {
      output << "Неизвестная команда: " << command << std::endl;
    }
  }
}
