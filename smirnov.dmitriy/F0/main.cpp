#include "commands.hpp"
#include <functional>
#include <random>
#include <limits>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Wrong arguments\n";
        return 1;
    }

    using namespace smirnov;

    dictionaries dictOfDicts;
    std::string inputCmd(argv[1]);
    if (inputCmd == "--help") {
        printHelp(std::cout);
    }

    using namespace std::placeholders;

    const std::map<std::string, std::function<void(dictionaries&, std::istream&)>> commandsIn = {
        {"createemptydict", createemptydict},
        {"insertword", insertword},
        {"insertfile", insertfile},
        {"deleteword", deleteword},
        {"clear", clear},
        {"merge", merge},
        {"intersect", intersect},
        {"delete", deleteDict}
    };

    const std::map<std::string, std::function<void(dictionaries&, std::istream&, std::ostream&)>> commandsInOut = {
        {"getfrequency", getfrequency},
        {"mostfrequent", mostfrequent},
        {"printsorted", printsorted},
        {"printbyfrequency", printbyfrequency},
        {"size", size},
        {"topprelativefrequent", topprelativefrequent},
        {"getrelativefrequency", getrelativefrequency},
        {"frequencyrangerelative", frequencyrangerelative},
        {"bottomprelativefrequent", bottomprelativefrequent},
        {"medianfrequency", medianfrequency}
    };

    std::string cmd;
    while (std::cin >> cmd) {
        try {
            commandsIn.at(cmd)(dictOfDicts, std::cin);
        }
        catch (const std::out_of_range&) {
            try {
                commandsInOut.at(cmd)(dictOfDicts, std::cin, std::cout);
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << '\n';
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

