#ifndef INTERACTION_GAME_HPP
#define INTERACTION_GAME_HPP

#include <map>
#include "characters.hpp"

namespace karnauhova
{
  void choiceCharact(std::map< std::string, Character >& players, const std::map< size_t, Character >& characters);
  void fight(std::map< std::string, Character >& players);
}
#endif
