#include "./uefi_ncurses.h"

typedef struct {
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *SimpleTextOut;
    int height;
    int width;
    int cursor_y;
    int cursor_x;
} WINDOW;

EFI_SIMPLE_TEXT_INPUT_PROTOCOL *gSimpleTextIn;
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

    // Allocate memory for the WINDOW structure
    WINDOW *win = AllocatePool(sizeof(WINDOW));
    if (win == NULL) {
        return NULL;
    }

    // Initialize the WINDOW structure
    win->SimpleTextOut = gSimpleTextOut;
    win->height = gSimpleTextOut->Mode->Mode;
    win->width = gSimpleTextOut->Mode->Attribute;
    win->cursor_y = 0;
    win->cursor_x = 0;

    return win;
}




EFI_STATUS endwin(void)
{
    FreePool(gWin);
    return EFI_SUCCESS;
}




#define INPUT_MODE_RAW      0
#define INPUT_MODE_COOKED   1

UINTN gOriginalInputMode;
UINTN gCurrentInputMode;

EFI_STATUS SetInputMode(UINTN Mode)
{
    return gSimpleTextIn->SetMode(gSimpleTextIn, Mode);
}

EFI_STATUS raw(void)
{
    EFI_STATUS Status;

    // Locate the Simple Text Input protocol
    Status = gBS->LocateProtocol(
        &gEfiSimpleTextInProtocolGuid,
        NULL,
        (VOID **)&gSimpleTextIn
    );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    gSimpleTextIn->Mode->GetMode(gSimpleTextIn, &gOriginalInputMode);

    // Set the input mode to raw
    Status = SetInputMode(INPUT_MODE_RAW);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Update the global input mode
    gCurrentInputMode = INPUT_MODE_RAW;

    return EFI_SUCCESS;
}

EFI_STATUS cbreak(void)
{
    EFI_STATUS Status;

    // Locate the Simple Text Input protocol
    Status = gBS->LocateProtocol(
        &gEfiSimpleTextInProtocolGuid,
        NULL,
        (VOID **)&gSimpleTextIn
    );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    gSimpleTextIn->Mode->GetMode(gSimpleTextIn, &gOriginalInputMode);

    Status = SetInputMode(INPUT_MODE_COOKED);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Update the global input mode
    gCurrentInputMode = INPUT_MODE_COOKED;

    return EFI_SUCCESS;
}

EFI_STATUS RestoreInputMode(void)
{
    return SetInputMode(gOriginalInputMode);
}




BOOLEAN gOriginalEchoState;
BOOLEAN gCurrentEchoState;

EFI_STATUS SetEchoState(BOOLEAN EnableEcho)
{
    return gSimpleTextIn->SetAttribute(gSimpleTextIn, EnableEcho);
}

EFI_STATUS echo(void)
{
    EFI_STATUS Status;

    Status = gBS->LocateProtocol(
        &gEfiSimpleTextInProtocolGuid,
        NULL,
        (VOID **)&gSimpleTextIn
    );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = gBS->LocateProtocol(
        &gEfiSimpleTextOutProtocolGuid,
        NULL,
        (VOID **)&gSimpleTextOut
    );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    gSimpleTextIn->Mode->GetAttribute(gSimpleTextIn, &gOriginalEchoState);

    Status = SetEchoState(TRUE);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    gCurrentEchoState = TRUE;

    return EFI_SUCCESS;
}

EFI_STATUS noecho(void)
{
    EFI_STATUS Status;

    Status = gBS->LocateProtocol(
        &gEfiSimpleTextInProtocolGuid,
        NULL,
        (VOID **)&gSimpleTextIn
    );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    gSimpleTextIn->Mode->GetAttribute(gSimpleTextIn, &gOriginalEchoState);

    Status = SetEchoState(FALSE);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    gCurrentEchoState = FALSE;

    return EFI_SUCCESS;
}

EFI_STATUS RestoreEchoState(void)
{
    return SetEchoState(gOriginalEchoState);
}


BOOLEAN gOriginalKeypadState;
BOOLEAN gCurrentKeypadState;

EFI_STATUS SetKeypadState(BOOLEAN EnableKeypad)
{
    return gSimpleTextIn->SetAttribute(gSimpleTextIn, EnableKeypad);
}

int keypad(WINDOW *win, BOOLEAN enable)
{
    if (win == NULL) {
        return ERR;
    }

    gSimpleTextIn->Mode->GetAttribute(gSimpleTextIn, &gOriginalKeypadState);

    SetKeypadState(enable);

    gCurrentKeypadState = enable;

    return OK;
}

EFI_STATUS RestoreKeypadState(void)
{
    return SetKeypadState(gOriginalKeypadState);
}