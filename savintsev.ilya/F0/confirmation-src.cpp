#include "confirmation-src.hpp"

savintsev::ConfirmationPrompt::ConfirmationPrompt(std::istream & in, std::ostream & out):
  in_(in),
  out_(out)
{
  in_.clear();
  out_.clear();
}

bool savintsev::ConfirmationPrompt::ask(const std::string & message)
{
  std::string answer;
  while (true)
  {
    out_ << message << " [Y / N]: ";
    in_ >> answer;

    if (is_yes(answer))
    {
      return true;
    }
    if (is_no(answer))
    {
      return false;
    }
  }
}

bool savintsev::ConfirmationPrompt::is_yes(const std::string & answer)
{
  return !answer.empty() && (answer[0] == 'Y' || answer[0] == 'y');
}

bool savintsev::ConfirmationPrompt::is_no(const std::string & answer)
{
  return !answer.empty() && (answer[0] == 'N' || answer[0] == 'n');
}
