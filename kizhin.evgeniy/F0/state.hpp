#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_STATE_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_STATE_HPP

#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace kizhin {
  using State = std::map< std::string, std::vector< std::string > >;
  std::string getStateFile();
  void saveState(std::ostream&, const State&);
  State loadState(std::istream&);
}

#endif

