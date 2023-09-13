#include "main.h"
/**
 * handle_custom_command - command format
 * @line: string to find argument
 * @args: arguments
 *
 * handles custom argument
 *
 * Return: true if found,
 * false if general
 */
bool handle_custom_command(char *line, char ***args)
{
	if (strncmp(line, "exit", 4) == 0)
	{
		*args = malloc(sizeof(char *) * 2);
		(*args)[0] = "exit";
		(*args)[1] = NULL;
		return (true);
	}
	else if (strncmp(line, "env", 3) == 0)
	{
		*args = malloc(sizeof(char *) * 2);
		(*args)[0] = "env";
		(*args)[1] = NULL;
		return (true);
	}
	return (false);
}
/**
 * execute_custom_command - command format
 * @line: string to find argument
 * @args: arguments
 *
 * handles custom argument
 *
 * Return: true if found,
 * false if general
 */
void execute_custom_command(char ***cmd)
{
	if (strcmp((*cmd)[0], "exit") == 0)
	{
		free(*cmd);
		exit(0);
	}
	else if (strcmp((*cmd)[0], "env") == 0)
	{
		free(*cmd);
		print_env();
	}
}
