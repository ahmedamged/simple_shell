#include "main.h"

char *program_name;
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
	int child_status, exit_status;

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
	char **command_args = NULL, *path = NULL,
		 *line = safe_malloc(sizeof(char) * MAX_LINE_LENGTH), *one_line,
		 *temp, **save_ptr = safe_malloc(sizeof(char) * MAX_LINE_LENGTH);
	int i, status;
	ssize_t len = MAX_LINE_LENGTH;

	i = _getline(&line, &len, stdin);
	if (!is_empty(line))
	{
		if (i == EOF)
		{
			perror(program_name);
			free_args(&command_args);
			exit(139);
		}
		one_line = strtok_r(line, "\n", save_ptr);
		while (one_line != NULL)
		{
			temp = safe_malloc(sizeof(char) * (strlen(one_line) + 1));
			temp = strcpy(temp, one_line);
			i = get_command_args(temp, &command_args, &path);
			if (i != NOT_FOUND && i != EOF)
			{
				status = execute_command(&path, &command_args, program_name, env);
				if (status != EXIT_SUCCESS)
					break;
			}
			else
			{
				status = 127;
				perror(program_name);
				free(path);
				free_args(&command_args);
			}
			free(temp);
			one_line = strtok_r(NULL, "\n", save_ptr);
		}
	}
	free(save_ptr);
	free(line);
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
	char **command_args = NULL, *path = NULL;

	(void)argc;
	program_name = argv[0];
	if (isatty(STDIN_FILENO) == IS_PART_OF_PIPE)
	{
		return (handle_pipe(program_name, env));
	}
	if (isatty(STDIN_FILENO) != IS_PART_OF_PIPE)
	{
		write(STDOUT_FILENO, "($) ", 4);
		read_status = read_command(&path, &command_args);
		while (read_status != EOF)
		{
			if (read_status != IS_NEW_LINE)
			{
				if (read_status != NOT_FOUND && read_status != EOF)
				{
					write_status = execute_command(&path, &command_args, program_name, env);
				}
				else
				{
					perror(program_name);
					free_args(&command_args);
				}
			}
			write(STDOUT_FILENO, "($) ", 4);
			read_status = read_command(&path, &command_args);
		}
		write(STDOUT_FILENO, "\n", 1);
	}
	return (write_status);
}
