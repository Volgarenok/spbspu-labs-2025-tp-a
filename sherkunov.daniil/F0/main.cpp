#include "Xref.h"

int main(int argc, char **argv)
{
  sherkunov::CrossReferenceSystem system;

  int result = sherkunov::processCommandLineArguments(argc, argv, system);
  if (result != -1) {
    return result;
  }

  sherkunov::runInteractiveMode(system);
  return 0;
}
