#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <boost/variant2.hpp>
#include <utility>
#include "compare.hpp"

namespace rychkov
{
  template< class T >
  struct DynMemWrapper: std::unique_ptr< T >
  {
    using value_type = T;
    DynMemWrapper():
      std::unique_ptr< T >(new T)
    {}
    DynMemWrapper(const DynMemWrapper& rhs):
      std::unique_ptr< T >(rhs != nullptr ? new T{*rhs} : nullptr)
    {}
    DynMemWrapper(DynMemWrapper&&) = default;
    DynMemWrapper(std::nullptr_t):
      std::unique_ptr< T >(nullptr)
    {}
    DynMemWrapper(T* ptr):
      std::unique_ptr< T >(ptr)
    {}
    template< class... Args >
    DynMemWrapper(Args&&... args):
      std::unique_ptr< T >(new T{std::forward< Args >(args)...})
    {}
    DynMemWrapper& operator=(const DynMemWrapper& rhs)
    {
      if (rhs != nullptr)
      {
        static_cast< std::unique_ptr< T >& >(*this).reset(new T{*rhs});
      }
      return *this;
    }
    DynMemWrapper& operator=(DynMemWrapper&&) = default;
    template< class U = T >
    DynMemWrapper& operator=(U&& rhs)
    {
      static_cast< std::unique_ptr< T >& >(*this).reset(new T{std::forward< U >(rhs)});
      return *this;
    }
    DynMemWrapper& operator=(std::nullptr_t)
    {
      static_cast< std::unique_ptr< T >& >(*this) = nullptr;
      return *this;
    }
    DynMemWrapper& operator=(T* ptr)
    {
      static_cast< std::unique_ptr< T >& >(*this).reset(ptr);
      return *this;
    }
  };

  struct Macro
  {
    std::string name;
    std::string body;
    bool func_style = false;
    std::vector< std::string > parameters;
  };
  struct Operator
  {
    enum Type
    {
      UNARY = 1,
      BINARY = 2,
      TERNARY = 3,
      MULTIPLE = -1
    };
    enum Category
    {
      ARITHMETIC,
      INCREMENT,
      MODULUS,
      LOGIC,
      COMPARE,
      BIT,
      DEREFERENCE,
      ADDRESSOF,
      FIELD_ACCESS,
      SPECIAL,
      ASSIGN
    };

    Type type;
    Category category;
    std::string token;
    bool require_lvalue = false;
    bool right_align = false;
    bool produce_lvalue = false;
    int priority = 0;
  };
  namespace typing
  {
    enum LengthCategory
    {
      NO_LENGTH,
      SHORT,
      LONG,
      LONG_LONG
    };
    enum Category
    {
      STRUCT,
      ENUM,
      BASIC,
      POINTER,
      ARRAY,
      FUNCTION,
      COMBINATION
    };
    enum MatchType
    {
      EXACT,
      IMPLICIT,
      NO_CAST
    };
    struct Type
    {
      std::string name;
      Category category = COMBINATION;
      DynMemWrapper< Type > base = nullptr;
      bool is_const = false;
      bool is_volatile = false;
      bool is_signed = false;
      bool is_unsigned = false;
      LengthCategory length_category = NO_LENGTH;
      bool array_has_length = false;
      size_t array_length = 0;
      std::vector< Type > function_parameters;

      bool empty() const noexcept;
      bool ready() const noexcept;
    };
    bool is_basic(const Type* type);
    bool is_enum(const Type* type);
    bool is_function(const Type* type);
    bool is_pointer(const Type* type);
    bool is_array(const Type* type);
    bool is_callable(const Type* type);
    bool is_iterable(const Type* type);
    bool is_integer(const Type* type);

    bool exact_cv(const Type& dest, const Type& src);
    MatchType check_cast(const Type& dest, const Type& src);
    MatchType check_overload(const Type& function, const std::vector< Type >& args);
    const typing::Type* largest_integer(const Type& lhs, const Type& rhs);
    const typing::Type* common_type(const Type& lhs, const Type& rhs);
    bool pointer_comparable(const Type& lhs, const Type& rhs);
  }
  namespace entities
  {
    enum ScopeType
    {
      EXTERN,
      STATIC,
      UNSPECIFIED
    };
    struct Expression;
    struct Body
    {
      Body();
      std::vector< Expression > data;
    };
    struct Variable
    {
      typing::Type type;
      std::string name;
    };
    struct Function
    {
      typing::Type type;
      std::string name;
      std::vector< std::string > parameters;
    };
    struct Statement
    {
      enum Type
      {
        IF,
        ELSE,
        FOR,
        WHILE,
        DO,
        RETURN,
        DO_WHILE,
        TYPE_LAST
      };
      Type type;
      std::vector< Expression > conditions;
    };
    struct Struct
    {
      std::string name;
      std::set< Variable, NameCompare > fields;
    };
    struct Enum
    {
      std::string name;
      std::map< std::string, int, NameCompare > fields;
    };
    struct Union
    {
      std::string name;
      std::set< Variable, NameCompare > fields;
    };
    struct Alias
    {
      typing::Type type;
      std::string name;
    };
    struct Declaration
    {
      using declared = boost::variant2::variant< Variable, Struct, Enum, Union, Alias, Function, Statement >;
      declared data;
      DynMemWrapper< Expression > value = nullptr;
      ScopeType scope = UNSPECIFIED;
    };
    struct CastOperation
    {
      typing::Type to;
      bool is_explicit = false;
      DynMemWrapper< Expression > expr;
    };
    struct Literal
    {
      enum Type
      {
        Char,
        String,
        Number
      };

      std::string literal;
      std::string suffix;
      Type type;
      typing::Type result_type;
    };
    struct Expression
    {
      using operand = boost::variant2::variant< DynMemWrapper< Expression >, Variable,
            Declaration, Literal, CastOperation, Body >;

      const Operator* operation;
      typing::Type result_type;
      std::vector< operand > operands;

      Expression();
      Expression(Variable var);
      Expression(Declaration decl);
      Expression(Literal lit);
      Expression(CastOperation cast);
      Expression(Body body);
      Expression(const Operator* op, std::vector< operand > opers);

      bool empty() const noexcept;
      bool full() const noexcept;
    };

    bool is_body(const Expression& expr);
    bool is_decl(const Expression& expr);
    bool is_operator(const Expression& expr);
    bool is_bridge(const Expression& expr);

    void remove_bridge(Expression& expr);
    const typing::Type* get_type(const Expression::operand& expr);
    bool is_lvalue(const Expression::operand& operand);
    bool is_lvalue(const Expression* expr);

    template< class T >
    const std::string& get_name(const T& entity)
    {
      return entity.name;
    }
    const std::string& get_name(const std::string& identity);
  }
}

#endif

