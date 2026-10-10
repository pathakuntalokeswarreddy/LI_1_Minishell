#include "minishell.h"

void execute_internal_commands(char *input_string)
{
    char new_path[1024], temp[1024];
    memset(temp,0,sizeof(temp));

    /* for path related commands */
    if ( strstr(input_string, "cd")==input_string )
    {
        
        //Minishell Directory ==> Home directory, command:cd
        if( *(input_string+2) == '\0' )
        { 
            getcwd(temp,sizeof(temp));
            memset(new_path,0,sizeof(new_path));
            strcpy(new_path,home_path);
        }
        
        //previous working directory, command: cd -
        else if( (input_string[3]=='-') &&  (input_string[4]=='\0') )
        {       
            getcwd(temp,sizeof(temp));
            memset(new_path,0,sizeof(new_path));
            strcpy(new_path,prev_path);
        }
        
        // if path is specified, command: cd <new_path_name>
        else
        {
            getcwd(temp,sizeof(temp));
            memset(new_path,0,sizeof(new_path));
            strcpy(new_path,input_string+3);
        }
        

        /* chdir() is a system call to change paths */
        if(chdir(new_path) == -1)
        {
            if(errno == EACCES)
                printf("Permission is denied\n");
            else if(errno == EFAULT)
                printf("Path points to outside of accessable address space\n");
            else if(errno == ENOENT)
                printf("Specified path does not exist\n");
        }
        else
        {
            strcpy(prev_path,temp);
        }
    }

    /* command: jobs => will print all queued processes, which are sent to background by signal ctrl+z */
    else if (!strcmp(input_string, "jobs"))
    {
        print_nodes();
    }

    /* command: bg => will print status of recent backgrond process */
    else if (!strcmp(input_string, "bg"))
    {
        background_process();
    }

    /* command: fg => will bring back recent backgrond process to foreground */
    else if (!strcmp(input_string, "fg"))
    {
        foreground_process();
    }
}

void background_process(void)
{
    if (head == NULL)
    {
        printf("bg: no stopped process\n");
        return;
    }
    
    pid_t pid = head->child_pid;
    printf("[%d] %s &\n", pid, head->process_name);
    
    kill(pid, SIGCONT);
        
    int status;
    waitpid(pid, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        head->status = DONE;
        printf("[%d]+ Done %s\n", pid, head->process_name);
        exit_status = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
        head->status = RUNNING;
        exit_status = 128 + WTERMSIG(status);
    }
    else if (WIFSTOPPED(status))
    {
        insert_first(process_name, pid);
        exit_status = 128 + WSTOPSIG(status);
    }
    child_pid = 0;
}

void foreground_process(void)
{
    if (head == NULL)
    {
        printf("fg: no stopped process\n");
        return;
    }

    pid_t pid = head->child_pid;
    strcpy(process_name, head->process_name);

    delete_first();

    child_pid = pid;
    kill(pid, SIGCONT);

    int status;

    waitpid(pid, &status, WUNTRACED);
    if (WIFEXITED(status))
    {
        exit_status = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
        exit_status = 128 + WTERMSIG(status);
    }
    else if (WIFSTOPPED(status))
    {
        insert_first(process_name, pid);
        exit_status = 128 + WSTOPSIG(status);
    }
    child_pid = 0;
}

