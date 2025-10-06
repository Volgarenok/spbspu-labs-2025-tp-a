#include "consoleOptions.hpp"
#include <iostream>

void holodilov::printHelpOption(std::ostream& os)
{
  os << "Usage:\n\n";
  os << "CreateDict <dictionary name> <language>\n";
  os << "\tCreate new dictionary.\n\n";

  os << "ExportDict <dictionary name> <file name>\n";
  os << "\tWrite dictionary to the file.\n\n";

  os << "ImportDict <file name>\n";
  os << "\tRead dictionary from file. Dictionary name and language will be taken from file.\n\n";

  os << "ClearDict <dictionary name>\n";
  os << "\tClear the dictionary.\n\n";

  os << "DeleteDict <dictionary name>\n";
  os << "\tDelete dictionary from program memory.\n\n";

  os << "AddWord <dictionary name> <english word> <amount translations> <translation1> translation2> ...\n";
  os << "\tAdd word with its translations to the dictionary.\n\n";

  os << "DeleteWord <dictionary name> <english word>\n";
  os << "\tDelete english word and all its translations from the dictionary.\n\n";

  os << "UpdateWord <dictionary name> <english word> <new english word>\n";
  os << "\tChange english word writing int the dictionary.\n\n";

  os << "FindWord <english word>\n";
  os << "\tFind this english word among all the dictionaries. Print all the translations ";
  os << "of this word to the console.\n\n";

  os << "PrintDict <dictionary name>\n";
  os << "\tPrint dictionary name, language and its words with translations to the consoole.\n\n";

  os << "PrintDictNames\n";
  os << "\tPrint names of all the dictionaries in the program.\n\n";

  os << "UpdateWord <dictionary name> <english word> <new english word>\n";
  os << "\tChange english word writing int the dictionary.\n\n";

  os << "Merge <dictionary 1> <dictionary 2> <new dictionary> <new dictionary language>\n";
  os << "\tMerge two dictionaries into new dictionary with new name and language.\n\n";

  os << "Intersect <dictionary 1> <dictionary 2> <new dictionary> <new dictionary language>\n";
  os << "\tFind common words for two dictionaries and save them in the new dictionary.\n\n";

  os << "ExportAlphabet <dictionary name> <alphabet file name>\n";
  os << "\tAlphabet is all the characters of translations of the dictionary. This command creates alphabet ";
  os << "for the dictionary and save it in file.\n\n";

  os << "CheckAlphabet <dictionary name> <alphabet file name>\n";
  os << "\tCheck that dictionary has as equal alphabet as in the alphabet file.\n\n";

  os << "------------------------------------------------\n";
}
