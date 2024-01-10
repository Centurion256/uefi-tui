//
// Created by paul on 07/01/24.
//

#include "Library/NcursesLib.h"

// Global variable to hold the WINDOW structure
WINDOW *gWindow = NULL;

EFI_SIMPLE_TEXT_INPUT_PROTOCOL *gSimpleTextIn;
//EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *gSimpleTextInEx;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *gSimpleTextOut;


WINDOW* initscr(void)
{
    // Locate the Simple Text Input and Output protocols
    EFI_STATUS Status = gBS->LocateProtocol(
            &gEfiSimpleTextInProtocolGuid,
            NULL,
            (VOID **)&gSimpleTextIn
    );
    if (EFI_ERROR(Status)) {
        return NULL;
    }

    Status = gBS->LocateProtocol(
            &gEfiSimpleTextOutProtocolGuid,
            NULL,
            (VOID **)&gSimpleTextOut
    );
    if (EFI_ERROR(Status)) {
        return NULL;
    }

//    Status = gBS->LocateProtocol(
//            &gEfiSimpleTextInputExProtocolGuid,
//            NULL,
//            (VOID **)&gSimpleTextInEx
//    );
//    if (EFI_ERROR(Status)) {
//        return NULL;
//    }

    // Allocate memory for the WINDOW structure
    gWindow = AllocatePool(sizeof(WINDOW));
    if (gWindow == NULL) {
        return NULL;
    }

    // Initialize the WINDOW structure
//    win->SimpleTextOut = gSimpleTextOut;
    UINTN ScreenWidth;
    UINTN ScreenRows;

    gST->ConOut->SetCursorPosition (gST->ConOut,0,0);
    gST->ConOut->QueryMode (gST->ConOut, gST->ConOut->Mode->Mode, &ScreenWidth, &ScreenRows );
    gWindow->height = ScreenRows;
    gWindow->width = ScreenWidth;
    gWindow->cursor_y = 0;
    gWindow->cursor_x = 0;
    gWindow->content = AllocatePool(sizeof(CHAR16) * (ScreenRows - 1) * ScreenWidth);
    gWindow->attributes = AllocatePool(sizeof(UINTN*) * (ScreenRows - 1) * ScreenWidth);
    UINTN index;
    for (index = 0; index < (ScreenRows - 1) * ScreenWidth; index++) {
        gWindow->content[index] = L' ';
        gWindow->attributes[index] = 0;
    }

    gWindow->SimpleTextOut = gSimpleTextOut;
//    gWindow->SimpleTextInEx = gSimpleTextInEx;

    return gWindow;
}


// End ncurses mode
UINTN endwin(void) {
    if (gWindow == NULL) {
        return -1; // curses mode not initialized
    }


    FreePool(gWindow->content);
    FreePool(gWindow->attributes);

    FreePool(gWindow);

    return 0;
}



// Enter raw mode
UINTN raw(void) {
    if (gWindow == NULL || gWindow->SimpleTextInEx == NULL) {
        return -1; // curses mode not initialized or SimpleTextInEx not available
    }

    EFI_STATUS Status = gWindow->SimpleTextInEx->SetState(gWindow->SimpleTextInEx, NULL);

    return EFI_ERROR(Status) ? -1 : 0;
}

// Enter cbreak mode
UINTN cbreak(void) {
    if (gWindow == NULL || gWindow->SimpleTextInEx == NULL) {
        return -1; // curses mode not initialized or SimpleTextInEx not available
    }

    // Set the console input mode to non-raw
    EFI_STATUS Status = gWindow->SimpleTextInEx->Reset(gWindow->SimpleTextInEx, FALSE);
    
    return EFI_ERROR(Status) ? -1 : 0;
}

// Enable echoing of characters typed by the user
UINTN echo(void) {
    if (gWindow == NULL || gWindow->SimpleTextInEx == NULL) {
        return -1; // curses mode not initialized or SimpleTextInEx not available
    }

    // Enable echoing
    gWindow->SimpleTextInEx->SetState(gWindow->SimpleTextInEx, NULL);

    return 0;
}

