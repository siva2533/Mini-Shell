#include "header.h"
int status;
int job = 0, pid;
void scan_input(char *prompt, char *input)
{
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);

    while (1)
    {
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, prompt);
        scanf("%[^\n]", input);
        getchar();

        int l = strlen(input);
        while (l > 0 && input[l - 1] == ' ')
        {
            input[l - 1] = '\0';
            l--;
        }

        if (strncmp(input, "PS1=", 4) == 0 && strlen(input) > 4)
        {
            if (strchr(input, ' ') == NULL)
            {
                strcpy(prompt, input + 4);
            }
        }

        char *command = get_command(input);
        int type = check_command_type(command);

        if (type == BUILTIN)
        {
            execute_internal_commands(input);
        }
        else if (type == EXTERNAL)
        {
            int st;
            pid = fork();
            if (pid > 0)
            {
                waitpid(pid, &st, WUNTRACED);
                 status = st;
            }
            else if (pid == 0)
            {
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                execute_external_commands(input);
                exit(0);
            }
        }
        else
        {
            if (cmd[0] == '\0')
                continue;

            printf("%s: command not found\n", cmd);
        }

        input[0] = '\0';
    }
}

void signal_handler(int sig_num)
{
    if (sig_num == SIGINT)
    {
        if (pid == 0)
        {
            printf("\n");
            printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, prompt);
            fflush(stdout);
            __fpurge(stdin);
        }
    }
    else if (sig_num == SIGTSTP)
    {
        if (pid == 0)
        {
            printf("\n");
            printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, prompt);
            fflush(stdout);
            __fpurge(stdin);
        }
        else
        {
            insert_at_first(pid, input, job + 1);
            job++;
            printf("\n[%d]+  Stopped\t\t\t%s\n", job, input);
            pid = 0;
        }
    }
    else if (sig_num == SIGCHLD)
    {
        int st;
        int done_pid;
        while ((done_pid = waitpid(-1, &st, WNOHANG)) > 0)
        {
            delete_by_pid(done_pid);
            job--;
        }
    }
}
