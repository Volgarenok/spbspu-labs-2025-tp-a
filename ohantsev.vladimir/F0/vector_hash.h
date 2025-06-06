#ifndef VECTOR_HASH_H
#define VECTOR_HASH_H
#include <vector>
#include <numeric>

namespace ohantsev
{
  template<typename T, typename Hash>
  struct VectorHash
  {
  public:
    std::size_t operator()(const std::vector< T >& vec) const
    {
      return std::accumulate(vec.begin(), vec.end(), vec.size(), HashCombiner{ Hash{} });
    }

  private:
    struct HashCombiner
    {
        Hash hasher;

        std::size_t operator()(std::size_t seed, const T& elem) const
        {
          return seed ^ (hasher(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
        }
    };
  };
}
#endif
