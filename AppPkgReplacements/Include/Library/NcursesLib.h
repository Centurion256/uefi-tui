//
// Created by paul on 07/01/24.
//

#ifndef UEFI_TUI_NCURSES_H
#define UEFI_TUI_NCURSES_H

#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/MemoryAllocationLib.h>
#include  <Library/UefiBootServicesTableLib.h>
#include  <Library/UefiRuntimeServicesTableLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/BaseLib.h>

UINTN PrintCharAt (
        CHAR16    Character
);

#endif //UEFI_TUI_NCURSES_H
