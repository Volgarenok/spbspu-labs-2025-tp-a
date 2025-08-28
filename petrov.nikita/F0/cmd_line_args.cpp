#include "cmd_line_args.hpp"
#include <cstring>
#include <string>
#include <fstream>
#include <iomanip>

namespace
{
  bool isDigit(const char * arg)
  {
    bool is_digit = true;
    for (size_t i = 0; i < strlen(arg) && is_digit; i++)
    {
      if (!bool(isdigit(arg[i])))
      {
        is_digit = false;
      }
    }
    return is_digit;
  }
}

void petrov::outputHelp()
{
  std::string text;
  std::ifstream input("help.txt");
  if (input.is_open())
  {
    while (std::getline(input, text))
    {
      std::cout << text << "\n";
    }
  }
  input.close();
}

void petrov::setPrecision(std::ostream & out, const size_t & precision)
{
  out << std::fixed << std::setprecision(precision);
}

void petrov::processCmdLineAgruments(const int argc, const char * const * argv)
{
  if (argc == 2)
  {
    outputHelp();
  }
  else
  {
    if (isDigit(argv[2]))
    {
      size_t precision = std::atoi(argv[2]);
      setPrecision(std::cout, precision);
    }
  }
}

