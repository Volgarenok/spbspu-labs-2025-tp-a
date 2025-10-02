#include "survival.hpp"

using Accum = dribas::WorkoutAccumulator;

Accum dribas::accumulate_workout_data(Accum acc, const std::pair< time_t, dribas::workout >& entry)
{
  const auto& w = entry.second;
  acc.totalDistance += w.distance;
  acc.totalDuration += (w.timeEnd - w.timeStart) / 3600.0;
  acc.strengthSum += (w.maxHeart - w.avgHeart) + w.cadence;
  acc.workoutCount++;
  return acc;
}

std::string dribas::RecommendationGenerator::operator()(const std::pair< double, std::string >& rule) const
{
  if ((rule.first == 50.0 && score.enduranceScore < rule.first) ||
    (rule.first == 30.0 && score.strengthScore < rule.first)) {
    return rule.second;
  }
  return std::string();
}

dribas::SurvivalScore dribas::calculate_survival_score(const std::map< time_t, dribas::workout >& workouts)
{
  dribas::SurvivalScore score{};
  if (workouts.empty()) {
    return score;
  }
  auto accum = Accum{};
  const auto totals = std::accumulate(workouts.begin(), workouts.end(), accum, dribas::accumulate_workout_data);
  score.enduranceScore = (totals.totalDistance * totals.totalDuration) / totals.workoutCount;
  score.strengthScore = totals.strengthSum / totals.workoutCount;

  auto result = (score.enduranceScore * 0.1) + (score.strengthScore * 0.05) + (totals.totalDistance/totals.workoutCount * 0.2);
  score.estimatedSurvivalDays = static_cast< int >(result);
  const std::map< double, std::string > recommendation = {
    {50.0, "Increase long-distance workouts"},
    {30.0, "Add interval training"}
  };
  dribas::RecommendationGenerator generator{score};
  std::transform(recommendation.begin(), recommendation.end(), std::back_inserter(score.recommendations), generator);
  auto rule = std::remove(score.recommendations.begin(), score.recommendations.end(), std::string());
  score.recommendations.erase(rule, score.recommendations.end());
  return score;
}

std::ostream& dribas::operator<<(std::ostream& os, const dribas::SurvivalScore& score)
{
  std::ostream::sentry sentry(os);
  if (!sentry) {
    return os;
  }
  dribas::StreamGuard guard(os);
  os << "=== Survival Analysis Report ===\n";
  os << "Estimated survival capacity: " << score.estimatedSurvivalDays << " days\n\n";
  os << "Key Metrics:\n";
  os << "- Endurance: " << std::fixed << std::setprecision(1) << score.enduranceScore << "\n";
  os << "- Strength:  " << score.strengthScore << "\n\n";
  if (!score.recommendations.empty()) {
    os << "Recommendations:\n";
    std::copy(score.recommendations.begin(), score.recommendations.end(), std::ostream_iterator< std::string >(os, "\n"));
  }
  return os;
}
