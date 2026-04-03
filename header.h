#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define BUILTIN 1
#define EXTERNAL 2
#define NO_COMMAND 0
#define STOPPED	0
#define RUNNING	1

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct node
{
    int pid;
    int job_no;
    int job_status;
    char command[100];
    struct node *next;
} node;

extern node *head;

extern char cmd[30];
extern char *external_cmd[154];
extern char *builtins[];
extern char prompt[25];
extern char input[100];

void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);
void copy_change(char *prompt, char *input_string);
int check_command_type(char *command);
void execute_internal_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);
void execute_external_commands(char *input);
void print_list();
void delete_first();
void delete_by_pid(int pid);
void insert_at_first(int pid, char *cmd, int job_no);
void update_job_status(int pid, int new_status);
#endif