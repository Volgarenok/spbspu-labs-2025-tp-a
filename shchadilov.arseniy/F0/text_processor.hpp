#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>
#include <vector>


namespace shchadilov
{
  std::string cleanWord(const std::string& word);
  std::vector< std::string > extractWords(const std::string& text);
}

#endif
