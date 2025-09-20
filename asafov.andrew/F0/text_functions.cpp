#include "functions.h"
#include <iostream>
#include <fstream>

namespace
{
  std::string readFileToString(const std::string &filename)
  {
    std::ifstream ifs(filename);
    if (!ifs)
    {
      return "";
    }
    std::string content((std::istreambuf_iterator< char >(ifs)), std::istreambuf_iterator< char >());
    return content;
  }
}

void asafov::loadtext(State &state, const std::string &filename)
{
  const std::string text = readFileToString(filename);
  if (text.empty())
  {
    std::cout << "Ошибка: не удалось загрузить файл " << filename << std::endl;
    return;
  }
  state.texts[filename] = text;
  state.history.push_back("loadtext " + filename);
  std::cout << "Файл " << filename << " успешно загружен." << std::endl;
}

void asafov::list_texts(State &state)
{
  for (auto &p: state.texts)
  {
    std::cout << p.first << " (" << p.second.size() << " символов)" << std::endl;
  }
  state.history.emplace_back("list_texts");
}

void asafov::delete_text(State &state, const std::string &name)
{
  if (state.texts.erase(name))
  {
    std::cout << "Текст " << name << " удален." << std::endl;
  }
  else
  {
    std::cout << "Текст " << name << " не найден." << std::endl;
  }
  state.history.push_back("delete_text " + name);
}
