#ifndef FORMATWRAPPER_HPP
#define FORMATWRAPPER_HPP

#include "DataStruct.hpp"
#include <iostream>

std::istream& operator>>(std::istream& in, DataStruct& value);
std::ostream& operator<<(std::ostream& out, const DataStruct& value);

#endif
