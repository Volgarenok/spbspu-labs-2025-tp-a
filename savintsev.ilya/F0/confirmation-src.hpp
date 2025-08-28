#ifndef CONFIRMATION_SRC_HPP
#define CONFIRMATION_SRC_HPP
#include <iostream>
#include <string>

namespace savintsev
{
  class ConfirmationPrompt
  {
  public:
    ConfirmationPrompt(std::istream & in, std::ostream & out);
    bool ask(const std::string & message);
  private:
    std::istream & in_;
    std::ostream & out_;

    bool is_yes(const std::string & answer);
    bool is_no(const std::string & answer);
  };
}

#endif
