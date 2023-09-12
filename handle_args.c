#include "main.h"

/**
 * get_command_args - command format
 * @line: string of command adn args
 * @args: args
 *
 * formats line into command and args
 *
 * Return: 0 (Success)
 * 404 (non found)
 */
int get_command_args(char *line, char ***args)
{
	char *temp = malloc(sizeof(char) * (strlen(line) + 1)), *string_array, **temp_array;
	size_t length = 0, i;

	temp = strcpy(temp, line);
	string_array = strtok(temp, " ");
	while (string_array != NULL)
	{
		length++;
		string_array = strtok(NULL, " ");
	}
	temp = strcpy(temp, line);
	*args = malloc(sizeof(char *) * (length + 1));
	for (i = 0; i <= length; i++)
	{
		(*args)[i] = NULL;
	}
	temp_array = *args;
	string_array = strtok(temp, " ");
	while (string_array != NULL)
	{
		length = strlen(string_array);
		*temp_array = malloc(sizeof(char) * (length + 1));
		*temp_array = strcpy(*temp_array, string_array);
		*temp_array = strcat(*temp_array, "\0");
		temp_array++;
		string_array = strtok(NULL, " ");
	}
	free(temp);
	temp = locate_relative_cmd(**args);
	if (temp == NULL)
	{
		return (404);
	}
	free(**args);
	**args = temp;
	*temp_array = NULL;
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
	path *path_temp = malloc(sizeof(path)), *temp;
	size_t path_len;
	char *temp_command;

	path_temp->next = NULL;
	temp = path_temp;
	if (stat(command, &st) == 0)
	{
		return (command);
	}
	free(path_temp);
	path_temp = _getenv("PATH");

	while (path_temp != NULL && path_temp->value != NULL)
	{
		path_len = strlen(path_temp->value);
		temp_command = malloc(sizeof(char) * (path_len + 2 + strlen(command) + 1));
		temp_command = strcpy(temp_command, path_temp->value);
		printf("path_len: %ld, temp_command: %s, command: %s\n", path_len, temp_command, command);
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
 * return: linked list of strings
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