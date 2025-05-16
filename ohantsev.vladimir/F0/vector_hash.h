#ifndef VECTOR_HASH_H
#define VECTOR_HASH_H
#include <vector>

template< typename T, typename Hash >
struct VectorHash
{
  std::size_t operator()(const std::vector< T >& vec) const
  {
    Hash hasher;
    std::size_t seed = vec.size();
    for (const auto& elem: vec)
    {
      seed ^= hasher(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
  }
};
#endif
