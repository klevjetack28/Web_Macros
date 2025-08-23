#ifndef KEYS_H
#define KEYS_H

#include <stddef.h>

typedef enum 
{
    KEY_HOME,
    KEY_REV,
    KEY_FWD,
    KEY_PLAY,
    KEY_SELECT,
    KEY_LEFT,   
    KEY_RIGHT,
    KEY_DOWN,
    KEY_UP,
    KEY_BACK,
    KEY_INSTANT_REPLAY,
    KEY_INFO,
    KEY_BACKSPACE,
    KEY_SEARCH,
    KEY_ENTER,
    KEY_FIND_REMOTE,
    KEY_VOLUME_DOWN,    
    KEY_VOLUME_MUTE,
    KEY_VOLUME_UP,
    KEY_POWER_OFF,
    KEY_CHANNEL_UP,
    KEY_CHANNEL_DOWN,
    KEY_INPUT_TUNER, 
    KEY_INPUT_HDMI_1, 
    KEY_INPUT_HDMI_2, 
    KEY_INPUT_HDMI_3, 
    KEY_INPUT_HDMI_4,
    KEY_INPUT_AV_1,
    KEY_COUNT
} RokuKey;

// TODO KEY_LIT_X sending characters on keypresses

typedef struct 
{
    const char *name;
    RokuKey code;
} RokuKeyEntry;

extern const RokuKeyEntry g_keys[KEY_COUNT];

static inline const char* key_name(RokuKey k)
{
    return g_keys[k].name;
}

#endif // KEYS_H
