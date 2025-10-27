#include "functions.h"
#include <algorithm>
#include <iostream>

namespace
{
  std::vector< std::string > splitWords(const std::string &text)
  {
    std::vector< std::string > words;
    std::string word;
    for (char c: text)
    {
      if (std::isalpha(static_cast< unsigned char >(c)))
      {
        word.push_back(static_cast< char >(std::tolower(static_cast< unsigned char >(c))));
      }
      else
      {
        if (!word.empty())
        {
          words.push_back(word);
          word.clear();
        }
      }
    }
    if (!word.empty()) words.push_back(word);
    return words;
  }

  void printTopN(const std::map< std::string, int > &freq, int N)
  {
    std::vector< std::pair< std::string, int > > v(freq.begin(), freq.end());
    std::sort(v.begin(), v.end(), [](auto &a, auto &b) { return a.second > b.second; });

    for (int i = 0; i < N && i < static_cast< int >(v.size()); ++i)
    {
      std::cout << v[i].first << ": " << v[i].second << std::endl;
    }
  }
}

void asafov::analyze_letters(State &state, const std::string &name)
{
  const auto it = state.texts.find(name);
  if (it == state.texts.end())
  {
    std::cout << "Текст " << name << " не найден." << std::endl;
    return;
  }

  const auto start = std::chrono::high_resolution_clock::now();
  std::map< std::string, int > freq;
  for (const char c: it->second)
  {
    if (std::isalpha(static_cast< unsigned char >(c)))
    {
      std::string s(1, static_cast< char >(std::tolower(static_cast< unsigned char >(c))));
      freq[s]++;
    }
  }
  const auto end = std::chrono::high_resolution_clock::now();
  state.lastDuration = end - start;

  state.lastResults.clear();
  state.lastResults[name] = freq;
  state.history.push_back("analyze_letters " + name);

  for (auto &p: freq)
  {
    std::cout << p.first << ": " << p.second << std::endl;
  }
}

void asafov::analyze_words(State &state, const std::string &name)
{
  auto it = state.texts.find(name);
  if (it == state.texts.end())
  {
    std::cout << "Текст " << name << " не найден." << std::endl;
    return;
  }

  auto start = std::chrono::high_resolution_clock::now();
  auto words = splitWords(it->second);
  std::map< std::string, int > freq;
  for (auto &w: words) freq[w]++;
  auto end = std::chrono::high_resolution_clock::now();
  state.lastDuration = end - start;

  state.lastResults.clear();
  state.lastResults[name] = freq;
  state.history.push_back("analyze_words " + name);

  for (auto &p: freq)
  {
    std::cout << p.first << ": " << p.second << std::endl;
  }
}

void asafov::analyze_letters_top(State &state, const std::string &name, int N)
{
  const auto it = state.texts.find(name);
  if (it == state.texts.end())
  {
    std::cout << "Текст " << name << " не найден." << std::endl;
    return;
  }

  std::map< std::string, int > freq;
  for (const char c: it->second)
  {
    if (std::isalpha(static_cast< unsigned char >(c)))
    {
      std::string s(1, static_cast< char >(std::tolower(static_cast< unsigned char >(c))));
      freq[s]++;
    }
  }

  state.lastResults.clear();
  state.lastResults[name] = freq;
  state.history.push_back("analyze_letters_top " + name + " " + std::to_string(N));

  printTopN(freq, N);
}

void asafov::analyze_words_top(State &state, const std::string &name, int N)
{
  auto it = state.texts.find(name);
  if (it == state.texts.end())
  {
    std::cout << "Текст " << name << " не найден." << std::endl;
    return;
  }

  auto words = splitWords(it->second);
  std::map< std::string, int > freq;
  for (auto &w: words) freq[w]++;

  state.lastResults.clear();
  state.lastResults[name] = freq;
  state.history.push_back("analyze_words_top " + name + " " + std::to_string(N));

  printTopN(freq, N);
}

void asafov::count_symbols(State &state, const std::string &name)
{
  auto it = state.texts.find(name);
  if (it == state.texts.end())
  {
    std::cout << "Текст " << name << " не найден." << std::endl;
    return;
  }
  std::cout << "Символов: " << it->second.size() << std::endl;
  state.history.push_back("count_symbols " + name);
}

void asafov::count_words(State &state, const std::string &name)
{
  auto it = state.texts.find(name);
  if (it == state.texts.end())
  {
    std::cout << "Текст " << name << " не найден." << std::endl;
    return;
  }
  auto words = splitWords(it->second);
  std::cout << "Слов: " << words.size() << std::endl;
  state.history.push_back("count_words " + name);
}
