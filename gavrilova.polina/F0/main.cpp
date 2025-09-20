#include <iostream>
#include "CommandProcessor.hpp"
#include "StudentsDataBase.hpp"

int main(int argc, char* argv[])
{
  gavrilova::StudentDatabase db;

  if (argc > 1) {
    std::cout << "Загрузка данных из файла: " << argv[1] << '\n';
    if (!db.loadFromFile(argv[1])) {
      std::cerr << "Ошибка: не удалось загрузить данные из файла " << argv[1] << '\n';
      return 1;
    }
  }

  gavrilova::CommandProcessor processor(db);
  processor.run();

  return 0;
}
