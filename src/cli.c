#include <stdio.h>

#include "ecp.h"
#include "macros.h"
#include "cli.h"

typedef enum {
    MAIN_MENU,
    USE_MACROS,
    DISPLAY_MACROS,
    SEND_KEY_BY_NAME
} UiState;

static void cli_menu(void)
{
    puts("===== Roku Macros =====");
    puts("1) Use macros");
    puts("2) Edit macros");
    puts("3) Edit keybinds for macros");
    puts("4) Display macros");
    fputs("> ", stdout);
}

void cli_input(void)
{
    char c;
    while (c = getchar())
    {
        /*
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
        }*/
    }
}

static void cli_setup(void)
{
    ecp_init("");
    macro_init();
}

void cli_run(void)
{
    cli_setup();

    while (1)
    {
        /*
        switch (context.state)
        {
            case MAIN_MENU:
                cli_menu();
                cli_input();
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
        }*/
    // TODO check_exit() somehow find when player wants to exit

        macro_play(0);
    }
}
