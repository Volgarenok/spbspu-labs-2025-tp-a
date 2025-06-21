#include "utils.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include "commands.hpp"

namespace {
  void processCommand(const std::function< void(const std::vector< gavrilova::Polygon >&,
                          const std::vector< std::string >&, std::ostream&) >& func,
      const std::vector< gavrilova::Polygon >& polygons, const std::vector< std::string >& commands, std::ostream& os)
  {
    return func(polygons, commands, os);
  }
}

std::vector< std::string > gavrilova::tokenize(std::vector< std::string >& tokens, const std::string& str)
{
  std::istringstream iss(str);

  if (!tokens.empty()) {
    tokens.clear();
  }

  std::string token = {};
  while (iss >> token) {
    std::transform(
        token.begin(),
        token.end(),
        token.begin(),
        std::bind(
            std::toupper< char >,
            std::placeholders::_1,
            std::locale()));
    tokens.push_back(token);
  }
  return tokens;
}

void gavrilova::readFile(const std::string& filename, std::vector< Polygon >& polygons)
{
  if (filename.empty()) {
    throw std::runtime_error("Filename is empty");
  }

  std::ifstream input_file(filename);
  if (!input_file) {
    throw std::runtime_error("Could not open file " + filename);
  }

  std::copy(std::istream_iterator< Polygon >(input_file),
      std::istream_iterator< Polygon >(),
      std::back_inserter(polygons));

  struct Empty {
    bool operator()(const Polygon& p) const
    {
      return p.empty();
    }
  };

  polygons.erase(std::remove_if(polygons.begin(), polygons.end(), Empty()), polygons.end());
}

void gavrilova::startCommandInterface(const std::string& filename, std::istream& is, std::ostream& out)
{
  std::vector< Polygon > polygons;
  try {
    readFile(filename, polygons);
  } catch (const std::exception& e) {
    throw;
  }

  std::vector< std::string > command_tokens;

  auto callCommand = std::bind(processCommand, std::placeholders::_1, std::cref(polygons), std::ref(command_tokens), std::ref(out));

  static std::map< std::string, std::function< void() > > command_map = {
      {"AREA", std::bind(callCommand, processArea)},
      {"MIN", std::bind(callCommand, processMinMax)},
      {"MAX", std::bind(callCommand, processMinMax)},
      {"COUNT", std::bind(callCommand, processCount)},
      {"PERMS", std::bind(callCommand, processPerms)},
      {"LESSAREA", std::bind(callCommand, processLessArea)}};

  std::string input_command;
  while (std::getline(is, input_command)) {
    try {
      auto tokens = tokenize(command_tokens, input_command);
      if (tokens.empty()) {
        continue;
      }
      auto it = command_map.find(tokens[0]);
      if (it == command_map.end()) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      it->second();
    } catch (const std::exception&) {
      throw;
    }
  }
}