// Disable echoing
UINTN noecho(void) {
    if (gWindow == NULL || gWindow->SimpleTextInEx == NULL) {
        return -1; // curses mode not initialized or SimpleTextInEx not available
    }

    // Disable echoing
    gWindow->SimpleTextInEx->SetState(gWindow->SimpleTextInEx, NULL);

    return 0;
}

UINTN keypad(WINDOW *win, BOOLEAN enable) {
    if (win == NULL || win->SimpleTextInEx == NULL) {
        return -1; // curses mode not initialized or SimpleTextInEx not available
    }

    if (enable) {
        win->SimpleTextInEx->Reset(win->SimpleTextInEx, FALSE); // Enable the input
    } else {
        win->SimpleTextInEx->Reset(win->SimpleTextInEx, TRUE); // Disable the input
    }

    return 0;
}


EFI_STATUS addch(CHAR16 ch)
{
    // Put a single character into the current cursor location
    UINTN index = gWindow->cursor_y * gWindow->width + gWindow->cursor_x;
    gWindow->content[index] = ch;
    gWindow->attributes[index] = 0;
    gWindow->cursor_x++;
    return EFI_SUCCESS;
}

EFI_STATUS mvaddch(UINTN y, UINTN x, CHAR16 ch)
{
    // Move cursor to a given poUINTN and then print
    gWindow->cursor_y = y;
    gWindow->cursor_x = x;
    return addch(ch);
}

void refresh(void)
{
    // Actually prUINTN the window
//    EFI_STATUS Status;
//    UINTN index;
    gWindow->SimpleTextOut->ClearScreen(gWindow->SimpleTextOut);
    gWindow->SimpleTextOut->SetCursorPosition(gWindow->SimpleTextOut, 0, 0);
//    for (index = 0; index < gWindow->width * (gWindow->height - 1); index++) {
////        gWindow->SimpleTextOut->SetAttribute(gWindow->SimpleTextOut, gWindow->attributes[index]);
////        gWindow->SimpleTextOut->OutputString(gWindow->SimpleTextOut, L"%c", gWindow->content[index]);
//    }
    gWindow->SimpleTextOut->OutputString(gWindow->SimpleTextOut, gWindow->content);

}

// UINTN mvprintw(UINTN y, UINTN x, const CHAR16 *format, ...)
// {
//     // Move cursor to a position and then print
//     gWindow->cursor_y = y;
//     gWindow->cursor_x = x;
//     VA_LIST args;

//     return printw(format, args);
// }

UINTN printw(const CHAR16 *format, ...)
{
    VA_LIST args;
    UINTN returnVal;
    CHAR16 Buffer[1024]; // Buffer to hold the output

    // Initialize the argument list
    VA_START(args, format);

    // Print the formatted output to the buffer
    returnVal = UnicodeVSPrint(Buffer, sizeof(Buffer), format, args);

    // Clean up the argument list
    VA_END(args);

    for (UINTN i = 0; i < returnVal; i++) {
        addch(Buffer[i]);
    }

    return returnVal;
}

UINTN addstr(const CHAR16 *str)
{
    // Put a character string into a window
    UINTN len = StrLen(str);
    for (UINTN i = 0; i < len; i++) {
        addch(str[i]);
    }

    return len;
}

UINTN addnstr(const CHAR16 *str, UINTN n)
{
    // Put at most n characters into the screen
    UINTN len = StrnLenS(str, n);
    for (UINTN i = 0; i < len; i++) {
        addch(str[i]);
    }

    return len;
}

void clear(void)
{
    // Clear all text
    for (UINTN y = 0; y < gWindow->height; y++) {
        for (UINTN x = 0; x < gWindow->width; x++) {
            UINTN index = y * gWindow->width + x;
            gWindow->content[index] = L' ';
            gWindow->attributes[index] = 0;
        }
    }

    gWindow->cursor_y = 0;
    gWindow->cursor_x = 0;
}

EFI_INPUT_KEY *getch(void)
{
    // Read a single character from the terminal
    UINTN index;
    EFI_INPUT_KEY *key = AllocateRuntimeZeroPool(sizeof(EFI_INPUT_KEY));

    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &index);
    gST->ConIn->ReadKeyStroke(gST->ConIn, key);

    return key;
}
