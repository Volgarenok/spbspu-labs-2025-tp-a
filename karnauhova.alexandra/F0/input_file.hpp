#ifndef INPUT_FILE_HPP
#define INPUT_FILE_HPP

#include <map>
#include "characters.hpp"

namespace karnauhova
{
  std::map< std::string, Character > inputData(std::map< size_t, Character >& characters);
  void inputNewData(std::istream& in, std::map< size_t, Character >& characters);
  std::map< std::string, Character > inputSaveData(std::istream& in, std::map< size_t, Character >& characters);
}
#endif
