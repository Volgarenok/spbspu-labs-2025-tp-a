#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <cstddef>
#include <utility>

namespace rychkov
{
  template< class T >
  struct DinMemWrapper: std::unique_ptr< T >
  {
    using value_type = T;
    DinMemWrapper():
      std::unique_ptr< T >(new T)
    {}
    DinMemWrapper(const DinMemWrapper& rhs):
      std::unique_ptr< T >(rhs != nullptr ? new T{*rhs} : nullptr)
    {}
    DinMemWrapper(DinMemWrapper&&) = default;
    DinMemWrapper(std::nullptr_t):
      std::unique_ptr< T >(nullptr)
    {}
    DinMemWrapper(T* ptr):
      std::unique_ptr< T >(ptr)
    {}
    template< class... Args >
    DinMemWrapper(Args&&... args):
      std::unique_ptr< T >(new T{std::forward< Args >(args)...})
    {}
    DinMemWrapper& operator=(const DinMemWrapper& rhs)
    {
      if (rhs != nullptr)
      {
        static_cast< std::unique_ptr< T >& >(*this).reset(new T{*rhs});
      }
      return *this;
    }
    DinMemWrapper& operator=(DinMemWrapper&&) = default;
    template< class U = T >
    DinMemWrapper& operator=(U&& rhs)
    {
      static_cast< std::unique_ptr< T >& >(*this).reset(new T{std::forward< U >(rhs)});
      return *this;
    }
    DinMemWrapper& operator=(std::nullptr_t)
    {
      static_cast< std::unique_ptr< T >& >(*this) = nullptr;
      return *this;
    }
    DinMemWrapper& operator=(T* ptr)
    {
      static_cast< std::unique_ptr< T >& >(*this).reset(ptr);
      return *this;
    }
  };

  struct Operator
  {
    enum Type
    {
      unary = 1,
      binary = 2,
      ternary = 3,
      multiple = -1
    };
    enum Category
    {
      arithmetic, // +, -, *, ...
      logic, // &&, ||, !, <, >, >=, <=, ==
      bit, // &, |, ~, ^, <<, ...
      address, // ., ->, *, &
      special, // (), [], ?:, sizeof, ','
      assign // =
    };

    Type type;
    Category category;
    std::string token;
    bool require_lvalue = false;
    bool right_align = false;
    int priority = 0;
  };
  namespace typing
  {
    struct Type
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
        Struct,
        Int,
        Pointer,
        Array,
        Function,
        Combination
      };

      std::string name;
      Category category = Combination;
      DinMemWrapper< Type > base = nullptr;
      bool is_const = false;
      bool is_volatile = false;
      bool is_signed = false;
      bool is_unsigned = false;
      LengthCategory length_category = NO_LENGTH;
      bool array_has_length = false;
      size_t array_length = 0;
      std::vector< Type > function_parameters;

      bool empty() const noexcept;
    };
  }
  namespace entities
  {
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
    struct Struct
    {
      std::string name;
      std::vector< Variable > fields;
    };
    struct Enum
    {
      std::string name;
      std::vector< std::pair< std::string, int > > fields;
    };
    struct Union
    {
      std::string name;
      std::vector< Variable > fields;
    };
    struct Alias
    {
      typing::Type type;
      std::string name;
    };
    struct Declaration
    {
      std::variant< Variable, Struct, Enum, Union, Alias, Function > data;
      DinMemWrapper< Expression > value;
    };
    struct CastOperation
    {
      typing::Type from, to;
      bool is_explicit = false;
      DinMemWrapper< Expression > expr;
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
    };
    struct Expression
    {
      using operand = std::variant< DinMemWrapper< Expression >, Variable, Declaration, Literal, CastOperation, Body >;

      const Operator* operation;
      typing::Type result_type;
      std::vector< operand > operands;

      Expression();
      Expression(Variable var);
      Expression(Declaration decl);
      Expression(Literal lit);
      Expression(CastOperation cast);
      Expression(Body body);
      Expression(const Operator* op, typing::Type result, std::vector< operand > opers);

      bool empty() const noexcept;
      bool full() const noexcept;
    };

    bool is_body(const entities::Expression& expr);
    bool is_decl(const entities::Expression& expr);
  }
}

#endif

