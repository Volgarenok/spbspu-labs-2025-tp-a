#include "datastruct.h"

namespace
{
  bool safeDoubleLess(double a, double b, double epsilon = 1e-10)
  {
    return (b - a) > epsilon * std::max(std::abs(a), std::abs(b));
  }
}

bool asafov::cmpDataStruct(const DataStruct &a, const DataStruct &b)
{
  if (a.key1 != b.key1) return a.key1 < b.key1;
  if (a.key2 != b.key2)
  {
    return a.key2.real() < b.key2.real() ||
      (safeDoubleLess(a.key2.real(), b.key2.real()) && safeDoubleLess(a.key2.imag(), b.key2.imag()));
  }
  return a.key3 < b.key3;
}
