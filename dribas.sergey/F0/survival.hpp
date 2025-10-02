#ifndef SURVIVAL_HPP
#define SURVIVAL_HPP

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iterator>
#include <map>
#include <numeric>
#include <vector>
#include <string>

#include <streamGuard.hpp>
#include "workout.hpp"

namespace dribas
{

  struct SurvivalScore
  {
    double enduranceScore;
    double strengthScore;
    double resilienceScore;
    int estimatedSurvivalDays;
    std::vector< std::string > recommendations;
  };

  struct WorkoutAccumulator
  {
    double totalDistance = 0.0;
    double totalDuration = 0.0;
    double strengthSum = 0.0;
    int workoutCount = 0;
  };

  WorkoutAccumulator accumulate_workout_data
  (
    WorkoutAccumulator acc,
    const std::pair< time_t, workout >& entry);

  struct RecommendationGenerator
  {
    const SurvivalScore& score;
    std::string operator()(const std::pair< double, std::string >& rule) const;
  };

  SurvivalScore calculate_survival_score(const std::map< time_t, workout >& workouts);

  std::ostream& operator<<(std::ostream& os, const SurvivalScore& score);

}

#endif
