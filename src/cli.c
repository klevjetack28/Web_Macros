#include <stdio.h>

#include "ecp.h"
#include "macros.h"
#include "cli.h"

typedef enum {
    MAIN_MENU,
    USE_MACROS,
    CREATE_MACROS,
    EDIT_MACROS,
    TESTER,
    SETTINGS
} UiState;

static UiState state = MAIN_MENU;
static UiState prev_state = MAIN_MENU;

static void cli_set_state(UiState s)
{
    prev_state = state;
    state = s;
}

static void trim_str(char *s)
{
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\0' || isspace(s[n - 1])))
    {
        s[--n] = '\0';
    }
    
    size_t = 0;
    while (s[i] && isspace(s[i]))
    {
        i++;
    }
    
    if (i)
    {
        memmove(s, s + 1, strlen(s + i) + 1);
    }
}

static void cli_get_input(char* buff, size_t bufflen, const char *prompt)
{
    fputs(prompt, stdout);
    fflush(stdout);
    
    if (!fgets(buff, bufflen, stdin))
    {
        return;
    }
    
    trim_str(buff);
}

static void cli_input_settings(void)
{
    
}

static void cli_settings(void)
{
    
}

static void cli_input_tester(void)
{
    
}

static void cli_tester(void)
{
    
}

static void cli_input_edit_macros(void)
{
    
}

static void cli_edit_macros(void)
{
    
}

static void cli_input_create_macros(void)
{
    
}

static void cli_create_macros(void)
{
    
}

static void cli_input_use_macros(void)
{
    
}

static void cli_use_macros(void)
{
    puts("===== USE MACROS =====");
    for (int i = 0; i < NUM_MACROS; i++)
    {
        printf("%d) %s", i + 1, g_macros[i].name);
    }
}

static void cli_input_menu(char c)
{
    switch (c)
    {
        case '1':
            cli_set_state(USE_MACROS);
            return;
        case '2':
            cli_set_state(CREATE_MACROS);
            return;
        case '3':
            cli_set_state(EDIT_MACROS);
            return;
        case '4':
            cli_set_state(TESTER);
            return;
        case '5':
            cli_set_state(SETTINGS);
            return;
        case '\n':
            break;
        default:
            printf("Please enter a numeric value 1-4.\n");
    }
}

static void cli_menu(void)
{
    puts("===== Roku Macros =====");
    puts("1) Use Macros");
    puts("2) Create Macros");
    puts("3) Edit Macros");
    puts("4) Key Tester (send single key)");
    puts("5) Settings (IP, file path, autosave)");
    puts("6) Quit");
}

static void cli_loop(void)
{
    switch (state)
    {
        case MAIN_MENU:
            cli_menu();
            cli_input_menu(getchar());
            break;
        case USE_MACROS:
            puts("USE MACROS");
            break;
        case CREATE_MACROS:
            puts("CREATE MACROS");
            break;
        case EDIT_MACROS:
            puts("EDIT MACROS");
            break;
        case TESTER:
            puts("TESTER");
            break;
        case SETTINGS:
            puts("SETTINGS");
            break;
        default:
            printf("ERROR DEFAULT SWITCH\n");
            return 1;
    }
    // TODO check_exit() somehow find when player wants to exit

    macro_play(0);
}

void cli_init(void)
{
    ecp_init("");
    macro_init();
}

void cli_run(void)
{
    while (1)
    {
        cli_loop();
    }
}
