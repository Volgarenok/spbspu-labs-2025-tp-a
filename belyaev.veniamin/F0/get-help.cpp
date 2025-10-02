#include "get-help.hpp"

void belyaev::getHelp(std::ostream& out)
{
  out << "1. INSERT <dictionary> <russian_word> <translation>\n"
      << "   Добавляет запись с переводом в словарь. Если"
      << "   словаря \"dictionary\" не существует, то такой"
      << "   создается. Если в словаре такая запись уже"
      << "   существует, то вывести сообщение"
      << "   <THIS ENTRY ALREADY EXISTS>\n\n"

      << "2. REMOVE <dictionary> <russian_word> <translation>"
      << "   Удаляет из словаря запись с переводом слова.\n"
      << "   Если в словаре таких не существует, то вывести\n"
      << "   сообщение <THIS ENTRY DOES NOT EXIST>. Если\n"
      << "   словаря не существует, вывести\n"
      << "   <THIS DICTIONARY DOES NOT EXIST>.\n\n"

      << "3. SEARCH <dictionary> <russian_word>\n"
      << "   Ищет в словаре записи с переводом слова и\n"
      << "   выводит их. Если поиск неудачен, то вывести\n"
      << "   сообщение <THIS ENTRY DOES NOT EXIST>.\n\n"

      << "4. SEARCH_BY_ENGLISH <dictionary> <english_word>\n"
      << "   Ищет в словаре записи, где переводом является\n"
      << "   указанное слово, и выводит их. Если поиск\n"
      << "   неудачен, то вывести сообщение\n"
      << "   <THIS ENTRY DOES NOT EXIST>.\n\n"

      << "5. SEARCH_СONTAINS <dictionary> <russian_characters>\n"
      << "   Ищет в словаре записи, где в русском слове\n"
      << "   содержится указанный порядок символов. Если\n"
      << "   поиск неудачен, то вывести сообщение\n"
      << "   <NO ENTRIES>.\n\n"

      << "6. SEARCH_CONTAINS_ENGLISH <dictionary>\n"
      << "   <english_characters>\n"
      << "   Ищет в словаре записи, где в переводе\n"
      << "   содержится указанный порядок символов. Если\n"
      << "   поиск неудачен, то вывести сообщение\n"
      << "   <NO ENTRIES>.\n\n"

      << "7. PRINT <dictionary>\n"
      << "   Выводит все записи из словаря. Если словаря\n"
      << "   не существует, то вывести сообщение\n"
      << "   <THIS DICTIONARY DOES NOT EXIST>.\n\n"

      << "8. PRINT_ALL\n"
      << "   Выводит все записи в системе.\n\n"

      << "9. CLEAR <dictionary>\n"
      << "   Очищает все записи в словаре, как следствие,\n"
      << "   удаляя его. Если словаря не существует, то\n"
      << "   вывести сообщение\n"
      << "   <THIS DICTIONARY DOES NOT EXIST>.\n\n"

      << "10. CLEAR_ALL\n"
      << "    Очищает все записи в системе.\n\n"

      << "11. COPY <dictionary_old> <dictionary_new>\n"
      << "    Копирует словарь \"dictionary_old\", присваивая\n"
      << "    новому имя \"dictionary_new\". Если словаря в\n"
      << "    первом поле не существует, то вывести сообщение\n"
      << "    <THIS DICTIONARY DOES NOT EXIST>.\n\n"

      << "12. RENAME <dictionary_old> <dictionary_new>\n"
      << "    Переименовывает словарь \"dictionary_old\",\n"
      << "    присваивая новому имя \"dictionary_new\". Если\n"
      << "    словаря в первом поле не существует, то вывести\n"
      << "    сообщение <THIS DICTIONARY DOES NOT EXIST>.\n\n"

      << "13. MERGE <N> <dictionary_new> <dictionary1> ...\n"
      << "    <dictionaryN>\n"
      << "    N - натуральное число больше 1. Команда\n"
      << "    совмещает все записи в словарях с 1 по N в\n"
      << "    новом словаре \"dictionary_new\". Если одного из\n"
      << "    словарей не существует, то вывести сообщение\n"
      << "    <ONE OF THE DICTIONARIES DOES NOT EXIST>.\n\n"

      << "14. INTERSECT <N> <dictionary_new> <dictionary1>\n"
      << "    ... <dictionaryN>\n"
      << "    N - натуральное число больше 1. Сохраняет все\n"
      << "    пересечения словарей с 1 по N в новый словарь\n"
      << "    \"dictionary_new\". Если одного из словарей не\n"
      << "    существует, то вывести сообщение\n"
      << "    <ONE OF THE DICTIONARIES DOES NOT EXIST>.\n\n"

      << "15. DELETE_INTERSECTIONS <N> <dictionary_new>\n"
      << "    <dictionary1> ... <dictionaryN>\n"
      << "    N - натуральное число больше 1. Удаляет всe\n"
      << "    пересечения в первом словаре с остальными, с\n"
      << "    2 по N. Если одного из словарей не существует,\n"
      << "    то вывести сообщение\n"
      << "    <ONE OF THE DICTIONARIES DOES NOT EXIST>.\n\n"

      << "16. HELP\n"
      << "    Вывод всего списка команд со справкой.\n";
}

