#include "data-struct.hpp"
#include <tuple>

bool kizhin::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  const auto lhsSize = lhs.key3.size();
  const auto rhsSize = rhs.key3.size();
  const auto lhsK2 = lhs.key2;
  const auto rhsK2 = rhs.key2;
  const auto lhsTuple = std::tie(lhs.key1, lhsK2.numerator, lhsK2.denominator, lhsSize);
  const auto rhsTuple = std::tie(rhs.key1, rhsK2.numerator, rhsK2.denominator, rhsSize);
  return lhsTuple < rhsTuple;
}

