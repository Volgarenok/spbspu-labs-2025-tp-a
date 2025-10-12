#ifndef INPUT_OUTPUT_PROCESSING_HPP
#define INPUT_OUTPUT_PROCESSING_HPP

#include <map>
#include "pantsir.hpp"
#include "target.hpp"

namespace lanovenko
{
  void input_pantsir_status(std::istream& in, std::map< std::string, Pantsir >& pantsir_status_sets);
  void input_targets(std::istream& in, std::map< std::string, Target >& rhs);
}

#endif
