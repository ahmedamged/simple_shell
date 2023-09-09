#include "main.h"
/**
 * get_command_args - command format
 * @line: string of command adn args
 * @command: command
 * @args: args
 *
 * formats line into command and args
 *
 * Return: void
 */
void get_command_args(char *line, char **args[])
{
	int length = 0;
	char *temp = malloc(sizeof(char) * (strlen(line) + 1)), *string_array, **temp_array;

	temp = strcpy(temp, line);
	string_array = strtok(temp, " ");

	while (string_array != NULL)
	{
		length++;
		string_array = strtok(NULL, " ");
	}

	free(temp);

	if (length != 0)
	{
		*args = malloc(sizeof(char *) * (length + 1));
		temp_array = *args;
		string_array = strtok(line, " ");
		printf("%s\n", string_array);
		while (string_array != NULL)
		{
			*temp_array = malloc(sizeof(char) * (strlen(string_array) + 1));
			*temp_array = strcat(string_array, "\0");
			(temp_array)++;
			string_array = strtok(NULL, " ");
		}
		*temp_array = NULL;
	}
}
