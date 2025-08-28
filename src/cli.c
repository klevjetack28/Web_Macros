#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "ecp.h"
#include "macros.h"
#include "cli.h"

typedef enum
{
    TEST_SIGNAL,
    TEST_DELAY,
    TESTING
} TestPhase;

static TestPhase test_phase = TEST_SIGNAL;

typedef enum 
{
    EDIT_MAIN,
    SELECT_MACRO,
    EDIT_NAME,
    SELECT_SIGNAL,
    EDIT_SIGNAL,
    EDIT_DELAY
} EditPhase;

static EditPhase g_edit_phase = EDIT_MAIN;

typedef enum
{
    CREATE,
    EDIT,
} Action;

static Action g_action = CREATE;

static int g_index = -1;
static int g_signal_index = -1;
static Macro temp_macro; // used if canceled mid edit copies original into it and if canceled copues original back

typedef enum 
{
    MAIN_MENU,
    USE_MACROS,
    CREATE_EDIT_MACROS,
    TESTER
} CliState;

static CliState state = MAIN_MENU;
static CliState prev_state = MAIN_MENU;

static void cli_set_state(CliState s)
{
    prev_state = state;
    state = s;
}

static int cli_to_decimal_2_digit(const char *buff)
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

static void menu_print_delays()
{
    for (int i = 1; i < DELAY_COUNT; i++)
    {
        printf("%d) %s\n", i + 1, g_delays[i].name);
    }
}

static void menu_print_keys()
{
    for (int i = 1; i < KEY_COUNT; i++)
    {
        printf("%d) %s\n", i + 1, g_keys[i].name);
    }
}

typedef struct
{
    int key_index;
    int delay_index;
} TesterContext;

static void menu_input_tester_options(TesterContext tester_context)
{
    char c[8];
    cli_get_input(c, sizeof(c), "> ");
    
    switch (c[0])
    {
        case 'p':
            macro_play_single(g_keys[tester_context.key_index].code, g_delays[tester_context.delay_index].code);
            break;
        case 'd':
            test_phase = TEST_DELAY;
            break;
        case 'r':
            test_phase = TEST_SIGNAL;
            break;
        case 'q':
            cli_set_state(MAIN_MENU);
            break;
        default:
            puts("Invalid Input");
            menu_input_tester_options(tester_context);
    }
}

static int menu_input_tester(void)
{
    char c[8];
    cli_get_input(c, sizeof(c), "> ");
    return cli_to_decimal_2_digit(c);
}

static void cli_tester(void)
{
    TesterContext tester_context;
    tester_context.key_index = -1;
    tester_context.delay_index = -1;

    switch (test_phase)
    {
        case TEST_SIGNAL:
            puts("===== KEY TESTER =====");
            puts("Used to see functionality of key and tune delays.");
            menu_print_keys();
            puts("Type the key index if the key you want to test.");
            tester_context.key_index = menu_input_tester();
            break;
        case TEST_DELAY:
            puts("===== KEY TESTER =====");
            puts("Used to see functionality of key and tune delays.");
            menu_print_delays();
            puts("Type the delay index if the key you want to test.");
            tester_context.delay_index = menu_input_tester();
            break;
        case TESTING:
            puts("===== KEY TESTER =====");
            printf("Testing %s with delay %s.", g_keys[tester_context.key_index].name, g_delays[tester_context.delay_index].name);
            puts("Type 'p' to play key.");
            puts("Type 'd' to edit delay.");
            puts("Type 'r' to reset and pick new key and delay.");
            puts("Type 'q' to quit.");
            // have the same type or inout context here or a similar one. 
            menu_input_tester_options(tester_context);
            break;
    }
}

// I want some sort of struct to hold the macro index, signal index, key and delay so i can pass one parameter and remove the globals. 
static void menu_input_delay(void)
{
    char c[8];
    cli_get_input(c, sizeof(c), "> ");
    
    int n = cli_to_decimal_2_digit(c);
    if (n < DELAY_COUNT && n >= 0)
    {
        macro_set_delay(g_index, g_signal_index, g_delays[n].code);
        g_edit_phase = SELECT_SIGNAL;
        g_num_macros++;
    }
    else
    {
        puts("Invalid delay.");
        menu_input_delay();
    }
}

static void menu_delay(void)
{
    puts("===== DELAYS =====");
    menu_print_delays();
}

static void menu_input_signal(void)
{
    char c[8];
    cli_get_input(c, sizeof(c), "> ");
    
    int n = cli_to_decimal_2_digit(c);
    if (n < KEY_COUNT && n >= 0)
    {
        printf("%d, %d\n", g_index, g_signal_index);
        macro_set_key(g_index, g_signal_index, g_keys[n].code);
        g_edit_phase = EDIT_DELAY;
    }
    else
    {
        puts("Invalid signal.");
        menu_input_signal();
    }
}

