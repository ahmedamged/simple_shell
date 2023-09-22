#include "main.h"
#define MAX_READ_BUFFER_SIZE 1024
static char buffer[MAX_READ_BUFFER_SIZE + 1] = {'\0'}, *temp;
static ssize_t read_len = 0, read_status, temp_len;
/**
 * handle_custom_command - command format
 * @line: string to find argument
 * @args: arguments
 * @program_name: program name
 *
 * handles custom argument
 *
 * Return: true if found,
 * false if general
 */
bool handle_custom_command(char *line, char ***args, char *program_name)
{
	char *temp = NULL;

	if (strncmp(line, "exit", 4) == 0)
	{
		temp = strtok(line, " ");
		temp = strtok(NULL, " ");
		if (temp != NULL)
		{
			*args = safe_malloc(sizeof(char *) * 3, program_name);
			(*args)[0] = "exit";
			(*args)[1] = NULL;
			(*args)[1] = safe_malloc(sizeof(char) * (strlen(temp) + 1), program_name);
			(*args)[1] = strcpy((*args)[1], temp);
			(*args)[1] = strcat((*args)[1], "\0");
			(*args)[2] = NULL;
		}
		else
		{
			*args = safe_malloc(sizeof(char *) * 2, program_name);
			(*args)[0] = "exit";
			(*args)[1] = NULL;
		}

		return (true);
	}
	else if (strncmp(line, "env", 3) == 0)
	{
		*args = safe_malloc(sizeof(char *) * 2, program_name);
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
					fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
							program_name, (*cmd)[1]);
					free((*cmd)[1]);
					free(*cmd);
					exit(2);
				}
			}
			free((*cmd)[1]);
		}
		free(*cmd);
		exit(exit_status < 0 ? -1 : exit_status);
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
 * @program_name: program name
 *
 * getline at home
 *
 * Return: Pointer to the entered line
 */
ssize_t _getline(char **lineptr, ssize_t *len, FILE *file, char *program_name)
{

	fflush(NULL);
	read_status = read(file->_fileno, buffer, MAX_READ_BUFFER_SIZE);
	if (read_status < *len)
	{
		if (read_status == 0)
			return (-1);
		*lineptr = strcpy(*lineptr, buffer);
		return (read_status);
	}
	**lineptr = '\0';
	while (read_status > 0)
	{
		read_status = read(file->_fileno, buffer, MAX_READ_BUFFER_SIZE);
		temp = safe_malloc(sizeof(char) * (strlen(*lineptr) + 1), program_name);
		temp = strcpy(temp, *lineptr);
		read_len = strcspn(buffer, "\n");
		temp_len = strlen(temp) + read_len;
		free(*lineptr);
		*lineptr = safe_malloc(sizeof(char) * (temp_len + 1), program_name);
		*lineptr = strcpy(*lineptr, temp);
		*lineptr = strncat(*lineptr, buffer, read_len);
		*len += read_len;
		if (read_status < MAX_READ_BUFFER_SIZE && buffer[read_status - 1] == '\n')
			break;
		free(temp);
	}
	free(temp);
	return (temp_len);
}
