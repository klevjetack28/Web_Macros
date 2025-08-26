#include <stdio.h>
#include <unistd.h>

#include "macros.h"
#include "ecp.h"

Macro g_macros[NUM_MACROS];
int g_num_macros = 0;

void macro_init(void)
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
    macro_set(0, 0, KEY_HOME, DELAY_3000);
    macro_set(0, 1, KEY_RIGHT, DELAY_500);
    macro_set(0, 2, KEY_SELECT, DELAY_7500);
    macro_set(0, 3, KEY_SELECT, DELAY_3000);
    macro_set(0, 4, KEY_DOWN, DELAY_500);
    macro_set(0, 5, KEY_DOWN, DELAY_500);
    macro_set(0, 6, KEY_DOWN, DELAY_500);
    macro_set(0, 7, KEY_SELECT, DELAY_2000);
    macro_set(0, 8, KEY_SELECT, DELAY_500);
    g_macros[0].length = 9;
    g_macros[0].name = "MACRO 1";
    g_num_macros++;
    printf("Macro Length Init: %d\n", g_macros[0].length);
}

void macro_set(int macro_index, int signal_index, RokuKey key, Delay delay)
{
    g_macros[macro_index].signals[signal_index].key = key;
    g_macros[macro_index].signals[signal_index].delay = delay;
}

void macro_play(int macro_index)
{
    int length = g_macros[macro_index].length;
    for (int i = 0; i < length; i++)
    {
        RokuKey key = g_macros[macro_index].signals[i].key;
        ecp_keypress(key_name(key));
        Delay delay = g_macros[macro_index].signals[i].delay;
        sleep(delay_seconds(delay));
        printf("Playing Macro %d, Current Keypress %s, With Delay %f\n", macro_index, key_name(key), delay_seconds(delay));
    }
}
