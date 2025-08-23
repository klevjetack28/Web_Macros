#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "curl/curl.h"

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
    KEY_INPUT_AV_1
} RokuKey;

// TODO KEY_LIT_X sending characters on keypresses

typedef struct 
{
    const char *name;
    RokuKey code;
} RokuKeyEntry;

RokuKeyEntry keys[] = {
    { "home", KEY_HOME },
    { "rev", KEY_REV },
    { "fwd", KEY_FWD },
    { "play", KEY_PLAY },
    { "select", KEY_SELECT },
    { "left", KEY_LEFT },
    { "right", KEY_RIGHT },
    { "down", KEY_DOWN },
    { "up", KEY_UP },
    { "back", KEY_BACK },
    { "instant_replay", KEY_INSTANT_REPLAY },
    { "info", KEY_INFO },
    { "backspace", KEY_BACKSPACE },
    { "search", KEY_SEARCH },
    { "enter", KEY_ENTER },
    { "find_remote", KEY_FIND_REMOTE },
    { "volume_down", KEY_VOLUME_DOWN },
    { "volume_mute", KEY_VOLUME_MUTE },
    { "volume_up", KEY_VOLUME_UP },
    { "power_off", KEY_POWER_OFF },
    { "channel_up", KEY_CHANNEL_UP },
    { "channel_down", KEY_CHANNEL_DOWN },
    { "input_tuner", KEY_INPUT_TUNER },
    { "input_hdmi_1", KEY_INPUT_HDMI_1 },
    { "input_hdmi_2", KEY_INPUT_HDMI_2 },
    { "input_hdmi_3", KEY_INPUT_HDMI_3 },
    { "input_hdmi_4", KEY_INPUT_HDMI_4 },
    { "input_av_1", KEY_INPUT_AV_1 }
};

typedef enum
{
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
    int code;
    double value;
} DelayEntry;

DelayEntry delays[] = {
    { "Delay_250ms", DELAY_250,   0.250 },
    { "Delay_500ms", DELAY_500,   0.500 },
    { "Delay_750ms", DELAY_750,   0.750 },
    { "Delay_1000ms", DELAY_1000, 1.000 },
    { "Delay_1500ms", DELAY_1500, 1.500 },
    { "Delay_2000ms", DELAY_2000, 2.000 },
    { "Delay_2500ms", DELAY_2500, 2.500 },
    { "Delay_3000ms", DELAY_3000, 3.000 },
    { "Delay_5000ms", DELAY_5000, 5.000 },
    { "Delay_7500ms", DELAY_7500, 7.500 },
    { "Delay_10000ms", DELAY_10000, 10.000 }
};

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

Macro macros[NUM_MACROS];

typedef enum
{
    MAIN_MENU,
    USE_MACROS,
    EDIT_MACROS,
    EDIT_KEYBINDS,
    DISPLAY_MACROS
} State;

typedef struct
{
    CURL *curl;
    CURLcode res;
    State state;
    bool running;
    char *ip;
} Context;

Context context;

void init_context()
{
    context.state = MAIN_MENU;
    context.running = true;
}

// Wrapper for sending keyboard letters with Lit_x or changing channels or volume or typical keypresses like home or left or right
void send_keypress(const char *keypress)
{
    char url[64];
    sprintf(url, "http://192.168.1.5:8060/keypress/%s", keypress);
    
    curl_easy_setopt(context.curl, CURLOPT_URL, url);
    curl_easy_setopt(context.curl, CURLOPT_POST, 1L);
    curl_easy_setopt(context.curl, CURLOPT_POSTFIELDS, "");
    
    context.res = curl_easy_perform(context.curl);
    if (context.res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(context.res));
    }
}

void set_macro(int macro_index, int signal_index, RokuKey key, Delay delay)
{
    macros[macro_index].signals[signal_index].key = key;
    macros[macro_index].signals[signal_index].delay = delay;
}

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

void display_menu()
{
    printf("1> Use macros\n");
    printf("2> Edit macros\n");
    printf("3> Edit keybinds for macros\n");
    printf("4> Display macros\n");
}

void get_menu_input()
{
    char c;
    while (c = getchar())
    {
        switch (c)
        {
            case '1':
                context.state = USE_MACROS;
                return;
            case '2':
                context.state = EDIT_MACROS;
                return;
            case '3':
                context.state = EDIT_KEYBINDS;
                return;
            case '4':
                context.state = DISPLAY_MACROS;
                return;
            case '\n':
                break;
            default:
                printf("Please enter a numeric value 1-4.\n");
                display_menu();
        }
    }
}

void init()
{
    // TODO Init Curl
    // TODO Connect to TV
    // TODO Get IP address

    init_context();

    // TODO Init Macros
}

int main(int argc, char argv[]) 
{
    curl_global_init( CURL_GLOBAL_DEFAULT );

    init();

    context.curl = curl_easy_init();
    if (context.curl)
    {
        /*
        while (context.running)
        {
            switch (context.state)
            {
                case MAIN_MENU:
                    display_menu();
                    get_menu_input();
                    break;
                case USE_MACROS:
                    printf("USE MACROS\n");
                    break;
                case EDIT_MACROS:
                    printf("EDIT MACROS\n");
                    break;
                case EDIT_KEYBINDS:
                    printf("EDIT KEYBINDS\n");
                    break;
                case DISPLAY_MACROS:
                    printf("DISPLAY MACROS\n");
                    break;
                default:
                    printf("ERROR DEFAULT SWITCH\n");
                    return 1;
            }
            // TODO check_exit() somehow find when player wants to exit
        }*/

        init_macros();
        play_macro(0);
    }
    
    return 0;
}
