#include "commands.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <functional>

using namespace shiryaeva;

int main()
{
  Dictionaries dicts;

  std::map< std::string, std::function< void(std::istringstream&) > > cmds;

  using std::placeholders::_1;
  cmds["create_dict"]   = std::bind(create_dict, std::ref(dicts), _1, std::ref(std::cout));
  cmds["add_text"]      = std::bind(add_text, std::ref(dicts), _1, std::ref(std::cout));
  cmds["add_string"]    = std::bind(add_string, std::ref(dicts), _1, std::ref(std::cout));
  cmds["get_freq"]      = std::bind(get_freq, std::ref(dicts), _1, std::ref(std::cout));
  cmds["get_rel_freq"]  = std::bind(get_rel_freq, std::ref(dicts), _1, std::ref(std::cout));
  cmds["remove_word"]   = std::bind(remove_word, std::ref(dicts), _1, std::ref(std::cout));
  cmds["filter_by_freq"]= std::bind(filter_by_freq, std::ref(dicts), _1, std::ref(std::cout));
  cmds["exclude_by_freq"]= std::bind(exclude_by_freq, std::ref(dicts), _1, std::ref(std::cout));
  cmds["display_all"]   = std::bind(display_all, std::ref(dicts), _1, std::ref(std::cout));
  cmds["clear_dict"]    = std::bind(clear_dict, std::ref(dicts), _1, std::ref(std::cout));
  cmds["merge_dicts"]   = std::bind(merge_dicts, std::ref(dicts), _1, std::ref(std::cout));
  cmds["most_common"]   = std::bind(most_common, std::ref(dicts), _1, std::ref(std::cout));


  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty()) continue;

    std::istringstream full(line);
    std::string cmd;
    full >> cmd;
    if (cmd.empty()) continue;

    std::string rest;
    std::getline(full, rest);
    std::istringstream args(rest);

    auto it = cmds.find(cmd);
    if (it != cmds.end())
    {
      try
      {
        it->second(args);
      }
      catch (const std::runtime_error &e)
      {
        std::cout << e.what() << '\n';
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    else if (cmd == "exit" || cmd == "quit")
    {
      break;
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}

