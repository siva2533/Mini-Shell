#include "header.h"

char *get_command(char *input)
{
    // static char cmd[20];
    int i = 0;
    while (input[i] != ' ' && input[i] != '\0' && i < 29)
    {
        cmd[i] = input[i];
        i++;
    }
    cmd[i] = '\0';
    return cmd;
}

int check_command_type(char *cmd)
{
    for (int i = 0; builtins[i] != NULL; i++)
    {
        if (strcmp(builtins[i], cmd) == 0)
            return BUILTIN;
    }

    for (int i = 0; external_cmd[i] != NULL; i++)
    {
        if (strcmp(external_cmd[i], cmd) == 0)
            return EXTERNAL;
    }

    return NO_COMMAND;
}

void extract_external_commands(char **ext_cmd)
{
    int fd = open("ext_cmd.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("Sour77777ce open failed\n");
        return;
    }

    for (int i = 0; i < 153; i++)
    {
        char ch, arr[30];
        int n = 0;
        while (read(fd, &ch, 1) > 0)
        {
            if (ch == '\n')
                break;
            arr[n++] = ch;
        }
        arr[n] = '\0';
        ext_cmd[i] = malloc((n + 1) * sizeof(char));
        strcpy(ext_cmd[i], arr);
    }
}
