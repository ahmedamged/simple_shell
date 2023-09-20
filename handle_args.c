#include "main.h"

/**
 * get_command_args - command format
 * @line: string of command and args
 * @args: args
 *
 * formats line into command and args
 *
 * Return: 0 (Success)
 * 404 (non found)
 */
int get_command_args(char *line, char ***args)
{
	char *temp = malloc(sizeof(char) * (strlen(line) + 1)), *string_array,
		 **temp_array;
	size_t length = 0, i;

	temp = strcpy(temp, line);
	if (handle_custom_command(line, args))
	{
		free(temp);
		return (0);
	}
	string_array = strtok(temp, " ");
	while (string_array != NULL)
	{
		length++;
		string_array = strtok(NULL, " ");
	}
	temp = strcpy(temp, line);
	*args = malloc(sizeof(char *) * (length + 1));
	for (i = 0; i <= length; i++)
		(*args)[i] = NULL;
	temp_array = *args;
	string_array = strtok(temp, " ");
	for (i = 0; string_array != NULL; i++)
	{
		length = strlen(string_array);
		temp_array[i] = malloc(sizeof(char) * (length + 1));
		temp_array[i] = strcpy(temp_array[i], string_array);
		temp_array[i] = strcat(temp_array[i], "\0");
		string_array = strtok(NULL, " ");
	}
	temp_array[i] = NULL;
	free(temp);
	temp = locate_relative_cmd(**args);
	if (temp == NULL)
		return (404);
	if (temp == *args[0])
		return (0);
	free(*args[0]);
	*args[0] = temp;
	return (0);
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
	path *path_temp, *temp;
	size_t path_len;
	char *temp_command;

	if (stat(command, &st) == 0)
		return (command);
	path_temp = _getenv("PATH");
	temp = path_temp;
	while (path_temp != NULL && path_temp->value != NULL)
	{
		path_len = strlen(path_temp->value);
		temp_command = malloc(sizeof(char) * (path_len + 2 + strlen(command) + 1));
		temp_command = strcpy(temp_command, path_temp->value);
		if (temp_command[path_len - 1] != '/')
			temp_command = strcat(temp_command, "/");
		temp_command = strcat(temp_command, command);
		temp_command = strcat(temp_command, "\0");
		if (stat(temp_command, &st) == 0)
		{
			while (path_temp != NULL)
			{
				free(path_temp->value);
				temp = path_temp;
				path_temp = path_temp->next;
				free(temp);
			}
			return (temp_command);
		}
		free(temp_command);
		free(path_temp->value);
		temp = path_temp;
		path_temp = path_temp->next;
		free(temp);
	}
	free(path_temp->next);
	free(path_temp->value);
	free(path_temp);
	return (NULL);
}
/**
 * _getenv - environ
 * @var: name of variable
 *
 * gets array of values
 *
 * Return: linked list of strings
 */
path *_getenv(char *var)
{
	char *token, **envs = __environ, *env;
	path *start = malloc(sizeof(path)), *current;
	size_t var_len = strlen(var), str_len;

	start->next = NULL;
	start->value = NULL;
	current = start;
	while (*envs != NULL)
	{
		if (strncmp(var, *envs, var_len) == 0)
		{
			env = malloc(sizeof(char) * (strlen(*envs) + 1));
			env = strcpy(env, *envs);
			token = strtok(env + var_len + 1, ":");
			while (token != NULL)
			{
				str_len = strlen(token);
				current->value = malloc(sizeof(char) * (str_len + 1));
				strcpy(current->value, token);
				current->value[str_len] = '\0';
				current->next = malloc(sizeof(path));
				current->next->next = NULL;
				current->next->value = NULL;
				current = current->next;
				token = strtok(NULL, ":");
			}
			free(env);
			return (start);
		}
		envs++;
	}
	return (NULL);
}
