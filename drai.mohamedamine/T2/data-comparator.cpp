#include "data-struct.hpp"
#include <tuple>
#include <cmath>

bool firstry::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  const auto lhsSize = lhs.key3.size();
  const auto rhsSize = rhs.key3.size();
  
  if (std::abs(lhs.key1 - rhs.key1) > 1e-10) {
    return lhs.key1 < rhs.key1;
  }
  
  double lhsRatio = static_cast<double>(lhs.key2.numerator) / lhs.key2.denominator;
  double rhsRatio = static_cast<double>(rhs.key2.numerator) / rhs.key2.denominator;
  if (std::abs(lhsRatio - rhsRatio) > 1e-10) {
    return lhsRatio < rhsRatio;
  }
  
  return lhsSize < rhsSize;
}
