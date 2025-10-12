#include "cparser.hpp"

void rychkov::CParser::fold(CParseContext& context, const Operator* reference)
{
  if ((reference != nullptr) && entities::is_bridge(*stack_.top()))
  {
    move_up_down();
    stack_.top()->operation = reference;
    return;
  }
  entities::Expression* last = stack_.top();
  while (entities::is_operator(*stack_.top()))
  {
    if ((stack_.top()->operation == &inline_if) && (reference == &inline_if))
    {
      return;
    }
    if (!stack_.top()->full())
    {
      log(context, "found not full operator during priority folding");
    }
    if ((reference != nullptr) && ((stack_.top()->operation->priority > reference->priority)
        || ((stack_.top()->operation->priority == reference->priority) && reference->right_align)))
    {
      move_down();
      stack_.top()->operation = reference;
      return;
    }
    last = stack_.top();
    calculate_type(context, *last);
    stack_.pop();
  }
  if (reference != nullptr)
  {
    stack_.push(last);
    move_up();
    stack_.top()->operation = reference;
    return;
  }
}
rychkov::entities::Expression* rychkov::CParser::move_up()
{
  entities::Expression* temp = new entities::Expression{std::move(*stack_.top())};
  stack_.top()->operation = nullptr;
  stack_.top()->operands.push_back(temp);
  return temp;
}
void rychkov::CParser::move_down()
{
  entities::Expression* temp = new entities::Expression{nullptr, {std::move(stack_.top()->operands.back())}};
  stack_.top()->operands.back() = temp;
  stack_.push(temp);
}
void rychkov::CParser::move_up_down()
{
  stack_.push(move_up());
}

void rychkov::CParser::clear_scope()
{
  clear_scope(base_types, stack_.size());
  clear_scope(variables, stack_.size());
  clear_scope(structs, stack_.size());
  clear_scope(unions, stack_.size());
  clear_scope(enums, stack_.size());
}
