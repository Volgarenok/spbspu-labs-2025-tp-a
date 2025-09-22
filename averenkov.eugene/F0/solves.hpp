#ifndef SOLVES_HPP
#define SOLVES_HPP
#include <queue>
#include "commands.hpp"

namespace averenkov
{

  using vec_it = std::vector< std::weak_ptr< const Item > >;
  using vecs_it = std::vector< std::shared_ptr< const Item > >;
  using vec_st = const std::vector< std::string >&;

  vecs_it sharingVec(vec_it weak_items);

  struct CombinationEvaluator
  {
  public:
    CombinationEvaluator(int cap, vec_it& best, int& maxVal, int& bestWgt);
    void operator()(const vec_it& combination);

  private:
    int capacity;
    vec_it& bestCombination;
    int& maxValue;
    int& bestWeight;
  };

  struct CombinationBuilder
  {
    std::vector< std::shared_ptr< const Item > >& current;
    const std::vector< std::shared_ptr< const Item > >& items;
    void operator()(int pos) const;
  };

  struct DPTableInitializer
  {
    std::vector< std::vector< int > >& dp;
    int capacity;

    void operator()(std::vector< int >& row) const;
  };

  struct DPRowFiller
  {
    const vec_it& weak_items;
    std::vector< std::vector< int > >& dp;
    size_t current_index;
    int current_weight;

    void operator()();
  };

  struct DPRowProcessor
  {
    const vec_it& items;
    std::vector< std::vector< int > >& dp;
    size_t current_index;

    void operator()();
  };

  struct DPSolutionBuilder
  {
    const vec_it& weak_items;
    const std::vector< std::vector< int > >& dp;
    vec_it& result;
    int& remaining_weight;
    size_t current_index;

    void operator()();
  };

  struct BacktrackState
  {
    const vec_it& weak_items;
    int capacity;
    vec_it& best_items;
    int& best_value;
    int current_weight;
    int current_value;
    std::vector< bool >& included;
    size_t index;
  };

  struct BacktrackStep
  {
    BacktrackState& state;
    void operator()();
  };

  struct BestSolutionUpdater
  {
    BacktrackState& state;
    void operator()();
  };

  struct ItemCollector
  {
    vec_it& result;
    const std::vector< bool >& included;
    const vec_it& items;
    size_t current_index;

    ItemCollector(vec_it& r, const std::vector< bool >& inc, const vec_it& it);
    void operator()(bool is_included);
  };

  struct Node
  {
    size_t level;
    int weight;
    int value;
    int bound;
    std::vector< bool > included;
  };

  struct ItemSorter
  {
    bool operator()(std::weak_ptr< const Item > shared_a, std::weak_ptr< const Item > shared_b) const;
  };

  struct BoundCalculator
  {
    const vec_it& weak_items;
    int capacity;
    int operator()(const Node* node) const;
  };

  struct BoundCalculatorHelper
  {
    const std::vector< std::shared_ptr< const Item > >& items;
    int capacity;
    int bound;
    int total_weight;
    size_t j;

    void operator()();
  };

  struct NodeExpander
  {
    std::queue< Node* >& queue;
    const vec_it& weak_items;
    int capacity;
    int& max_value;
    std::vector< bool >& best_included;
    void operator()(Node* node) const;
  };

  struct QueueProcessor
  {
    std::queue< Node* >& queue;
    NodeExpander& expander;
    void operator()() const;
  };

  struct BBSolutionBuilder
  {
    const vec_it& items;
    const std::vector<bool>& included;
    Kit& resultKit;
    size_t current_index;
    void operator()() const;
  };

  int calculateTotalWeight(const vec_it& combination);
  int calculateTotalValue(const vec_it& combination);
  void generateCombinations(const vec_it& items, std::vector< vec_it >& allCom, vec_it curCom, size_t index);

  void solve(Base& base, const vec_st args);
  void bruteforce(Base& base, const vec_st args);
  void dynamicProgrammingSolve(Base& base, const vec_st args);
  void backtrackingSolve(Base& base, const vec_st args);
  void branchAndBoundSolve(Base& base, const vec_st args);
}
#endif
