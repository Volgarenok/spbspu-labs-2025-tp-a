#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <string>
#include <utility>
#include <limits>
#include "commands.hpp"

int main(int argc, char** argv)
{
  using namespace brevnov;
  League league;
  if (argc > 1)
  {
    std::ifstream file(argv[1]);
    if (!file)
    {
      std::cout << "FILE ERROR\n";
      return 1;
    }
    input(file, league);
    file.close();
  }
  std::map< std::string, std::function< void() > > commands;
  using namespace std::placeholders;
  commands.insert(std::make_pair("NewPlayer", std::bind(addNewPlayer, std::ref(std::cin), std::ref(league))));
  commands.insert(std::make_pair("AddTeam", std::bind(addTeam, std::ref(std::cin), std::ref(league))));
  commands.insert(std::make_pair("DeleteTeam", std::bind(deleteTeam, std::ref(std::cin), std::ref(league))));
  commands.insert(std::make_pair("EndCareer", std::bind(endCareer, std::ref(std::cin), std::ref(league))));
  commands.insert(std::make_pair("UpdateRating", std::bind(updateRating, std::ref(std::cin), std::ref(league))));
  commands.insert(std::make_pair("BuyPlayer", std::bind(buyPlayer, std::ref(std::cin), std::ref(std::cout), std::ref(league))));
  commands.insert(std::make_pair("BuyPosition", std::bind(buyPosition, std::ref(std::cin), std::ref(std::cout), std::ref(league))));
  commands.insert(std::make_pair("BuyTeam", std::bind(buyTeam, std::ref(std::cin), std::ref(std::cout), std::ref(league))));
  commands.insert(std::make_pair("SoldPlayer", std::bind(soldPlayer, std::ref(std::cin), std::ref(league))));
  commands.insert(std::make_pair("SoldTeam", std::bind(soldTeam, std::ref(std::cin), std::ref(league))));
  commands.insert(std::make_pair("Deposit", std::bind(deposit, std::ref(std::cin), std::ref(league))));
  commands.insert(std::make_pair("ViewTeam", std::bind(viewTeam, std::ref(std::cin), std::ref(std::cout), std::ref(league))));
  commands.insert(std::make_pair("ViewPosition", std::bind(viewPosition, std::ref(std::cin), std::ref(std::cout), std::ref(league))));
  commands.insert(std::make_pair("Transfer", std::bind(transfer, std::ref(std::cin), std::ref(std::cout), std::ref(league))));
  commands.insert(std::make_pair("StartTeam", std::bind(startTeam, std::ref(std::cin), std::ref(std::cout), std::ref(league))));
  commands.insert(std::make_pair("ViewPlayer", std::bind(viewPlayer, std::ref(std::cin), std::ref(std::cout), std::ref(league))));
  commands.insert(std::make_pair("ViewTeamPosition", std::bind(viewTeamPosition, std::ref(std::cin),
    std::ref(std::cout), std::ref(league))));
  commands.insert(std::make_pair("ViewMarket", std::bind(viewMarket, std::ref(std::cout), std::ref(league))));
  commands.insert(std::make_pair("ViewMarketPosition", std::bind(viewMarketPosition, std::ref(std::cin),
    std::ref(std::cout), std::ref(league))));
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 1;
}
