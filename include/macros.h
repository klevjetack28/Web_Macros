#ifndef MACROS_H
#define MACROS_H

#include <stdbool.h>

#include "keys.h"
#include "delays.h"

#define NUM_MACROS 16
#define MACRO_LENGTH 32

typedef struct
{
    RokuKey key;
    Delay delay;
} Signal;

typedef struct
{
    Signal signals[MACRO_LENGTH];
    int length;
    bool created;
    const char *name;
} Macro;

extern Macro g_macros[NUM_MACROS];
extern int g_num_macros;

void macro_init(void);
void macro_set(int macro_index, int signal_index, RokuKey key, Delay delay);
void macro_set_delay(int macro_index, int_signal_index, Delay d);
void macro_set_key(int macro_index, int signal_index, RokuKey k);
void macro_play(int macro_index);

#endif // MACROS_H
