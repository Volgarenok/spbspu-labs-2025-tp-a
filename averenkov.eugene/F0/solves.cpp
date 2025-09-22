#include <numeric>
#include "solves.hpp"
#include "commands.hpp"

averenkov::vecs_it averenkov::sharingVec(vec_it weak_items)
{
  vecs_it items;
  for (size_t i = 0; i < weak_items.size(); i++)
  {
    if (auto shared_item = weak_items[i].lock())
    {
      items.push_back(shared_item);
    }
  }
  return items;
}

void averenkov::CombinationBuilder::operator()(int pos) const
{
  if (MaskChecker(pos)(pos))
  {
    current.push_back(items[pos]);
  }
}

averenkov::CombinationEvaluator::CombinationEvaluator(int cap, vec_it& best, int& maxVal, int& bestWgt):
  capacity(cap),
  bestCombination(best),
  maxValue(maxVal),
  bestWeight(bestWgt)
{}

void averenkov::CombinationEvaluator::operator()(const vec_it& combination)
{
  int totalWeight = calculateTotalWeight(combination);
  int totalValue = calculateTotalValue(combination);

  if (totalWeight <= capacity && (totalValue > maxValue || (totalValue == maxValue && totalWeight < bestWeight)))
  {
    bestCombination = combination;
    maxValue = totalValue;
    bestWeight = totalWeight;
  }
}

void averenkov::DPTableInitializer::operator()(std::vector<int>& row) const
{
  row.resize(capacity + 1);
  std::fill(row.begin(), row.end(), 0);
}

void averenkov::DPRowFiller::operator()()
{
  auto items = sharingVec(weak_items);
  if (current_weight > 0)
  {
    if (items[current_index-1]->getWeight() <= current_weight)
    {
      auto te = dp[current_index-1][current_weight];
      auto tu = dp[current_index-1][current_weight - items[current_index-1]->getWeight()];
      dp[current_index][current_weight] = std::max( te, tu + items[current_index-1]->getValue());
    }
    else
    {
      dp[current_index][current_weight] = dp[current_index-1][current_weight];
    }
    DPRowFiller next{weak_items, dp, current_index, current_weight - 1};
    next();
  }
}

void averenkov::DPRowProcessor::operator()()
{
  if (current_index <= items.size())
  {
    int last_weight = dp[current_index].empty() ? 0 : dp[current_index].size() - 1;
    DPRowFiller filler{items, dp, current_index, last_weight};
    filler();
    DPRowProcessor next{items, dp, current_index + 1};
    next();
  }
}

void averenkov::DPSolutionBuilder::operator()()
{
  auto items = sharingVec(weak_items);
  if (current_index > 0 && remaining_weight > 0)
  {
    if (dp[current_index][remaining_weight] != dp[current_index-1][remaining_weight])
    {
      result.push_back(items[current_index-1]);
      remaining_weight -= items[current_index-1]->getWeight();
    }
    DPSolutionBuilder next{weak_items, dp, result, remaining_weight, current_index - 1};
    next();
  }
}

void averenkov::generateCombinations(const vec_it& items, std::vector< vec_it >& allCom, vec_it curCom, size_t index)
{
  if (index == items.size())
  {
    allCom.push_back(curCom);
    return;
  }
  generateCombinations(items, allCom, curCom, index + 1);

  curCom.push_back(items[index]);
  generateCombinations(items, allCom, curCom, index + 1);
}

int averenkov::calculateTotalWeight(const vec_it& combination)
{
  return std::accumulate(combination.begin(), combination.end(), 0, WeakWeightCalculator());
}

int averenkov::calculateTotalValue(const vec_it& combination)
{
  return std::accumulate(combination.begin(), combination.end(), 0, WeakValueCalculator());
}

averenkov::ItemCollector::ItemCollector(vec_it& r, const std::vector< bool >& inc, const vec_it& it):
  result(r),
  included(inc),
  items(it),
  current_index(0)
{}

void averenkov::ItemCollector::operator()(bool is_included)
{
  if (is_included)
  {
    result.push_back(items[current_index]);
  }
  current_index++;
}

void averenkov::BestSolutionUpdater::operator()()
{
  auto items = sharingVec(state.weak_items);
  int best_weight = 0;
  WeakWeightCalculator calc;
  best_weight = std::accumulate(state.best_items.begin(), state.best_items.end(), 0, calc);
  auto cond = (state.current_value == state.best_value && state.current_weight < best_weight);
  if (state.current_value > state.best_value || cond)
  {
    state.best_value = state.current_value;
    state.best_items.clear();
    ItemCollector collector(state.best_items, state.included, state.weak_items);
    std::for_each(state.included.begin(), state.included.end(), collector);
  }
}

