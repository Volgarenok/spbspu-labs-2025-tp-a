#include "rest_time.hpp"

int dribas::get_rest_time(const workout& w)
{
  double trimp = 0.0;
  if (w.avgHeart > 0 && w.timeEnd > w.timeStart) {
    double duration_min = (w.timeEnd - w.timeStart) / 60.0;
    double heart_rate_ratio = (w.avgHeart - 60.0) / (200.0 - 60.0);
    trimp = duration_min * heart_rate_ratio * std::exp(1.92 * heart_rate_ratio);
  }
  return static_cast< int >(trimp);
}

int dribas::calculate_workout_recovery(int sum, const std::pair<time_t, workout>& pair)
{
  return sum + get_rest_time(pair.second);
}
