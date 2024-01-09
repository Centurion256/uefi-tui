//
// Created by paul on 09/01/24.
//

/** @file
    A simple, basic, EDK II native, snake game using ncurses ported to UEFI
**/
#include  <Uefi.h>
#include  "NcursesLib.h"
/***
  Print a welcoming message.

  Establishes the main structure of the application.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
INTN
        EFIAPI
ShellAppMain (
        IN UINTN Argc,
        IN CHAR16 **Argv
)
{
PrintCharAt(L'X');
UINTN index;
EFI_INPUT_KEY *key = AllocateRuntimeZeroPool(sizeof(EFI_INPUT_KEY));
// wait until arrow-up key is pressed
while (key->ScanCode != SCAN_UP) {

gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &index);
gST->ConIn->ReadKeyStroke(gST->ConIn, key);
Print(L"Key pressed: %c\n", key->UnicodeChar);
}
FreePool(key);

return(0);
}
