#include "DataStruct.hpp"

bool operator<(const DataStruct& lhs, const DataStruct& rhs)
{
    if (lhs.key1_ != rhs.key1_)
    {
        return lhs.key1_ < rhs.key1_;
    }
    if (lhs.key2_ != rhs.key2_)
    {
        return lhs.key2_ < rhs.key2_;
    }
    return lhs.key3_.size() < rhs.key3_.size();
}
