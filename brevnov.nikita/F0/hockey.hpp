#ifndef HOCKEY_HPP
#define HOCKEY_HPP
#include <string>
#include <iostream>
#include <map>
namespace brevnov
{
  enum class Position
  {
    LF,
    RF,
    CF,
    RB,
    LB,
    G,
  };
  inline Position definePosition(std::string pos)
  {
    if (pos == "CF")
    {
      return Position::CF;
    }
    else if (pos == "RF")
    {
      return Position::RF;
    }
    else if (pos == "LF")
    {
      return Position::LF;
    }
    else if (pos == "RB")
    {
      return Position::RB;
    }
    else if (pos == "LB")
    {
      return Position::LB;
    }
    else if (pos == "G")
    {
      return Position::G;
    }
    return Position::CF;
  }

  struct Player
  {
    Player(std::string pos, size_t raiting, size_t price):
      position_(Position::CF),
      raiting_(raiting),
      price_(price)
    {
      position_ = definePosition(pos);
    }
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
    Position position_;
    size_t raiting_;
    size_t price_;
  };

  struct Team
  {
    Team(size_t budget):
      players_(std::map< std::string, Player >()),
      budget_(budget)
    {}
    std::map< std::string, Player > players_;
    size_t budget_;
  };

  struct League
  {
    std::map< std::string, Player > fa_;
    std::map< std::string, Team > teams_;
  };

  inline std::ostream& operator<<(std::ostream& os, const Player& player)
  {
    switch (player.position_)
    {
      case Position::CF:
        os << "Center Forward ";
        break;
      case Position::RF:
        os << "Right Forward ";
        break;
      case Position::LF:
        os << "Left Forward ";
        break;
      case Position::RB:
        os << "Right Back ";
        break;
      case Position::LB:
        os << "Left Back ";
        break;
      case Position::G:
        os << "Goalie ";
        break;
    }
    os << player.raiting_ << " OVR " << player.price_ << " EURO";
    return os;
  }
}
#endif
