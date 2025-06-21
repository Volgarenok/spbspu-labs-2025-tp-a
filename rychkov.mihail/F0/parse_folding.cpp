#include "cparser.hpp"

void rychkov::CParser::fold(CParseContext& context, const Operator* reference)
{
  if ((reference != nullptr) && entities::is_bridge(*stack_.top()))
  {
    stack_.push(move_up());
    stack_.top()->operation = reference;
    return;
  }
  entities::Expression* last = stack_.top();
  while (entities::is_operator(*stack_.top()))
  {
    if (!stack_.top()->full())
    {
      log(context, "found not full operator during priority folding");
    }
    if ((reference != nullptr) && ((stack_.top()->operation->priority > reference->priority)
        || ((stack_.top()->operation->priority == reference->priority) && reference->right_align)))
    {
      move_down(context);
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
