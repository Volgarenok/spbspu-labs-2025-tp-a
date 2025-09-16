#ifndef PRINT_CONTENT_HPP
#define PRINT_CONTENT_HPP

#include <iosfwd>
#include <string>
#include "content.hpp"

namespace rychkov
{
  class ContentPrinter
  {
  public:
    std::ostream& out;
    ContentPrinter(std::ostream& ostream, size_t start_indent = 0);

    void operator()(const std::string& any);
    void operator()(const Macro& macro);
    void operator()(const entities::Variable& var);
    void operator()(const entities::Function& func);
    void operator()(const entities::Body& body);
    void operator()(const entities::Statement& statement);
    void operator()(const entities::Struct& structure);
    void operator()(const entities::Enum& structure);
    void operator()(const entities::Union& structure);
    void operator()(const entities::Alias& alias);
    void operator()(const entities::Declaration& decl);

    void operator()(const entities::Literal& literal);
    void operator()(const entities::CastOperation& cast);

    void operator()(const DynMemWrapper< entities::Expression >& root);
    void operator()(const entities::Expression& root);
    void operator()(const entities::Expression::operand& operand);

    std::ostream& indent();
    void print_empty();
  private:
    size_t indent_;
  };

  namespace details
  {
    std::ostream& print_left_parenthesis(std::ostream& out, const typing::Type& parent);
    std::ostream& print_right_parenthesis(std::ostream& out, const typing::Type& parent);
  }
  namespace typing
  {
    std::ostream& print_left(std::ostream& out, const Type& type);
    std::ostream& print_right(std::ostream& out, const Type& type);
    std::ostream& operator<<(std::ostream& out, const Type& type);
  }
  namespace entities
  {
    std::ostream& operator<<(std::ostream& out, const Variable& var);
    std::ostream& operator<<(std::ostream& out, const Function& func);
    std::ostream& operator<<(std::ostream& out, const Literal& literal);
  }
}

#endif
