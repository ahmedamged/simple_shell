#include "main.h"
/**
 * read_command - read commands
 * @path: fath for command
 * @command_args: command args
 * @p: command args
 *
 * formats commands
 *
 * Return: status of getline
 */
int read_command(char **path, char ***command_args, char *p)
{
	int status;
	char *line = safe_malloc(sizeof(char) * MAX_LINE_LENGTH, p);
	ssize_t len = MAX_LINE_LENGTH;

	status = _getline(&line, &len, stdin, p);
	if (status != EOF)
	{
		if (strlen(line) == 1 && line[0] == '\n')
		{
			free(line);
			return (IS_NEW_LINE);
		}
		if ((line)[strlen(line) - 1] == '\n')
			(line)[strlen(line) - 1] = '\0';
		status = get_command_args(line, command_args, path, p);
	}
	free(line);
	return (status);
}
/**
 * execute_command - run commands
 * @path: arguments to command
 * @argv: arguments to command
 * @p: name of program
 * @env: environment
 *
 * executes commands
 *
 * Return: 0 (Success)
 */
int execute_command(char **path, char **argv[],
					char *p, char **env)
{
	pid_t pid;
	int child_status, exit_status;

	if (!execute_custom_command(path, argv, p))
	{
		pid = fork();
		if (pid == EXEC_ERROR)
		{
			perror(p);
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (execve(*path, (*argv), env) == EXEC_ERROR)
			{
				free(*path);
				free_args(argv);
				perror(p);
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			wait(&child_status);
			exit_status = WEXITSTATUS(child_status);
			free(*path);
			free_args(argv);
		}
	}
	return (exit_status);
}
/**
 * handle_pipe - run commands
 * in pipe mode
 * @p: name of program
 * @env: environment
 *
 * executes commands in a pipe
 *
 * Return: 0 (Success)
 * 1 (Error)
 */
int handle_pipe(char *p, char **env)
{
	char **command_args = NULL, *path = NULL,
		 *line = safe_malloc(sizeof(char) * MAX_LINE_LENGTH, p), *one_line,
		 *temp, **save_ptr = safe_malloc(sizeof(char) * MAX_LINE_LENGTH, p);
	ssize_t i, status = 0, len = MAX_LINE_LENGTH;

	i = _getline(&line, &len, stdin, p);
	if (!is_empty(line))
	{
		if (i == EOF)
		{
			perror(p);
			free_args(&command_args);
			exit(139);
		}
		one_line = strtok_r(line, "\n", save_ptr);
		while (one_line != NULL)
		{
			temp = safe_cpy(temp, one_line, p);
			i = get_command_args(temp, &command_args, &path, p);
			free(temp);
			if (i != NOT_FOUND && i != EOF)
			{
				if (strcmp(command_args[0], "exit") == 0)
					free_many(2, line, save_ptr);
				status = execute_command(&path, &command_args, p, env);
				if (status != EXIT_SUCCESS)
					break;
			}
			else
			{
				status = 127;
				fprintf(stderr, "%s: 1: %s: not found\n", p, command_args[0]);
				free(path);
				free_args(&command_args);
			}
			one_line = strtok_r(NULL, "\n", save_ptr);
		}
	}
	free_many(2, save_ptr, line);
	return (status);
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
	int read_status, write_status;
	char **command_args = NULL, *path = NULL,
		 *p = argv[0];

	(void)argc;

	if (isatty(STDIN_FILENO) == IS_PART_OF_PIPE)
	{
		return (handle_pipe(p, env));
	}
	if (isatty(STDIN_FILENO) != IS_PART_OF_PIPE)
	{
		write(STDOUT_FILENO, "($) ", 4);
		read_status = read_command(&path, &command_args, p);
		while (read_status != EOF)
		{
			if (read_status != IS_NEW_LINE)
			{
				if (read_status != NOT_FOUND && read_status != EOF)
				{
					write_status = execute_command(&path, &command_args, p, env);
				}
				else
				{
					fprintf(stderr, "%s: 1: %s: not found\n", p, command_args[0]);
					free_args(&command_args);
				}
			}
			write(STDOUT_FILENO, "($) ", 4);
			read_status = read_command(&path, &command_args, p);
		}
		write(STDOUT_FILENO, "\n", 1);
	}
	return (write_status);
}
