#include "commands.hpp"
#include <stdexcept>
#include <algorithm>

void trukhanov::CommandProcessor::createIndexCmd(const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid command");
  }
  createIndex(args[0], args[1]);
}

void trukhanov::CommandProcessor::showIndexCmd(const std::vector< std::string >& args)
{
  if (args.size() != 1)
  {
    throw std::invalid_argument("Invalid command");
  }
  showIndex(args[0]);
}

void trukhanov::CommandProcessor::searchWordCmd(const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid command");
  }
  searchWord(args[0], args[1]);
}

void trukhanov::CommandProcessor::saveIndexCmd(const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid command");
  }
  saveIndex(args[0], args[1]);
}

void trukhanov::CommandProcessor::replaceWordCmd(const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid command");
  }
  replaceWord(args[0], args[1], args[2]);
}

void trukhanov::CommandProcessor::exportWordCmd(const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid command");
  }
  exportWord(args[0], args[1], args[2]);
}

void trukhanov::CommandProcessor::mergeIndexesCmd(const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid command");
  }
  mergeIndexes(args[0], args[1], args[2]);
}

void trukhanov::CommandProcessor::mergeByLinesCmd(const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid command");
  }
  mergeByLines(args[0], args[1], args[2]);
}

void trukhanov::CommandProcessor::showFrequencyCmd(const std::vector< std::string >& args)
{
  if (args.size() != 1)
  {
    throw std::invalid_argument("Invalid command");
  }
  showFrequency(args[0]);
}

void trukhanov::CommandProcessor::listIndexesCmd(const std::vector< std::string >& args)
{
  if (!args.empty())
  {
    throw std::invalid_argument("Invalid command");
  }
  listIndexes();
}

void trukhanov::CommandProcessor::reconstructTextCmd(const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid command");
  }
  reconstructText(args[0], args[1]);
}

void trukhanov::CommandProcessor::compareIndexesCmd(const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid command");
  }
  compareIndexes(args[0], args[1]);
}

void trukhanov::CommandProcessor::longestWordsCmd(const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid command");
  }
  if (!std::all_of(args[1].begin(), args[1].end(), ::isdigit))
  {
    throw std::invalid_argument("Invalid command");
  }
  longestWords(args[0], std::stoi(args[1]));
}

void trukhanov::CommandProcessor::shortestWordsCmd(const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid command");
  }
  if (!std::all_of(args[1].begin(), args[1].end(), ::isdigit))
  {
    throw std::invalid_argument("Invalid command");
  }
  shortestWords(args[0], std::stoi(args[1]));
}

void trukhanov::CommandProcessor::clearIndexCmd(const std::vector< std::string >& args)
{
  if (args.size() != 1)
  {
    throw std::invalid_argument("Invalid command");
  }
  clearIndex(args[0]);
}

void trukhanov::CommandProcessor::filterLinesCmd(const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid command");
  }
  std::size_t from = std::stoull(args[1]);
  std::size_t to = std::stoull(args[2]);
  filterLines(args[0], from, to);
}
