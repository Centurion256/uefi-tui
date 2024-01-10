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
#include <Protocol/SimpleTextInEx.h>
#include <Library/PrintLib.h>


// Define a WINDOW structure for representing a window
typedef struct WINDOW {
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *SimpleTextOut;
    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEx;
    UINTN height;
    UINTN width;
    UINTN cursor_y;
    UINTN cursor_x;
    CHAR16 *content;
    UINTN *attributes; 
} WINDOW;

// Initializers
WINDOW* initscr(void);  // Start curses mode
UINTN raw(void);          // Enter raw mode
UINTN cbreak(void);       // Enter cbreak mode
UINTN echo(void);         // Enable echoing of characters typed by the user
UINTN noecho(void);       // Disable echoing
UINTN keypad(WINDOW *win, BOOLEAN bf);  // Enable reading of function keys
UINTN endwin(void);       // End ncurses mode

// Output functions
UINTN addch(CHAR16 ch);                // Put a single character into the current cursor location
UINTN mvaddch(UINTN y, UINTN x, CHAR16 ch); // Move cursor to a given poUINTN and then print
UINTN mvprintw(UINTN y, UINTN x, const CHAR16 *format, ...); // Move cursor to a position and then print
UINTN printw(const CHAR16 *format, ...); // Simple print
UINTN addstr(const CHAR16 *str);        // Put a character string into a window
UINTN addnstr(const CHAR16 *str, UINTN n); // Put at most n characters into the screen
void clear(void);                   // Clear all text
void refresh(void);                 // Actually prUINTN the window

// Input functions
EFI_INPUT_KEY *getch(void);                    // Read a single character from the terminal
UINTN fgetc(IN EFI_FILE_PROTOCOL *File);            // Get character from file.
UINTN scanw(const CHAR16 *format, ...); // Similar to scanf() with the added capability of getting input from any location on the screen
UINTN mvscanw(UINTN y, UINTN x, const CHAR16 *format, ...); // Similar to scanw() with cursor movement
UINTN getstr(CHAR16 *str);               // Get strings from the terminal

// Attributes
void attron(UINTN attributes);        // Turn on printing with some effect
void attroff(UINTN attributes);       // Turn off printing with some effect
void attrset(UINTN attributes);       // Set the attributes of the window
void attr_get(UINTN *attributes, short *color_pair); // Get current attributes and color pair
UINTN chgat(UINTN num_chars, UINTN attributes, short color_pair, const void *opts); // Change attributes for characters on the screen

// Colors
BOOLEAN has_colors(void);               // Check if the terminal supports color
void start_color(void);              // Initialize color support
UINTN init_color(short color, UINTN r, UINTN g, UINTN b); // Define a new color
UINTN init_pair(short pair, short fcolor, short bcolor); // Define foreground and background for a color pair

#endif //UEFI_TUI_NCURSES_H
