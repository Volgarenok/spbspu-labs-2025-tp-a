#include <limits>
#include <functional>
#include <iostream>
#include "commands.hpp"
#include "utils.hpp"

int main()
{
    using namespace tarasova;
    std::map< std::string, std::function< void() > > cmds;

    Dictionaries dicts;
    cmds["createDict"] = std::bind(createDict, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
    cmds["deleteDict"] = std::bind(deleteDict, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
    cmds["addWord"] = std::bind(addWord, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
    cmds["findWord"] = std::bind(findWord, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
    cmds["deleteWord"] = std::bind(deleteWord, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
    cmds["addTranslate"] = std::bind(addTranslate, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
    cmds["deleteTranslate"] = std::bind(deleteTranslation, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
    cmds["findTranslate"] = std::bind(findTranslate, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
    cmds["mergeDicts"] = std::bind(mergeDicts, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
    cmds["clear"] = std::bind(clear, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
    cmds["printDict"] = std::bind(printDict, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
    cmds["delRepeatWords"] = std::bind(delRepeatWords, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
    std::string command;

    while (!(std::cin >> command).eof())
    {
        try
        {
            cmds.at(command)();
            std::cout << '\n';
        }
        catch (...)
        {
            if (std::cin.fail())
            {
                std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
            }
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
            std::cerr << "<ERROR>\n";
        }
    }
}