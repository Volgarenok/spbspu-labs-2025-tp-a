#ifndef VALIDATION_UTILS_HPP
#define VALIDATION_UTILS_HPP

#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

namespace crossref
{

  namespace validation
  {

    void checkArgCount(const std::vector< std::string > &args, size_t expected, const std::string &usage);

    template < typename Container >
    void checkIdExists(const Container &container, const std::string &id, const std::string &error);

    template < typename Container >
    void checkIdNotFound(const Container &container, const std::string &id, const std::string &error);

    void checkLineRange(int start, int end, size_t max_lines);

    void checkPositive(int value, const std::string &param);

  }

}

#endif
