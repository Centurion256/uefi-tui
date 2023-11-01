#include "efi.h"

efi_status_t efi_main(
        efi_handle_t handle, struct efi_system_table *system_table)
{
    uint16_t msg[] = u"Hello World!";
    struct efi_input_key* key = {0};
    efi_status_t status;
    efi_status_t result;

    status = system_table->out->clear_screen(system_table->out);
    if (status != 0)
        return status;

    status = system_table->out->output_string(system_table->out, msg);
    if (status != 0)
        return status;

    status = system_table->in->reset(system_table->in, 0);
    if (status != 0)
        return status;
    // wait for a key press
    result = system_table->in->read_key_stroke(system_table->in, key);
    while (result != 0){
        result = system_table->in->read_key_stroke(system_table->in, key);
    }

    status = system_table->in->reset(system_table->in, 0);
    if (status != 0)
        return status;

    status = 0;
    return status;
}