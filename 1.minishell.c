#include "minishell.h"

char prompt[30] = "minishell$ ";
char *external_command[153];

Slist *head = NULL;
pid_t child_pid = 0;
char process_name[30];

int exit_status = 0;

char home_path[1024];
char prev_path[1024];


int main()
{
    system("clear");

    char input_string[100];
    extract_external_commands(external_command);
    
    if ( !getcwd(home_path, sizeof(home_path)) )
    {
        perror("getcwd");
    }
    strcpy(prev_path,home_path);

    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);

    scan_input(prompt, input_string);

    return 0;
}

/* Base Function for the project */
void scan_input(char *prompt, char *input_string)
{
    while (1)
    {
        child_pid = 0;
        printf("\033[32m%s\033[0m", prompt);   //prints minishell prompt in green color


        __fpurge(stdin);   //flushing any leftover data from input stream

        //Reading Input from User      
        if (fgets(input_string, 100, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        //Replacing last character:'\n' with '\0'
        input_string[strcspn(input_string, "\n")] = '\0';
        
        if (strlen(input_string) == 0)
            continue;

        /* PS1=<minishell_new_name>
           If above format is satisfied, then change minishell name as requested */
        if (strstr(input_string, "PS1=") == input_string)
        {
            if ( (!strrchr(input_string, ' ')) && (!strrchr(input_string, '\t')) && 
                 (*(input_string+4) != '\0') && ( strchr(input_string, '$') && *(strchr(input_string, '$')+1) =='\0') ) 
            {
                strcpy(prompt, input_string + 4);
                strcat(prompt," ");
            }
            else
            {
                printf("%s : Invalid Command\nUSAGE: PS1=<name>$\n", input_string);
            }
            continue;
        }

        /*  -> exit  : to exit out of the function
            -> clear : clears previous work and makes terminal as new
            -> echo is both internel and externel command, so we can make them to work 
               according to our requirement, without checking command type 
            -> If pipe is involved in input string, no need of command type checking
               directly implement n-pipes concepts   
        */

        if (!strcmp(input_string, "exit"))
        {
            return;
        }
        else if (!strcmp(input_string, "clear"))
        {
            system("clear");
            continue;
        }
        else if (strcmp(input_string, "echo $$") == 0)
        {
            printf("%d\n", getpid());
            continue;
        }    
        else if (strcmp(input_string, "echo $?") == 0)
        {
            printf("%d\n", exit_status);
            continue;
        }
        else if (strcmp(input_string, "echo $SHELL") == 0)
        {
            char *shell = getenv("SHELL");

            if (shell != NULL)
                printf("%s\n", shell);
            
            continue;
        }
        else if(strchr(input_string, '|') != NULL)
        {
            execute_pipeline(input_string);
            continue;
        }

        //extracts first word from input command
        char *command = get_command(input_string);
        if (command == NULL)
        {
            printf("Memory Allocation Failed\n");
            continue;
        }
        
        //based on extracted word, command type is determined
        int type = check_command_type(command);

        if (type == BUILTIN)
        {
            execute_internal_commands(input_string);
        }
        else if (type == EXTERNAL)
        {
            execute_external_commands(input_string);
        }
        else
        {
            printf("%s : is Not Found\n", command);
        }

        free(command);
        memset(input_string,0,strlen(input_string));
    }
}

/* Signal Handler Function */
void signal_handler(int signum)
{
    if (child_pid > 0)           // for child processes, Default action should be taken
    {                            // Ex: sleep 10  , then ctrl+c   ==> process should be terminated
        if (signum == SIGINT)    //                      ctrl+z   ==> process should be stopped
        {
            kill(child_pid, SIGINT);
            delete_first();
        }
        else if (signum == SIGTSTP)
        {
            kill(child_pid, SIGTSTP);
        }   
        write(1, "\n", 1);  
    }
    else                               // for parent process
    {                                  // Just prompt in newline should be printed for ctrl+c and ctrl+z
        write(1, "\n", 1);
        write(1, "\033[32m", 5);
        write(1, prompt, strlen(prompt));
        write(1, "\033[0m", 4);
    }
}