static void menu_signal(void)
{
    puts("===== SIGNALS =====");
    menu_print_keys();
}

static void menu_input_select_signal(void)
{
    char c[8];
    cli_get_input(c, sizeof(c), "> ");
    
    int n = cli_to_decimal_2_digit(c);
    
    if (c[0] == 'd')
    {
        cli_set_state(prev_state);
        return;
    }

    if (g_action == CREATE)
    {
        if (n < MACRO_LENGTH && n >= 0)
        {
            g_signal_index = n;
            g_edit_phase = EDIT_SIGNAL;
        }
        else
        {
            puts("Invalid signal index try again.");
            menu_input_select_signal();
        }
    }
    else if (g_action == EDIT)
    {
        if (n < g_macros[g_index].length && n >= 0)
        {
            g_signal_index = n;
            g_edit_phase = EDIT_SIGNAL;
        }
        else 
        {
            puts("Invalid signal index try again.");
            menu_input_select_signal();
        }
    }
}


static void menu_select_signal(void)
{
    puts("===== SELECT SIGNAL =====");
    for (int i = 0; i < MACRO_LENGTH; i++)
    {
        if (g_action == CREATE)
        {
            printf("%d) %s, %s\n", i, key_name(g_macros[g_num_macros].signals[i].key), delay_name(g_macros[g_num_macros].signals[i].delay));
        }
        else if (g_action == EDIT)
        {
            printf("%d) %s, %s\n", i, key_name(g_macros[g_index].signals[i].key), delay_name(g_macros[g_index].signals[i].delay));
        }
    }
    puts("Type 'd' when done edit/create(ing) macro.");
    puts("Type 'a' to add a signal.");
    puts("Type 'i' followed by a valid index to insert.");
}

static void menu_input_name(void)
{
    char str[64];
    cli_get_input(str, sizeof(str), "> ");
    
    if (str[0] != '\0')
    {
        strcpy(g_macros[g_index].name, str);
    }
    g_edit_phase = SELECT_SIGNAL;
}

static void menu_name(void)
{
    puts("===== SET NAME =====");
    if (g_macros[g_index].created)
    {
        printf("Current Name: %s\n", g_macros[g_index].name);
    } 
    else
    {
        printf("Default Name: MACRO%d\n", g_num_macros); 
    }
    puts("Type a new name and press Enter.");
    puts("Press Enter with nothing to keep current (or use default on Create)."); 
}

static void menu_input_select_macro(void)
{
    char c[8];
    cli_get_input(c, sizeof(c), "> ");
    
    int index = cli_to_decimal_2_digit(c);
    
    if (index <= g_num_macros && index > 0)
    {
        g_index = index;
        g_edit_phase = EDIT_NAME;
    }
    else
    {
        puts("Select a valid macro to edit.");
        menu_input_select_macro();
    }
}

static void menu_select_macro(void)
{
    puts("===== SELECT MACRO =====");
    for (int i = 0; i < g_num_macros; i++)
    {
        printf("%d) %s\n", i + 1, g_macros[i].name);
    }
}

static void menu_input_main(void)
{
    char c[8];
    cli_get_input(c, sizeof(c), "> ");
    
    switch (c[0])
    {
        case 'c':
            g_edit_phase = EDIT_NAME;
            g_action = CREATE;
            g_index = g_num_macros;
            break;
        case 'e':
            g_edit_phase = SELECT_MACRO;
            g_action = EDIT;
            break;
        case 'b':
            cli_set_state(prev_state);
            break;
        default:
            puts("Invalid Option Try Again");
            menu_input_main();
    }
}

static void menu_main(void)
{
    puts("===== CREATE/EDIT MACROS =====");
    puts("Create a new macro press 'c'.");
    puts("Edit an existing macro press 'e'.");
    puts("Type 'b' to go back.");
}

static void cli_create_edit_macros(void)
{
    switch (g_edit_phase)
    {
        case EDIT_MAIN:
            menu_main();
            menu_input_main();
            break;
        case SELECT_MACRO:
            menu_select_macro();
            menu_input_select_macro();
            break;
        case EDIT_NAME:
            menu_name();
            menu_input_name();
            break;
        case SELECT_SIGNAL:
            menu_select_signal();
            menu_input_select_signal();
            break;
        case EDIT_SIGNAL:
            menu_signal();
            menu_input_signal();
            break;
        case EDIT_DELAY:
            menu_delay();
            menu_input_delay();
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
    int index = cli_to_decimal_2_digit(buff);
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
    cli_input_use_macros();
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
    cli_input_menu();
}

static void cli_loop(void)
{
    switch (state)
    {
        case MAIN_MENU:
            cli_menu();
            break;
        case USE_MACROS:
            cli_use_macros();
            break;
        case CREATE_EDIT_MACROS:
            cli_create_edit_macros();
            break;
        case TESTER:
            cli_tester();
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
