#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "hockey.hpp"
namespace brevnov
{
  bool checkPosition(std::string pos);
  void input(std::istream& in, brevnov::League& league);
  void addNewPlayer(std::istream&, League&);
  void addTeam(std::istream&, League&);
  void deleteTeam(std::istream&, League&);
  void endCareer(std::istream&, League&);
  void updateRating(std::istream&, League&);
  void buyPlayer(std::istream&, std::ostream&, League&);
  void buyPosition(std::istream&, std::ostream&, League&);
  void buyTeam(std::istream&, std::ostream&, League&);
  void soldPlayer(std::istream&, League&);
  void soldTeam(std::istream&, League&);
  void deposit(std::istream&, League&);
  void viewTeam(std::istream&, std::ostream&, League&);
  void viewPosition(std::istream&, std::ostream&, League&);
  void transfer(std::istream&, std::ostream&, League&);
  void startTeam(std::istream&, std::ostream&, League&);
  void viewPlayer(std::istream&, std::ostream&, League&);
  void viewTeamPosition(std::istream&, std::ostream&, League&);
  void viewMarket(std::ostream&, League&);
  void viewMarketPosition(std::istream&, std::ostream&, League&);
}
#endif
