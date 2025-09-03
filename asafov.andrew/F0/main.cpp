#include <string>
#include <vector>
#include <iostream>
#include "functions.h"

int main()
{
  std::vector<std::string> command_history;

  std::cout << "Группа: 5130904/40004\n";
  std::cout << "ФИО: Асафов Андрей Николаевич\n";
  std::cout << "Тема: Частотный анализ текста. Красно-черное дерево.\n";
  std::cout << "Введите 'show_help' для списка команд\n";

  std::string command;
  bool process = true;
  while (process)
  {
    std::cout << "> ";
    std::getline(std::cin, command);
    if (command.empty()) continue;

    command_history.push_back(command);
    process = asafov::process_command(command, command_history);
  }

  return 0;
}
