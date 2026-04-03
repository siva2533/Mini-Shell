#include "header.h"
char cmd[30];
char *external_cmd[154];
char prompt[25] = "minishell$: ";
char input[100];

int main()
{
    system("clear");

    extract_external_commands(external_cmd);

    scan_input(prompt, input);
}