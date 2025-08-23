#include "macros.h"

void init_macros(void)
{
    // TODO read from init txt or json file to read macros
    
    // BASIC CHANGE LATER
    // just inits everything to zero except first macro which is to modern family
    // volume up by 10 can be later when i add keydown and keyup for holding buttons

    for (int i = 0; i < NUM_MACROS; i++)
    {
        for (int j = 0; j < MACRO_LENGTH; j++)
        {
        }
    }

    // TODO Macro struct with array of macro, int for length, char* for name ect macros also need a delay with some form of sleep
    set_macro(0, 0, KEY_HOME, DELAY_3000);
    set_macro(0, 1, KEY_RIGHT, DELAY_500);
    set_macro(0, 2, KEY_SELECT, DELAY_7500);
    set_macro(0, 3, KEY_SELECT, DELAY_3000);
    set_macro(0, 4, KEY_DOWN, DELAY_500);
    set_macro(0, 5, KEY_DOWN, DELAY_500);
    set_macro(0, 6, KEY_DOWN, DELAY_500);
    set_macro(0, 7, KEY_SELECT, DELAY_2000);
    set_macro(0, 8, KEY_SELECT, DELAY_500);
    macros[0].length = 9;
}

void set_macro(int macro_index, int signal_index, RokuKey key, Delay delay)
{
    macros[macro_index].signals[signal_index].key = key;
    macros[macro_index].signals[signal_index].delay = delay;
}

void play_macro(int macro_index)
{
    int length = macros[macro_index].length;
    printf("Macro Length: %d\n", length);
    for (int i = 0; i < length; i++)
    {
        RokuKey key = macros[macro_index].signals[i].key;
        send_keypress(keys[key].name);
        Delay delay = macros[macro_index].signals[i].delay;
        sleep(delays[delay].value);
    }
}
