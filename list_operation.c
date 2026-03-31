#include "header.h"

void insert_at_first(int pid, char *cmd, int job_no)
{
    node *new = malloc(sizeof(node));
    if (new == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    new->pid = pid;
    new->job_no = job_no;
    new->job_status = STOPPED;
    strcpy(new->command, cmd);
    new->next = head;
    head = new;
}

void delete_first()
{
    if (head == NULL)
    {
        return;
    }
    node *temp = head;
    head = head->next;
    free(temp);
}
void delete_by_pid(int pid)
{
    if (head == NULL)
        return;

    /* If head node itself holds the pid */
    if (head->pid == pid)
    {
        node *temp = head;
        head = head->next;
        free(temp);
        return;
    }

    node *prev = head;
    node *curr = head->next;
    while (curr != NULL)
    {
        if (curr->pid == pid)
        {
            prev->next = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void update_job_status(int pid, int new_status)
{
    node *temp = head;
    while (temp != NULL)
    {
        if (temp->pid == pid)
        {
            temp->job_status = new_status;
            return;
        }
        temp = temp->next;
    }
}


void print_list()
{
    node *temp = head;
    if (temp == NULL)
    {
        printf("No jobs\n");
        return;
    }
    while (temp != NULL)
    {
         if (temp->job_status == STOPPED)
            printf("[%d]+  Stopped\t\t\t%s\n", temp->job_no, temp->command);
        else
            printf("[%d]-  Running\t\t\t%s &\n", temp->job_no, temp->command);
        temp = temp->next;
    }
}
