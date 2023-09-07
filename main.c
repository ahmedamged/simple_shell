#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define EXEC_ERROR -1
#define IS_PART_OF_PIPE 0

/**
 * read_command - read commands
 * @command: command to run
 * @len: length of command
 *
 * formats commands
 *
 * Return: status of getline
 */
int read_command(char **command, size_t *len)
{
	int status;

	status = getline(command, len, stdin);
	if ((*command)[strlen(*command) - 1] == '\n')
		(*command)[strlen(*command) - 1] = '\0';
	return (status);
}
/**
 * execute_command - run commands
 * @command: command to run
 * @argv: arguments to command
 * @program_name: name of program
 * @env: environment
 *
 * executes commands
 *
 * Return: void
 */
void execute_command(char *command, char **argv,
					 char *program_name, char **env)
{
	if (execve(command, argv, env) == EXEC_ERROR)
	{
		perror(program_name);
	}
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
	char *command = NULL, *command_args[] = {NULL};
	size_t len = 0;
	pid_t pid;

	read_command(&command, &len);
	pid = fork();
	if (pid == EXEC_ERROR)
	{
		perror(program_name);
		return (1);
	}
	if (pid == 0)
	{
		execute_command(command, command_args, program_name, env);
	}
	else
	{
		wait(NULL);
		printf("#cisfun$ ");
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
	char *command = NULL, *command_args[] = {NULL};
	size_t len = 0;
	pid_t pid;

	(void)argc;
	if (isatty(STDIN_FILENO) == IS_PART_OF_PIPE)
	{
		return (handle_pipe(argv[0], env));
	}
	if (isatty(STDIN_FILENO) != IS_PART_OF_PIPE)
	{
		printf("#cisfun$ ");
		status = read_command(&command, &len);
		while (status != EOF)
		{
			pid = fork();
			if (pid == EXEC_ERROR)
			{
				perror(argv[0]);
				return (1);
			}
			if (pid == 0)
			{
				execute_command(command, command_args, argv[0], env);
				free(command);
				return (0);
			}
			else
			{
				wait(NULL);
				printf("#cisfun$ ");
				status = read_command(&command, &len);
			}
		}
		free(command);
		printf("\n");
	}
	return (0);
}
