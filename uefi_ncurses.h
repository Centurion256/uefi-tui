#ifndef MY_CURSES_H
#define MY_CURSES_H

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/BaseLib.h>
#include <stdbool.h>

// Define a WINDOW structure for representing a window
typedef struct WINDOW {
    int height;         
    int width;            
    int cursor_y;         
    int cursor_x;  
} WINDOW;

// Initializers
WINDOW* initscr(void);  // Start curses mode
int raw(void);          // Enter raw mode
int cbreak(void);       // Enter cbreak mode
int echo(void);         // Enable echoing of characters typed by the user
int noecho(void);       // Disable echoing
int keypad(WINDOW *win, bool bf);  // Enable reading of function keys
int endwin(void);       // End ncurses mode

// Output functions
int addch(char ch);                // Put a single character into the current cursor location
int mvaddch(int y, int x, char ch); // Move cursor to a given point and then print
int mvprintw(int y, int x, const char *format, ...); // Move cursor to a position and then print
int printw(const char *format, ...); // Simple print
int addstr(const char *str);        // Put a character string into a window
int addnstr(const char *str, int n); // Put at most n characters into the screen
void clear(void);                   // Clear all text
void refresh(void);                 // Actually print the window

// Input functions
int getch(void);                    // Read a single character from the terminal
int fgetc(IN EFI_FILE_PROTOCOL *File);            // Get character from file.
int scanw(const char *format, ...); // Similar to scanf() with the added capability of getting input from any location on the screen
int mvscanw(int y, int x, const char *format, ...); // Similar to scanw() with cursor movement
int getstr(char *str);               // Get strings from the terminal

// Attributes
void attron(int attributes);        // Turn on printing with some effect
void attroff(int attributes);       // Turn off printing with some effect
void attrset(int attributes);       // Set the attributes of the window
void attr_get(int *attributes, short *color_pair); // Get current attributes and color pair
int chgat(int num_chars, int attributes, short color_pair, const void *opts); // Change attributes for characters on the screen

// Colors
bool has_colors(void);               // Check if the terminal supports color
void start_color(void);              // Initialize color support
int init_color(short color, int r, int g, int b); // Define a new color
int init_pair(short pair, short fcolor, short bcolor); // Define foreground and background for a color pair

#endif /* MY_CURSES_H */
