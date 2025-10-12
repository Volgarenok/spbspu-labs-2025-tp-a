#include <clocale>
#include <limits>
#include "CommandHandler.h"
#include "HelpText.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
  using namespace voronina;
  std::setlocale(LC_ALL, "Russian");
  if (argc > 2)
  {
    std::cout << "ОШИБКА: слишком много аргументов командной строки\n";
    return 1;
  }
  if (argc == 2)
  {
    std::string arg = argv[1];
    if (arg == "--help")
    {
      std::cout << HelpText::getHelpText();
      return 0;
    }
    else
    {
      std::cout << "ОШИБКА: неверный аргумент командной строки\n";
      return 1;
    }
  }

  CommandHandler commandHandler;

  std::string command;
  while (std::cin >> command)
  {
    commandHandler.executeCommand(toUpper(command), std::cin, std::cout);

    if (std::cin.fail())
    {
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  if (std::cin.bad())
  {
    std::cerr << "ОШИБКА: Произошла неустранимая ошибка ввода (badbit)\n";
    return 1;
  }

  return 0;
}
