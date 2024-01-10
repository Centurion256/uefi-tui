//
// Created by paul on 07/01/24.
//

#include "Library/NcursesLib.h"

/**
  Prints a character to the default console, at
  the supplied cursor position, using L"%c" format.

  @param  Column     The cursor position to print the string at.
  @param  Row        The cursor position to print the string at.
  @param  Character  Character to print.

  @return Length of string printed to the console.

**/
UINTN
PrintCharAt (
        CHAR16    Character
)
{
    return Print(L"%c", Character);
}