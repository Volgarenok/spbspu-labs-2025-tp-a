#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <limits>
#include "characters.hpp"
#include "input_file.hpp"
#include "terminal_text.hpp"
#include "interaction_game.hpp"
#include "output_save.hpp"

int main()
{
  setlocale(LC_ALL, "ru");
  using namespace karnauhova;
  std::map< size_t, Character > characters;
  std::map< std::string, Character > players;
  try
  {
    players = inputData(characters);
  }
  catch (const std::exception& e)
  {
    return 0;
  }
  if (players.size() != 0)
  {
    auto it = players.begin();
    auto one = it;
    auto two = ++it;
    if (one->second.position != 0 || two->second.position != 0)
    {
      clearScreen();
      std::cout << "\033[1;31m" << std::string(100, '=') << "\033[0m" << "\n";
      std::cout << "Возвращение в прошлый бой:\n";
      fight(players);
    }
    clearScreen();
    std::cout << "\033[1;32m" << std::string(100, '=') << "\033[0m" << "\n";
    std::cout << "С возвращением " << one->first << ", " << two->first << "!\n";
  }
  else
  {
    try
    {
      helloWorld(players);
    }
    catch (const std::exception& e)
    {
      return 0;
    }
  }
  delay(1200);
  std::map< std::string, std::function< void() > > cmds;
  cmds["CHARACTERS"] = std::bind(choiceCharact, std::ref(players), std::cref(characters));
  cmds["FIGHT"] = std::bind(fight, std::ref(players));
  while (!std::cin.eof())
  {
    clearScreen();
    std::cout << "\033[1;32m" << std::string(100, '=') << "\033[0m" << "\n";
    std::cout << "-Лобби-\n\n";
    std::cout << ">Чтобы взять нового персонажа или поменять старого введите CHARACTERS\n";
    std::cout << ">Чтобы начать бой введите FIGHT\n";
    std::string answer;
    std::cin >> answer;
    try
    {
      cmds.at(answer)();
    }
    catch (...)
    {
      if (std::cin.eof())
      {
        std::cin.clear();
        break;
      }
      std::cout << "Убедитесь, что вы верно выбрали персонажей";
      std::cout << "(у обоих игроков должны быть персонажи)\n";
      delay(2000);
    }
  }
  clearScreen();
  std::cout << "\033[1;31m" << std::string(100, '=') << "\033[0m" << "\n";
  std::cout << "Пока)\n";
  saveData(players, characters);
}
