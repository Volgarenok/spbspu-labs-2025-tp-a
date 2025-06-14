#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <string>
#include <utility>
#include "content.hpp"

namespace rychkov
{
  struct NameCompare
  {
    using is_transparent = void;

    using base_type_value = std::pair< typing::Type, size_t >;
    using struct_value = std::pair< entities::Struct, size_t >;
    using variable_value = std::pair< entities::Variable, size_t >;
    using operator_value = std::vector< rychkov::Operator >;

    bool operator()(const struct_value& lhs, const struct_value& rhs) const;

    bool operator()(const base_type_value& lhs, const base_type_value& rhs) const;
    bool operator()(const base_type_value& lhs, const std::string& rhs) const;
    bool operator()(const std::string& lhs, const base_type_value& rhs) const;

    bool operator()(const variable_value& lhs, const variable_value& rhs) const;
    bool operator()(const variable_value& lhs, const std::string& rhs) const;
    bool operator()(const std::string& lhs, const variable_value& rhs) const;

    bool operator()(const entities::Variable& lhs, const entities::Variable& rhs) const;
    bool operator()(const entities::Variable& lhs, const std::string& rhs) const;
    bool operator()(const std::string& lhs, const entities::Variable& rhs) const;

    bool operator()(const operator_value& lhs, const operator_value& rhs) const;
    bool operator()(const operator_value& lhs, const std::string& rhs) const;
    bool operator()(const std::string& lhs, const operator_value& rhs) const;
  };
}

#endif
