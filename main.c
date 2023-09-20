#include "main.h"

char *program_name;
/**
 * free_args - memory
 * @args: pointer to free
 *
 * frees memory of args
 *
 * Return: void
 */
void free_args(char **args[])
{
	size_t i;

	if (*args != NULL)
	{
		for (i = 0; (*args)[i] != NULL; i++)
		{
			free((*args)[i]);
		}
		free(*args);
	}
}
/**
 * read_command - read commands
 * @path: fath for command
 * @command_args: command args
 *
 * formats commands
 *
 * Return: status of getline
 */
int read_command(char **path, char ***command_args)
{
	int status;
	char *line = safe_malloc(sizeof(char) * MAX_LINE_LENGTH);
	ssize_t len = MAX_LINE_LENGTH;

	status = _getline(&line, &len, stdin);
	if (status != EOF)
	{
		if (strlen(line) == 1 && line[0] == '\n')
		{
			free(line);
			return (IS_NEW_LINE);
		}
		if ((line)[strlen(line) - 1] == '\n')
			(line)[strlen(line) - 1] = '\0';
		status = get_command_args(line, command_args, path);
	}
	free(line);
	return (status);
}
/**
 * execute_command - run commands
 * @path: arguments to command
 * @argv: arguments to command
 * @program_name: name of program
 * @env: environment
 *
 * executes commands
 *
 * Return: 0 (Success)
 */
int execute_command(char **path, char **argv[],
					char *program_name, char **env)
{
	pid_t pid;

	if (!execute_custom_command(path, argv, program_name))
	{
		pid = fork();
		if (pid == EXEC_ERROR)
		{
			perror(program_name);
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (execve(*path, (*argv), env) == EXEC_ERROR)
			{
				free(*path);
				free_args(argv);
				perror(program_name);
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			wait(NULL);
			free(*path);
			free_args(argv);
		}
	}
	return (0);
}
/**
 * handle_pipe - run commands
 * in pipe mode
 * @program_name: name of program
 * @env: environment
 *
 * executes commands in a pipe
 *
 * Return: 0 (Success)
 * 1 (Error)
 */
int handle_pipe(char *program_name, char **env)
{
	char **command_args = NULL, *path = NULL;
	size_t i;

	i = read_command(&path, &command_args);
	if (i == 0)
		execute_command(&path, &command_args, program_name, env);
	else
	{
		perror(program_name);
		free_args(&command_args);
		exit(139);
	}
	return (0);
}
/**
 * main - simple shell
 * @argc: number of arguments
 * @argv: arguments
 * @env: environment
 *
 * emulates how a shell works
 *
 * Return: 0 (Success)
 * or 1 (Fail)
 */
int main(int argc, char *argv[], char **env)
{
	int status;
	char **command_args = NULL, *path = NULL;

	(void)argc;
	program_name = argv[0];
	if (isatty(STDIN_FILENO) == IS_PART_OF_PIPE)
	{
		return (handle_pipe(program_name, env));
	}
	if (isatty(STDIN_FILENO) != IS_PART_OF_PIPE)
	{
		printf("($) ");
		status = read_command(&path, &command_args);
		while (status != EOF)
		{
			if (status != IS_NEW_LINE)
			{
				printf("status: %d\n", status);
				if (status != NOT_FOUND && status != EOF)
				{
					execute_command(&path, &command_args, program_name, env);
				}
				else
				{
					perror(program_name);
					free_args(&command_args);
				}
			}
			printf("($) ");
			status = read_command(&path, &command_args);
		}
		printf("\n");
	}
	return (0);
}