void averenkov::BacktrackStep::operator()()
{
  auto items = sharingVec(state.weak_items);
  if (state.index >= items.size())
  {
    BestSolutionUpdater updater{state};
    updater();
    return;
  }
  state.included[state.index] = false;
  BacktrackState next_state = state;
  next_state.index++;
  BacktrackStep step{next_state};
  step();
  if (state.current_weight + items[state.index]->getWeight() <= state.capacity)
  {
    state.included[state.index] = true;
    state.current_weight += items[state.index]->getWeight();
    state.current_value += items[state.index]->getValue();
    BacktrackState include_state = state;
    include_state.index++;
    BacktrackStep include_step{include_state};
    include_step();
    state.included[state.index] = false;
    state.current_weight -= items[state.index]->getWeight();
    state.current_value -= items[state.index]->getValue();
  }
}

bool averenkov::ItemSorter::operator()(std::weak_ptr < const Item > shared_a, std::weak_ptr< const Item > shared_b) const
{
  auto a = shared_a.lock();
  auto b = shared_b.lock();
  return a->getValue() * b->getWeight() > b->getValue() * a->getWeight();
}

int averenkov::BoundCalculator::operator()(const Node* node) const
{
  auto items = sharingVec(weak_items);

  if (node->weight >= capacity)
  {
    return 0;
  }

  auto cond = (node->level == 0 && node->weight == 0 && node->value == 0);
  size_t start_level = cond ? 0 : node->level + 1;
  BoundCalculatorHelper helper{ items, capacity, node->value, node->weight, start_level };
  helper();
  return helper.bound;
}

void averenkov::BoundCalculatorHelper::operator()()
{
  if (j < items.size() && total_weight + items[j]->getWeight() <= capacity)
  {
    total_weight += items[j]->getWeight();
    bound += items[j]->getValue();
    j++;
    (*this)();
  }
  else if (j < items.size())
  {
    bound += (capacity - total_weight) * items[j]->getValue() / items[j]->getWeight();
  }
}

void averenkov::NodeExpander::operator()(Node* node) const
{
  auto items = sharingVec(weak_items);

  if (node->bound > max_value)
  {
    auto w = node->weight + items[node->level]->getWeight();
    auto v = node->value + items[node->level]->getValue();
    Node* left = new Node{ node->level + 1, w, v, 0, node->included };
    left->included[node->level] = true;
    left->bound = BoundCalculator{weak_items, capacity}(left);

    if (left->weight <= capacity && left->value > max_value)
    {
      max_value = left->value;
      best_included = left->included;
    }

    if (left->bound > max_value)
    {
      queue.push(left);
    }
    else
    {
      delete left;
    }
    Node* right = new Node{ node->level + 1, node->weight, node->value, 0, node->included };
    right->bound = BoundCalculator{weak_items, capacity}(right);
    if (right->bound > max_value)
    {
      queue.push(right);
    }
    else
    {
      delete right;
    }
  }
  delete node;
}

void averenkov::QueueProcessor::operator()() const
{
  if (!queue.empty())
  {
    Node* node = queue.front();
    queue.pop();
    expander(node);
    QueueProcessor next{queue, expander};
    next();
  }
}

void averenkov::BBSolutionBuilder::operator()() const
{
  if (current_index < items.size())
  {
    if (included[current_index])
    {
      if (auto shared_item = items[current_index].lock())
      {
        resultKit.addItem(shared_item);
      }
    }
    BBSolutionBuilder next{items, included, resultKit, current_index + 1};
    next();
  }
}

void averenkov::solve(Base& base, vec_st args)
{
  auto kitIt = base.kits.find(args[1]);
  if (kitIt == base.kits.end())
  {
    throw std::invalid_argument("Source kit not found");
  }
  if (kitIt->second.getItems().size() < 4)
  {
    bruteforce(base, args);
    return;
  }
  branchAndBoundSolve(base, args);
}


void averenkov::bruteforce(Base& base, vec_st args)
{
  if (args.size() < 3)
  {
    throw std::invalid_argument("Not enough arguments for bruteforce command");
  }

  const std::string& sourceKitName = args[1];
  const std::string& resultKitName = args[2];

  auto kitIt = base.kits.find(sourceKitName);
  if (kitIt == base.kits.end())
  {
    throw std::invalid_argument("Source kit not found");
  }

  const Kit& sourceKit = kitIt->second;
  int capacity = base.current_knapsack.getCapacity();
  const auto& items = sourceKit.getItems();

  std::vector< std::vector< std::weak_ptr < const Item > > > allCombinations;
  generateCombinations(items, allCombinations, {}, 0);

  std::vector< std::weak_ptr < const Item > > bestCombination;
  int maxValue = 0;
  int bestWeight = 0;
  CombinationEvaluator evaluator(capacity, bestCombination, maxValue, bestWeight);
  std::for_each(allCombinations.begin(), allCombinations.end(), evaluator);

  if (base.kits.find(resultKitName) != base.kits.end())
  {
    throw std::invalid_argument("Kit already exists");
  }
  auto emplaceResult = base.kits.emplace(resultKitName, Kit(resultKitName));
  Kit& resultKit = emplaceResult.first->second;

  WeakItemAdder adder(resultKit);
  std::for_each(bestCombination.begin(), bestCombination.end(), adder);
}

