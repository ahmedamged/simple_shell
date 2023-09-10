#include "main.h"
/**
 * get_command_args - command format
 * @line: string of command adn args
 * @args: args
 *
 * formats line into command and args
 *
 * Return: void
 */
void get_command_args(char *line, char ***args)
{
	char *temp = malloc(sizeof(char) * (strlen(line) + 1)), *string_array;
	size_t i, length = 0;

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
		string_array = strtok(line, " ");
		for (i = 0; string_array != NULL; i++)
		{
			(*args)[i] = malloc(sizeof(char) * (strlen(string_array) + 1));
			(*args)[i] = strcpy((*args)[i], string_array);
			string_array = strtok(NULL, " ");
		}
		(*args)[0] = locate_relative_cmd((*args)[0]);
		(*args)[length] = NULL;
	}
}
/**
 * locate_relative_cmd - parse commands
 * @command: command to find
 *
 * locates the absolute path of a command
 *
 * Return: String (Command)
 */
char *locate_relative_cmd(char *command)
{
	struct stat st;
	char *paths, *temp_path, *token, *temp_command;

	if (stat(command, &st) == 0)
	{
		return (command);
	}

	paths = getenv("PATH");
	temp_path = malloc(sizeof(char) * strlen(paths));
	temp_path = strcpy(temp_path, paths);
	token = strtok(temp_path, ":");

	while (token != NULL)
	{
		temp_command = malloc(sizeof(char) * (strlen(token) + 1));
		temp_command = strcpy(temp_command, token);
		if (temp_command[strlen(token) - 1] != '/')
			temp_command = strcat(temp_command, "/");
		temp_command = strcat(temp_command, command);
		temp_command[strlen(temp_command)] = '\0';
		if (stat(temp_command, &st) == 0)
		{
			free(temp_path);
			return (temp_command);
		}
		free(temp_command);
		token = strtok(NULL, ":");
	}
	free(temp_path);
	return (command);
}