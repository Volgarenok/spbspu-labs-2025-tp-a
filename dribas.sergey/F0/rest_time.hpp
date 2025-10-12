#ifndef REST_TIME_HPP
#define REST_TIME_HPP

#include "workout.hpp"

namespace dribas
{
  int get_rest_time(const workout&);
  int calculate_workout_recovery(int sum, const std::pair< time_t, workout >& pair);
}

#endif
