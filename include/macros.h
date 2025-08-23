#ifndef MACROS_H
#define MACROS_H

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
    const char *name;
} Macro;

extern Macro g_macros[NUM_MACROS];

void init_macros(void);
void set_macro(int macro_index, int signal_index, RokuKey key, Delay delay);
void play_macro(int macro_index);

#endif // MACROS_H
