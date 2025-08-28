#include "print_content.hpp"

#include <iostream>

void rychkov::ContentPrinter::operator()(const DynMemWrapper< entities::Expression >& expr)
{
  operator()(*expr);
}
void rychkov::ContentPrinter::operator()(const entities::Expression::operand& oper)
{
  boost::variant2::visit(*this, oper);
}
void rychkov::ContentPrinter::operator()(const entities::Expression& expr)
{
  if (expr.operation == nullptr)
  {
    if (expr.operands.empty())
    {
      print_empty();
      return;
    }
    operator()(expr.operands[0]);
    return;
  }
  size_t oper_size = expr.operation->type;
  if (expr.operation->type == Operator::UNARY)
  {
    indent() << "* [unary] operator" << expr.operation->token;
    out << (expr.operation->right_align ? " <prefix form>\n" : " <suffix form>\n");
  }
  else if (expr.operation->type == Operator::BINARY)
  {
    indent() << "* [binary] operator" << expr.operation->token << '\n';
  }
  else if (expr.operation->type == Operator::TERNARY)
  {
    indent() << "* [ternary] operator" << expr.operation->token << '\n';
  }
  else
  {
    indent() << "* [multi] operator" << expr.operation->token << " - size=" << expr.operands.size() << '\n';
    oper_size = expr.operands.size();
  }
  if (expr.result_type.ready())
  {
    indent() << (entities::is_lvalue(&expr) ? "<type(l)> " : "<type> ") << expr.result_type << '\n';
  }
  else
  {
    indent() << "<type> [NONE]\n";
  }
  indent() << "<operands>:\n";
  indent_++;
  for (size_t i = 0; i < oper_size; i++)
  {
    if (i < expr.operands.size())
    {
      operator()(expr.operands[i]);
    }
    else
    {
      print_empty();
    }
  }
  indent_--;
}
