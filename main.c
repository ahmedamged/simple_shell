#include "main.h"

#define EXEC_ERROR -1
#define NOT_FOUND 404
#define IS_PART_OF_PIPE 0
#define MAX_LINE_LENGTH 1024
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

	for (i = 0; (*args)[i] != NULL; i++)
	{
		free((*args)[i]);
	}
	free(*args);
}
/**
 * read_command - read commands
 * @command_args: command args
 *
 * formats commands
 *
 * Return: status of getline
 */
int read_command(char ***command_args)
{
	int status;
	char *line = malloc(sizeof(char) * MAX_LINE_LENGTH);
	size_t len = MAX_LINE_LENGTH;

	status = _getline(&line, &len, stdin);
	if (status != EOF)
	{
		if ((line)[strlen(line) - 1] == '\n')
			(line)[strlen(line) - 1] = '\0';
		status = get_command_args(line, command_args);
	}
	free(line);
	return (status);
}
/**
 * execute_command - run commands
 * @argv: arguments to command
 * @program_name: name of program
 * @env: environment
 *
 * executes commands
 *
 * Return: 0 (Success)
 */
int execute_command(char **argv[],
					char *program_name, char **env)
{
	pid_t pid;

	if (!execute_custom_command(argv))
	{
		pid = fork();
		if (pid == EXEC_ERROR)
		{
			perror(program_name);
			exit(1);
		}
		if (pid == 0)
		{
			if (execve((*argv)[0], (*argv), env) == EXEC_ERROR)
			{
				free_args(argv);
				perror(program_name);
				exit(1);
			}
			exit(0);
		}
		else
		{
			wait(NULL);
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
	char **command_args;
	size_t i;

	i = read_command(&command_args);
	if (i == 0)
		execute_command(&command_args, program_name, env);
	else
	{
		free_args(&command_args);
		printf("%s: No such file or directory\n", program_name);
	}
	printf("($) ");
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
	char **command_args;

	command_args = NULL;
	(void)argc;
	if (isatty(STDIN_FILENO) == IS_PART_OF_PIPE)
	{
		return (handle_pipe(argv[0], env));
	}
	if (isatty(STDIN_FILENO) != IS_PART_OF_PIPE)
	{
		printf("($) ");
		status = read_command(&command_args);
		while (status != EOF)
		{
			if (status != NOT_FOUND && status != EOF)
			{
				execute_command(&command_args, argv[0], env);
			}
			else
			{
				printf("%s: No such file or directory\n", argv[0]);
				free_args(&command_args);
			}
			printf("($) ");
			status = read_command(&command_args);
		}
		printf("\n");
	}
	return (0);
}
