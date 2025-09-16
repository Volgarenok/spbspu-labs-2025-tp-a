#ifndef RACE_PREDICTION_HPP
#define RACE_PREDICTION_HPP

#include <map>
#include <ostream>
#include "workout.hpp"

namespace dribas
{
  struct RacePrediction
  {
    double fiveKm;
    double tenKm;
    double halfMarathon;
    double marathon;
  };
  RacePrediction predict_result(const std::map< time_t, workout >&);
  std::ostream& operator<<(std::ostream&, const RacePrediction&);

}

#endif
