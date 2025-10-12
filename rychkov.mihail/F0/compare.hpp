#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <string>
#include <utility>
#include <vector>

namespace rychkov
{
  struct Operator;
  namespace entities
  {
    template< class T >
    const std::string& get_name(const T& entity);
    const std::string& get_name(const std::string& identity);
  }
  struct NameCompare
  {
    using is_transparent = void;
    using operator_value = std::vector< Operator >;
    bool operator()(const std::string& lhs, const std::string& rhs) const;

    bool operator()(const operator_value& lhs, const operator_value& rhs) const;
    bool operator()(const operator_value& lhs, const std::string& rhs) const;
    bool operator()(const std::string& lhs, const operator_value& rhs) const;

    bool operator()(const Operator& lhs, const Operator& rhs) const;
    bool operator()(const Operator& lhs, const std::string& rhs) const;
    bool operator()(const std::string& lhs, const Operator& rhs) const;

    template< class T >
    bool operator()(const std::pair< T, size_t >& lhs, const std::pair< T, size_t >& rhs) const;
    template< class T >
    bool operator()(const std::pair< T, size_t >& lhs, const std::string& rhs) const;
    template< class T >
    bool operator()(const std::string& lhs, const std::pair< T, size_t >& rhs) const;

    template< class T >
    bool operator()(const T& lhs, const T& rhs) const;
    template< class T >
    bool operator()(const T& lhs, const std::string& rhs) const;
    template< class T >
    bool operator()(const std::string& lhs, const T& rhs) const;
  };
}

template< class T >
bool rychkov::NameCompare::operator()(const std::pair< T, size_t >& lhs, const std::pair< T, size_t >& rhs) const
{
  return operator()(lhs.first, rhs.first) || (!operator()(rhs.first, lhs.first) && (lhs.second > rhs.second));
}
template< class T >
bool rychkov::NameCompare::operator()(const std::pair< T, size_t >& lhs, const std::string& rhs) const
{
  return operator()(lhs.first, rhs);
}
template< class T >
bool rychkov::NameCompare::operator()(const std::string& lhs, const std::pair< T, size_t >& rhs) const
{
  return operator()(lhs, rhs.first);
}

template< class T >
bool rychkov::NameCompare::operator()(const T& lhs, const T& rhs) const
{
  return entities::get_name(lhs) < entities::get_name(rhs);
}
template< class T >
bool rychkov::NameCompare::operator()(const T& lhs, const std::string& rhs) const
{
  return entities::get_name(lhs) < rhs;
}
template< class T >
bool rychkov::NameCompare::operator()(const std::string& lhs, const T& rhs) const
{
  return lhs < entities::get_name(rhs);
}

#endif
