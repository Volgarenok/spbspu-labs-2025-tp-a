#include "race_predictor.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <vector>
#include <limits>
#include <streamGuard.hpp>


namespace
{
  std::string formatTime(double totalMinutes)
  {
    if (totalMinutes < 0) {
      return "--:--";
    }
    long long totalSeconds = static_cast<long long>(std::round(totalMinutes * 60.0));
    long long minutes = totalSeconds / 60;
    long long seconds = totalSeconds % 60;

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
    return ss.str();
  }

  struct WorkoutStats
  {
    size_t count;
    double max_distance;
    double min_pace;

    WorkoutStats():
      count(0),
      max_distance(0.0),
      min_pace(std::numeric_limits< double >::max())
    {}
    WorkoutStats operator()(WorkoutStats stats, const std::pair<time_t, dribas::workout>& entry) const
    {
      const auto& w = entry.second;
      if (w.distance > 0 && w.avgPaceMinPerKm > 0) {
        stats.count++;
        stats.max_distance = std::max(stats.max_distance, w.distance);
        stats.min_pace = std::min(stats.min_pace, w.avgPaceMinPerKm);
      }
      return stats;
    }
  };
}

dribas::RacePrediction dribas::predict_result(const std::map<time_t, workout>& workouts)
{
  RacePrediction prediction = { 0.0, 0.0, 0.0, 0.0 };
  WorkoutStats stats = std::accumulate(workouts.begin(), workouts.end(), WorkoutStats{}, WorkoutStats());

  if (stats.count == 0) {
    return prediction;
  }
  double max_distance = stats.max_distance;
  double best_pace = stats.min_pace;
  prediction.fiveKm = best_pace * 1.06;
  prediction.tenKm = best_pace * 1.08;
  if (max_distance >= 10.0) {
    prediction.halfMarathon = best_pace * 1.12;
  } else {
    prediction.halfMarathon = prediction.tenKm * 1.05;
  }
  if (max_distance >= 21.0) {
    prediction.marathon = best_pace * 1.18;
  } else if (max_distance >= 10.0) {
    prediction.marathon = prediction.halfMarathon * 1.10;
  } else {
    prediction.marathon = prediction.halfMarathon * 1.15;
  }
  return prediction;
}

std::ostream& dribas::operator<<(std::ostream& os, const RacePrediction& prediction)
{
  StreamGuard guard(os);
  const double DIST_5K = 5.0;
  const double DIST_10K = 10.0;
  const double DIST_HALF_MARATHON = 21.0975;
  const double DIST_MARATHON = 42.195;
  double time5K = prediction.fiveKm * DIST_5K;
  double time10K = prediction.tenKm * DIST_10K;
  double timeHalfMarathon = prediction.halfMarathon * DIST_HALF_MARATHON;
  double timeMarathon = prediction.marathon * DIST_MARATHON;

  os << "=== Race Time Predictions ===\n";
  os << "  5K:       " << formatTime(time5K) << " (min:sec)\n";
  os << "  10K:      " << formatTime(time10K) << " (min:sec)\n";
  os << "  21.1K:    " << formatTime(timeHalfMarathon) << " (min:sec)\n";
  os << "  42.2K:    " << formatTime(timeMarathon) << " (min:sec)\n";
  return os;
}
