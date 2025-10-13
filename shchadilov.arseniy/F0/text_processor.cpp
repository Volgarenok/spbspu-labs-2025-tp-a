#include "text_processor.hpp"
#include <algorithm>
#include <cctype>
#include <iterator>
#include <functional>

namespace shchadilov
{
  struct IsPunctuation
  {
    bool operator()(char c) const
    {
      static const std::string punctuation = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
      return punctuation.find(c) != std::string::npos;
    }
  };

  struct ToLower
  {
    char operator()(char c) const
    {
      return std::tolower(static_cast<unsigned char>(c));
    }
  };

  std::string cleanWord(const std::string& word)
  {
    std::string cleaned = "";;
    std::remove_copy_if(word.begin(), word.end(), std::back_inserter(cleaned), IsPunctuation{});
    std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(), ToLower{});
    return cleaned;
  }

  class WordAccumulator
  {
  public:
    explicit WordAccumulator(std::vector<std::string>& words):
      words_(words)
    {}

    void operator()(char c)
    {
      if (std::isspace(c) || isPunctuation_(c))
      {
        if (!currentWord_.empty())
        {
          words_.push_back(cleanWord(currentWord_));
          currentWord_.clear();
        }
      }
      else
      {
        currentWord_ += c;
      }
    }

    void finish()
    {
      if (!currentWord_.empty())
      {
        words_.push_back(cleanWord(currentWord_));
      }
    }

  private:
    std::vector< std::string >& words_;
    std::string currentWord_ = "";;
    IsPunctuation isPunctuation_;
  };

  std::vector< std::string > extractWords(const std::string& text)
  {
    std::vector< std::string > words;
    if (text.empty())
    {
      return words;
    }
    WordAccumulator accumulator(words);
    std::for_each(text.begin(), text.end(), std::ref(accumulator));
    accumulator.finish();
    return words;
  }
}
