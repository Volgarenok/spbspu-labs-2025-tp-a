#ifndef TERMINAL_TEXT_HPP
#define TERMINAL_TEXT_HPP

#include <map>
#include "characters.hpp"

namespace karnauhova
{
  void clearScreen();
  void helloWorld(std::map< std::string, Character >& players);
  void delay(int milliseconds);
}
#endif
