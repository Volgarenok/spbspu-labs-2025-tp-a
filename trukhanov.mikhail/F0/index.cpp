#include "index.hpp"

bool trukhanov::IndexStorage::hasIndex(const std::string& name) const
{
  return indexes_.find(name) != indexes_.end();
}

bool trukhanov::IndexStorage::addIndex(const std::string& name, const trukhanov::ConcordanceIndex& index)
{
  return indexes_.emplace(name, index).second;
}

bool trukhanov::IndexStorage::removeIndex(const std::string& name)
{
  return indexes_.erase(name) > 0;
}

const trukhanov::ConcordanceIndex* trukhanov::IndexStorage::getIndex(const std::string& name) const
{
  auto it = indexes_.find(name);
  return (it != indexes_.end()) ? &it->second : nullptr;
}

trukhanov::ConcordanceIndex* trukhanov::IndexStorage::getIndex(const std::string& name)
{
  auto it = indexes_.find(name);
  return (it != indexes_.end()) ? &it->second : nullptr;
}

const trukhanov::IndexStorage::IndexContainer& trukhanov::IndexStorage::getAll() const
{
  return indexes_;
}
