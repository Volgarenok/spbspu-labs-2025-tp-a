#ifndef OUTPUT_SAVE_HPP
#define OUTPUT_SAVE_HPP

#include <map>
#include "characters.hpp"

namespace karnauhova
{
  void saveData(std::map< std::string, Character >& players, std::map< size_t, Character >& characters);
}
#endif
