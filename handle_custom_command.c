#include "main.h"
#define MAX_READ_BUFFER_SIZE 1024
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
	char *temp = NULL;

	if (strncmp(line, "exit", 4) == 0)
	{
		if (strlen(line) > 4)
		{
			temp = strtok(line, " ");
			temp = strtok(NULL, " ");
			*args = malloc(sizeof(char *) * 3);
			(*args)[0] = "exit";
			(*args)[1] = NULL;
			(*args)[1] = malloc(sizeof(char) * (strlen(temp) + 1));
			(*args)[1] = strcpy((*args)[1], temp);
			(*args)[1] = strcat((*args)[1], "\0");
			(*args)[2] = NULL;
		}
		else
		{
			*args = malloc(sizeof(char *) * 2);
			(*args)[0] = "exit";
			(*args)[1] = NULL;
		}

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
 * @cmd: argument of command
 * @path: path of command
 * @program_name: program_name
 *
 * handles custom argument
 *
 * Return: true if found,
 * false if general
 */
bool execute_custom_command(char **path, char ***cmd, char *program_name)
{
	long int exit_status = EXIT_SUCCESS;
	size_t i;

	if (strcmp((*cmd)[0], "exit") == 0)
	{
		if ((*cmd)[1] != NULL)
		{
			exit_status = strtol((*cmd)[1], NULL, 10);
			for (i = 0; (*cmd)[1][i] != '\0'; i++)
			{
				if (!isdigit((*cmd)[1][i]))
				{
					fprintf(stderr, "%s: 2: exit: Illegal number: %s\n", program_name, (*cmd)[1]);
					free((*cmd)[1]);
					free(*cmd);
					return (true);
				}
			}
			free((*cmd)[1]);
		}
		free(*cmd);
		exit(exit_status);
	}
	else if (strcmp((*cmd)[0], "env") == 0)
	{
		free(*path);
		free(*cmd);
		print_env();
		return (true);
	}
	return (false);
}
/**
 * print_env - environ
 *
 * prints env
 *
 * Return: void
 */
void print_env(void)
{
	char **envs = __environ;

	while (envs != NULL && *envs != NULL)
	{
		printf("%s\n", *envs);
		(envs)++;
	}
}
/**
 * _getline - custom getline
 * @lineptr: address of buffer to fill
 * @len: size of buffer
 * @file: file to read
 *
 * getline at home
 *
 * Return: Pointer to the entered line
 */
ssize_t _getline(char **lineptr, size_t *len, FILE *file)
{
	char buffer[MAX_READ_BUFFER_SIZE + 1] = {'\0'}, *temp;
	int read_len = 0, read_status, temp_len;

	fflush(NULL);
	free(*lineptr);
	*lineptr = malloc(sizeof(char));
	**lineptr = '\0';
	read_status = read(file->_fileno, buffer, MAX_READ_BUFFER_SIZE);
	temp = "\0";
	temp = malloc(sizeof(char) * strlen(*lineptr) + 1);
	temp = strcpy(temp, *lineptr);
	while (read_status > 0)
	{
		read_len = strcspn(buffer, "\n");
		temp_len = strlen(temp) + read_len;
		free(*lineptr);
		*lineptr = malloc(sizeof(char) * (temp_len + 1));
		*lineptr = strcpy(*lineptr, temp);
		*lineptr = strncat(*lineptr, buffer, read_len);
		*len += read_len;
		if (read_status < MAX_READ_BUFFER_SIZE && buffer[read_status - 1] == '\n')
			break;
		free(temp);
		read_status = read(file->_fileno, buffer, 4);
		temp = malloc(sizeof(char) * (strlen(*lineptr) + 1));
		temp = strcpy(temp, *lineptr);
	}
	free(temp);
	if (read_status == 0)
		read_status = -1;
	return (read_status);
}
