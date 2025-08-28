#ifndef DELAYS_H
#define DELAYS_H

typedef enum
{
    DELAY_UNSET,
    DELAY_250,
    DELAY_500,
    DELAY_750,
    DELAY_1000,
    DELAY_1500,
    DELAY_2000,
    DELAY_2500,
    DELAY_3000,
    DELAY_5000,
    DELAY_7500,
    DELAY_10000,
    DELAY_COUNT
} Delay;

typedef struct
{
    const char *name;
    Delay       code;
    double      seconds;
} DelayEntry;

extern const DelayEntry g_delays[DELAY_COUNT];

static inline double delay_seconds(Delay d)
{
    return g_delays[d].seconds;
}

static inline const char* delay_name(Delay d)
{
    return g_delays[d].name;
}

#endif // DELAYS_H
