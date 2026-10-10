#include "minishell.h"

/* external commands are extracted into an 2D array from .txt file */
void extract_external_commands(char *ext_commd[])
{
	int fd = open("3.external_commands.txt",O_RDONLY);
	if(fd == -1)
	{
		perror("open");
		return;
	} 
	
	char buff[30], ch;
	int i=0,j=0;
	
	while(read(fd,&ch,1))
	{
		if(ch != '\n')
		{
			buff[i++] = ch;
		}
		else
		{
			buff[i] = '\0';

			ext_commd[j] = malloc(i+1);
			if(ext_commd[j] == NULL)
			{
				printf("memory allocation failed\n");
				close(fd);
				return;
			}
			strcpy(ext_commd[j],buff);
			j++;
			i = 0;
			memset(buff,0,sizeof(buff));
		}
	}
	ext_commd[j] = NULL;
	close(fd);
}

/* extracts first word(command) from input string. */
/* The extracted word is used later to identify the type of command among internal or external commands*/
char *get_command(char *input_string)
{
    char *buff = malloc(30);
    if (buff == NULL)
    {
        printf("Memory Allocation Failed\n");
        return NULL;
    }

    int i = 0;
    int j = 0;

	// skips first space characters before input_string, if present
	while ((input_string[i] == ' ') || (input_string[i] == '\t'))
		i++;

	// extracts characters from input_string, upto (space character or Null character or pipe) detected
    while ((input_string[i] != '\0') && (input_string[i] != ' ') && 
	       (input_string[i] != '\t') && (input_string[i] != '|') )
    {
        buff[j++] = input_string[i++];
    }
    buff[j] = '\0';

    return buff;
}

/* Checks command type among internal or external commands */
/* If matched with any, respective macro is returned */
/* BUILTIN  : if it is matched with internel command */
/* EXTERNAL : if it is matched with externel command */
/* NO_COMMAND : If neither is matched */
int check_command_type(char *command)
{
	char *builtin_commands[] = {"printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval", "set", 
		                "unset", "export", "declare", "typeset", "readonly", "getopts", "source", "exit",
						"exec", "shopt", "caller", "true", "type", "hash", "bind", "help", "fg", "jobs",
						"bg", NULL};
	int i = 0;

	while (external_command[i])
	{
		if (!strcmp(external_command[i], command))
			return EXTERNAL;

		i++;
	}

	i = 0;

	while (builtin_commands[i])
	{
		if (!strcmp(builtin_commands[i], command))
			return BUILTIN;

		i++;
	}
	return NO_COMMAND;
}