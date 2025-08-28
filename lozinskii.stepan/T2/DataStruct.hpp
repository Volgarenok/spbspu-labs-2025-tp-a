#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <string>

struct DataStruct
{
	long long key1_;
	double key2_;
	std::string key3_;
};

bool operator<(const DataStruct& lhs, const DataStruct& rhs);

#endif
