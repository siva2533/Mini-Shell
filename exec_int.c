#include "header.h"

extern int job, status;
node *head = NULL;
void execute_internal_commands(char *input)
{
    if (strcmp(input, "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(input, "pwd") == 0)
    {
        char buff[50];
        getcwd(buff, 50);
        printf("%s\n", buff);
    }
    else if (strstr(cmd, "cd") != NULL)
    {
        if (strcmp(input, "cd") == 0)
        {
            chdir(getenv("HOME"));
            char buff[100];
            getcwd(buff, 100);
            printf("%s\n", buff);
        }
        else
        {
            chdir(input + 3);
            char buff[100];
            getcwd(buff, 100);
            printf("%s\n", buff);
        }
    }

    else if (strcmp(cmd, "echo") == 0)
    {
        if (strcmp(input, "echo $$") == 0)
        {
            printf("%d\n", getpid());
        }
        else if (strcmp(input, "echo $?") == 0)
        {
            if (WIFEXITED(status))
            {
                printf("%d\n", WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status))
            {
                printf("%d\n", 128 + WTERMSIG(status));
            }
        }
        else if (strcmp(input, "echo $SHELL") == 0)
        {
            printf("%s\n", getenv("SHELL"));
        }
        else if (strlen(input) > 4)
        {
            printf("%s\n", input + 5);
        }
        else
        {
            printf("\n");
        }
    }
    else if (strcmp(input, "jobs") == 0)
    {
        print_list();
    }
    else if (strcmp(input, "fg") == 0)
    {
        if (head == NULL)
        {
            printf("fg: current: no such job\n");
            return;
        }
        printf("%s\n", head->command);
        kill(head->pid, SIGCONT);
        head->job_status = RUNNING;
        waitpid(head->pid, &status, WUNTRACED);
        if (WIFSTOPPED(status))
        {
           head->job_status = STOPPED;
			printf("\n[%d]+  Stopped\t\t\t%s\n", head->job_no, head->command);
        }
        else
        {
            delete_first();
            job--;
        }
    }
    else if (strcmp(input, "bg") == 0)
    {
        if (head == NULL)
        {
            printf("bg: current: no such job\n");
            return;
        }
        kill(head->pid, SIGCONT);
		head->job_status = RUNNING;
		printf("[%d]+ %s &\n", head->job_no, head->command);
        signal(SIGCHLD, signal_handler);
    }

    else if (strcmp(cmd, "printf") == 0)
    {
        char *tok = strtok(input + 7, " ");
        printf("%s", tok);
    }
}