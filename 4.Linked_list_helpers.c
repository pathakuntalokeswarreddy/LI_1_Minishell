#include "minishell.h"

void insert_first(char *name, pid_t pid)
{
    Slist *new = malloc(sizeof(Slist));
    if (new == NULL)
    {
        printf("Memory Allocation Failed\n");
        return;
    }

    strcpy(new->process_name, name);
    new->child_pid = pid;
    new->status = STOPPED;
    
    new->link = head;
    head = new;
}

void delete_first(void)
{
    if (head == NULL)
        return;

    Slist *temp = head;
    head = head->link;
    free(temp);
}

void print_nodes(void)
{
    Slist *temp = head;

    while(temp)
    {
        if (temp->status == STOPPED)
        {
            printf("[%d] Stopped %s\n", temp->child_pid, temp->process_name);
        }
        else if (temp->status == RUNNING)
        {
            printf("[%d] Running %s\n", temp->child_pid, temp->process_name);
        }
        if (temp->status == DONE)
        {
            printf("[%d] Done %s\n", temp->child_pid, temp->process_name);
            delete_first();
        }
        temp = temp->link;
    }
}
