#include "commands.hpp"
#include <functional>
#include <random>
#include <limits>

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Wrong arguments\n";
        return 1;
    }

    setlocale(LC_ALL, "Russian");
    using namespace smirnov;

    dictionaries dictOfDicts;
    std::string inputCmd(argv[1]);
    if (inputCmd == "--help") {
        printHelp();
    }

    using namespace std::placeholders;

    std::map<std::string, std::function<void(dictionaries&, std::istream&)>> commandsIn;
    std::map<std::string, std::function<void(dictionaries&, std::istream&, std::ostream&)>> commandsInOut;

    commandsIn["createemptydict"] = createemptydict;
    commandsIn["insertword"] = insertword;
    commandsIn["insertfile"] = insertfile;
    commandsIn["deleteword"] = deleteword;
    commandsIn["clear"] = clear;
    commandsIn["merge"] = merge;
    commandsIn["intersect"] = intersect;
    commandsIn["delete"] = deleteDict;

    commandsInOut["getfrequency"] = getfrequency;
    commandsInOut["mostfrequent"] = mostfrequent;
    commandsInOut["printsorted"] = printsorted;
    commandsInOut["printbyfrequency"] = printbyfrequency;
    commandsInOut["size"] = size;
    commandsInOut["topprelativefrequent"] = topprelativefrequent;
    commandsInOut["getrelativefrequency"] = getrelativefrequency;
    commandsInOut["frequencyrangerelative"] = frequencyrangerelative;
    commandsInOut["bottomprelativefrequent"] = bottomprelativefrequent;
    commandsInOut["medianfrequency"] = medianfrequency;

    std::string cmd;
    while (std::cin >> cmd) {
        try {
            commandsIn.at(cmd)(dictOfDicts, std::cin);
        } catch (const std::out_of_range &) {
            try {
                commandsInOut.at(cmd)(dictOfDicts, std::cin, std::cout);
            } catch (const std::exception &e) {
                std::cerr << e.what() << '\n';
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