void averenkov::dynamicProgrammingSolve(Base& base, vec_st args)
{
  if (args.size() < 3)
  {
    throw std::invalid_argument("Not enough arguments for dp command");
  }

  const std::string& sourceKitName = args[1];
  const std::string& resultKitName = args[2];

  auto kitIt = base.kits.find(sourceKitName);
  if (kitIt == base.kits.end())
  {
    throw std::invalid_argument("Source kit not found");
  }

  if (base.kits.find(resultKitName) != base.kits.end())
  {
    throw std::invalid_argument("Result kit already exists");
  }

  const Kit& sourceKit = kitIt->second;
  int capacity = base.current_knapsack.getCapacity();
  std::vector< std::weak_ptr< const Item > > items = sourceKit.getItems();

  std::vector< std::vector< int > > dp(items.size() + 1);
  DPTableInitializer init{dp, capacity};
  std::for_each(dp.begin(), dp.end(), init);

  DPRowProcessor processor{items, dp, 1};
  processor();

  std::vector< std::weak_ptr< const Item > > selectedItems;
  int remaining_weight = capacity;
  DPSolutionBuilder builder{items, dp, selectedItems, remaining_weight, items.size()};
  builder();

  Kit& resultKit = base.kits.emplace(resultKitName, Kit(resultKitName)).first->second;
  WeakItemAdder adder(resultKit);
  std::for_each(selectedItems.begin(), selectedItems.end(), adder);
}


void averenkov::backtrackingSolve(Base& base, vec_st args)
{
  if (args.size() < 3)
  {
    throw std::invalid_argument("Not enough arguments for backtrack command");
  }
  const std::string& sourceKitName = args[1];
  const std::string& resultKitName = args[2];
  auto kitIt = base.kits.find(sourceKitName);
  if (kitIt == base.kits.end())
  {
    throw std::invalid_argument("Source kit not found");
  }

  if (base.kits.find(resultKitName) != base.kits.end())
  {
    throw std::invalid_argument("Result kit already exists");
  }

  const Kit& sourceKit = kitIt->second;
  int capacity = base.current_knapsack.getCapacity();
  const vec_it& items = sourceKit.getItems();

  vec_it best_items;
  int best_value = 0;
  std::vector< bool > included(items.size(), false);

  BacktrackState initial_state{ items, capacity, best_items, best_value, 0, 0, included, 0 };

  BacktrackStep step{initial_state};
  step();

  Kit& resultKit = base.kits.emplace(resultKitName, Kit(resultKitName)).first->second;
  WeakItemAdder adder(resultKit);
  std::for_each(best_items.begin(), best_items.end(), adder);
}

void averenkov::branchAndBoundSolve(Base& base, const std::vector<std::string>& args)
{
  if (args.size() < 3)
  {
    throw std::invalid_argument("Not enough arguments for branch and bound command");
  }

  const std::string& sourceKitName = args[1];
  const std::string& resultKitName = args[2];

  auto kitIt = base.kits.find(sourceKitName);
  if (kitIt == base.kits.end())
  {
    throw std::invalid_argument("Source kit not found");
  }

  if (base.kits.find(resultKitName) != base.kits.end())
  {
    throw std::invalid_argument("Result kit already exists");
  }

  const Kit& sourceKit = kitIt->second;
  int capacity = base.current_knapsack.getCapacity();
  std::vector< std::weak_ptr< const Item > > items = sourceKit.getItems();

  ItemSorter sorter;
  std::sort(items.begin(), items.end(), sorter);

  std::queue< Node* > queue;
  std::vector< bool > best_included(items.size(), false);
  int max_value = 0;

  Node* root = new Node{ 0, 0, 0, 0, std::vector< bool >(items.size(), false) };
  root->bound = BoundCalculator{ items, capacity }(root);
  queue.push(root);

  NodeExpander expander{ queue, items, capacity, max_value, best_included };
  QueueProcessor processor{ queue, expander };
  processor();

  Kit& resultKit = base.kits.emplace(resultKitName, Kit(resultKitName)).first->second;
  BBSolutionBuilder builder{ items, best_included, resultKit, 0 };
  builder();
}
