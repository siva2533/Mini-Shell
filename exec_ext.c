#include "header.h"

void execute_external_commands(char *input)
{
	char *argv[50];
	int argc = 0, pid = 0, status;
	char *token = strtok(input, " ");
	while (token != NULL)
	{
		argv[argc++] = token;
		token = strtok(NULL, " ");
	}
	argv[argc] = NULL;
	int cmd[20], cmd_count = 0;
	cmd[cmd_count++] = 0;
	int pipe_present = 0;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "|") == 0)
		{
			argv[i] = NULL;
			cmd[cmd_count++] = i + 1;
			pipe_present = 1;
		}
	}
	if (pipe_present == 0)
	{

			execvp(argv[0], argv);
			printf("%s: command not found\n", argv[0]);
			exit(1);
	}
	int fd[2];
	int prev_fd = -1;
	for (int i = 0; i < cmd_count; i++)
	{
		if (i < cmd_count - 1)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				exit(1);
			}
		}
		int pid = fork();
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, 0);
				close(prev_fd);
			}
			if (i < cmd_count - 1)
			{
				close(fd[0]);
				dup2(fd[1], 1);
				close(fd[1]);
			}
			execvp(argv[cmd[i]], &argv[cmd[i]]);
			perror("Command not found");
			exit(1);
		}
		if (prev_fd != -1)
			close(prev_fd);

		if (i < cmd_count - 1)
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
	}
	for (int i = 0; i < cmd_count; i++)
	{
		wait(NULL);
	}
}