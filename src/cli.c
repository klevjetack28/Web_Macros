#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "ecp.h"
#include "macros.h"
#include "cli.h"

typedef enum 
{
    MAIN,
    CREATE,
    EDIT,
    SELECT_MACRO
    NAME,
    SELECT_SIGNAL
    SIGNAL,
    DELAY
} EditPhase;

static EditPhase phase = MAIN;

typedef enum {
    MAIN_MENU,
    USE_MACROS,
    CREATE_EDIT_MACROS,
    TESTER,
    SETTINGS
} CliState;

static CliState state = MAIN_MENU;
static CliState prev_state = MAIN_MENU;

static void cli_set_state(CliState s)
{
    prev_state = state;
    state = s;
}

static int cli_to_decimal(const char *buff)
{
    int v1 = buff[0] - '0';
    int v2 = buff[1] - '0';
    if (v1 > 9 || v1 < 0)
    {
        return -1;
    }
    if (buff[1] != '\0')
    {
        if (v2 > 9 || v2 < 0)
        {
            return -1;
        }
    }
    if (buff[1] == '\0')
    {
        return v1;
    } 
    else
    {
        int n = (v1 * 10) + v2;
        return (n < NUM_MACROS) ? n : -1;
    }
}

static void trim_str(char *s)
{
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\0' || isspace(s[n - 1])))
    {
        s[--n] = '\0';
    }
    
    size_t i = 0;
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

static void cli_input_create_edit_macros(void)
{
    
}

static void menu_delay(void)
{
    // Print the delays
}

static void menu_signal(void)
{
   // Print the signals 
}

static void menu_select_signal(void)
{
    puts("===== SELECT SIGNAL =====");
    for (int i = 0; i < MACRO_LENGTH; i++)
    {
        macro_print_signal(g_macro[index].signals[i]);
    }
    puts("Type 'b' to go back.");
}

static void menu_name(void)
{
    puts("===== SET NAME =====");
    if (g_macros[index].create)
    {
        printf("Current Name: %s", g_macros[index].name);
    } 
    else
    {
        printf("Default Name: %s%d", g_macros[index].name, g_num_macros); 
    }
    puts("Type a new name and press Enter.");
    puts("Press Enter with nothing to keep current (or use default on Create).");
    puts("Type 'b' to go back."); 
}

static void menu_create(void)
{
    // ??
}

static void menu_select_macro(void)
{
    puts("===== SELECT MACRO =====");
    for (int i = 0; i < g_num_macros; i++)
    {
        printf("%d) %s\n", i + 1, g_macros[i].name);
    }
    puts("Type 'b' to go back.");
}

static void menu_edit(void)
{
    // ???
}

static void menu_main(void)
{
    puts("===== CREATE/EDIT MACROS =====");
    puts("Create a new macro press 'c'.");
    puts("Edit an existing macro press 'e'.");
    puts("Type 'b' to go back.");
}

static void cli_create_macros(void)
{
    switch (phase)
    {
        case MAIN:
            menu_main();
            break;
        case EDIT:
            medu_edit();
            break;
        case SELECT_MACRO:
            menu_select_macro();
            break;
        case CREATE:
            menu_create();
            break;
        case NAME:
            menu_name();
            break;
        case SELECT_SIGNAL:
            menu_select_signal();
            break;
        case SIGNAL:
            menu_signal();
            break;
        case DELAY:
            menu_delay();
            break;
        default:
            puts("SOMETHING WENT WRONG IN CREATE/EDIT MACRO!!!! SEND HELP!!!");
    }
}

static void cli_input_use_macros(void)
{
    char buff[8];
    cli_get_input(buff, sizeof(buff), "> ");
    if (buff[0] == 'b')
    {
        cli_set_state(prev_state);
    }
    int index = cli_to_decimal(buff);
    if (index == -1)
    {
        puts("Invalid Macro");
        return;
    }
    macro_play(index - 1);
}

static void cli_use_macros(void)
{
    puts("===== USE MACROS =====");
    for (int i = 0; i < g_num_macros; i++)
    {
        printf("%d) %s\n", i + 1, g_macros[i].name);
    }
    puts("Type 'b' to go back.");
}

static void cli_input_menu()
{
    char c[8];
    cli_get_input(c, sizeof(c), "> ");
    switch (c[0])
    {
        case '1':
            cli_set_state(USE_MACROS);
            return;
        case '2':
            cli_set_state(CREATE_EDIT_MACROS);
            return;
        case '3':
            cli_set_state(TESTER);
            return;
        case '4':
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
    puts("2) Create/Edit Macros");
    puts("3) Key Tester (send single key)");
    puts("4) Settings (IP, file path, autosave)");
    puts("q) Quit");
}

static void cli_loop(void)
{
    switch (state)
    {
        case MAIN_MENU:
            cli_menu();
            cli_input_menu();
            break;
        case USE_MACROS:
            cli_use_macros();
            cli_input_use_macros();
            break;
        case CREATE_EDIT_MACROS:
            puts("CREATE MACROS");
            break;
        case TESTER:
            puts("TESTER");
            break;
        case SETTINGS:
            puts("SETTINGS");
            break;
        default:
            printf("ERROR DEFAULT SWITCH\n");
            return;
    }
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
