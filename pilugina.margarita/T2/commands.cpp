#include "commands.hpp"
#include <iostream>

void pilugina::outputBinRepresentation(unsigned long long num)
{
  if (num == 1)
  {
    std::cout << "01";
    return;
  }
  else if (num == 0)
  {
    std::cout << "0";
    return;
  }

  while (num > 0)
  {
    std::cout << num % 2;
    num /= 2;
  }
}

bool pilugina::isBinaryDigit(char c)
{
  return c == '0' || c == '1';
}

bool pilugina::isOctalDigit(char c)
{
  return c >= '0' && c <= '7';
}
